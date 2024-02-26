#ifndef SERVER_HPP
#define SERVER_HPP

#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <sstream>
#include <poll.h>
#include <vector>
#include <cerrno>
#include <sstream>


class Server;

#include "Client.hpp"
#include "CommandHandler.hpp"
#include "Channel.hpp"

#define MAX_CONNECTIONS 200


class ServerQuitException: public std::exception
{
	public:
		std::string	err() { return ("Quitting from server"); }
};

class Server
{
	private:
		std::string				port;
		std::string				password;
		std::map<int, Client *>	clients;
		int						sock;
		int						on;
		std::vector<pollfd>		poll_fds;
		CommandHandler			*handler;
		std::vector<Channel *>	channels;

	public:
		Server(std::string const &port, std::string const &password);
		~Server();


		std::string				getPassword() { return (this->password); }
		void					add_client();
		int						socket_init();
		int						handle_input(int fd);
		std::string				receive(int fd);
		void					init();
		Channel					*createChannel(std::string const &name, std::string const &password);
		Channel					*getChannel(std::string const &name);
		void					quit_server(int fd);
		Client					*getClient(std::string const &name);
		Client					*search_client(std::string const &name);
		std::vector<Channel *>	getChannels() { return (this->channels); }
};

#endif
