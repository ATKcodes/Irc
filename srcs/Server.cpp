#include "Server.hpp"

Server::Server(std::string const &port, std::string const &password)
{
	this->port = port;
	this->password = password;
	this->on = 1;
	this->sock = this->socket_init();
	this->handler = new CommandHandler((this));
}

Server::~Server()
{
	std::vector<int> fdlist;

	for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
		fdlist.push_back(it->second->getFd());
	for (std::vector<int>::iterator it = fdlist.begin(); it != fdlist.end(); ++it)
	{
		this->clients[*it]->send_msg("Closing the server\n");
		this->quit_server(*it);
	}
	for (std::vector<Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
		delete *it;
	delete this->handler;
	close(this->sock);
}

void	handle_sigint(int sig)
{
	(void)sig;
	std::cout << "\nServer shutting down\n";
	throw ServerQuitException();
}

void	Server::init()
{
	pollfd	server_fd = {this->sock, POLLIN, 0};
	poll_fds.push_back(server_fd);

	signal(SIGINT, handle_sigint);
	print_time("Server waiting for connections");
	while (this->on)
	{
		if (poll(poll_fds.begin().base(), poll_fds.size(), -1) < 0)
			throw std::runtime_error("Error while polling");
		// event handling
		for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end(); ++it)
		{
			// no events
			if (it->revents == 0)
				continue ;
			// on connect
			if ((it->revents & POLLIN) == POLLIN)
			{
				if (it->fd == this->sock)
				{
					this->add_client();
					break ;
				}
				if (this->handle_input(it->fd))
					break ;
			}
			// on disconnect
			if ((it->revents & POLLHUP) == POLLHUP)
			{
				this->quit_server(it->fd);
				break ;
			}
		}
	}
}

int	Server::socket_init()
{
	int	server_socket;
	int	port_struct;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
		throw std::runtime_error("Error socket : open");
	if (fcntl(server_socket, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error socket : NON-BLOCKING");
	struct sockaddr_in addr = {};
	bzero((char *)&addr, sizeof(addr));
	std::istringstream(this->port) >> port_struct;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port_struct);
	if (bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("Error socket : bind");
	if (listen(server_socket, MAX_CONNECTIONS) < 0)
		throw std::runtime_error("Error socket : listen");
	return (server_socket);
}

void	Server::add_client()
{
	sockaddr_in	addr = {};
	int			fd;
	std::string	ip;
	socklen_t 	size;

	// accept connection
	size = sizeof(addr);
	fd = accept(this->sock, (sockaddr *)&addr, &size);
	if (fd < 0)
		throw std::runtime_error("New Client Error");
	// save th client's fd
	pollfd	poll_fd = {fd, POLLIN, 0};
	this->poll_fds.push_back(poll_fd);
	// get client info
	ip = inet_ntoa(addr.sin_addr);
	if (getsockname(fd, (struct sockaddr *)&addr, &size) != 0)
		throw std::runtime_error("New Client Error");
	// create a new client
	Client *new_client = new Client(fd, ip, ntohs(addr.sin_port));
	this->clients.insert(std::make_pair(fd, new_client));
	// log new connection
	print_time(new_client->log("has connected"));
}

std::string Server::receive(int fd)
{
	std::string msg;
	char buffer[100];

	memset(buffer, 0, sizeof(buffer));
	// receive until newline
	while (msg.find('\n') == std::string::npos)
	{
		memset(buffer, 0, sizeof(buffer));
		int bytesReceived = recv(fd, buffer, sizeof(buffer) - 1, 0);
		if (bytesReceived < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("Error while receiving from client");
		}
		// empty buffer means exit (UNIX)
		if (bytesReceived == 0)
			return "";
		msg.append(buffer, bytesReceived);
	}
	return msg;
}


int Server::handle_input(int fd)
{
	std::string msg = this->receive(fd);
	if (DEBUG)
		print_time(msg);
	
	if (msg.empty() || msg == "\n")
	{
		this->quit_server(fd);
		return 1;
	}
	
	Client* client = this->clients.at(fd);
	if (this->handler->manage_cmds(client, msg))
	{
		this->quit_server(client->getFd());
		return 1;
	}
	return 0;
}

void	Server::quit_server(int fd)
{
	try
	{
		Client	*client = this->clients.at(fd);

		// remove the client from the channel
		client->leave();
		// remove the client
		this->clients.erase(fd);
		for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end(); ++it)
		{
			if (it->fd != fd)
				continue ;
			this->poll_fds.erase(it);
			close(fd);
			break ;
		}
		// message of disconnection
		print_time(client->log("has disconnected"));
		delete client;
	}
	catch (std::out_of_range const &err) {}
}

Client	*Server::getClient(std::string const &name)
{
	std::map<int, Client *>::iterator it;

	for (it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (!name.compare(it->second->getNickname()))
			return (it->second);
	}
	return (nullp);
}

Channel	*Server::getChannel(std::string const &name)
{
	std::vector<Channel *>::iterator it;

	for (it = this->channels.begin(); it != this->channels.end(); ++it)
	{
		if (!name.compare(it.operator*()->getName()))
			return (it.operator*());
	}
	return (nullp);
}

Channel	*Server::createChannel(std::string const &name, std::string const &password)
{
	Channel	*channel = new Channel(name, password);
	this->channels.push_back(channel);
	return (channel);
}

Client	*Server::search_client(std::string const &name)
{
	std::map<int, Client *>::iterator it;

	for (it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (!name.compare(it->second->getNickname()))
			return (it->second);
	}
	return (nullp);
}
