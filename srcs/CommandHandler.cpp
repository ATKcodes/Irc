#include "CommandHandler.hpp"

CommandHandler::CommandHandler(Server *server)
{
	this->server = server;

	this->commands["PASS"] = new PassCommand(server, 0);
	this->commands["NICK"] = new NickCommand(server, 0);
	this->commands["USER"] = new UserCommand(server, 0);
	
	this->commands["JOIN"] = new JoinCommand(server, 1);
	this->commands["PART"] = new PartCommand(server, 1);
	this->commands["PRIVMSG"] = new PrivMsgCommand(server, 1);
	this->commands["TOPIC"] = new TopicCommand(server, 1);

	this->commands["MODE"] = new ModeCommand(server, 2);
	this->commands["KICK"] = new KickCommand(server, 2);
	this->commands["INVITE"] = new InviteCommand(server, 2);
}

CommandHandler::~CommandHandler()
{
	std::map<std::string, Command *>::iterator	it;

	for (it = this->commands.begin(); it != this->commands.end(); ++it)
		delete it->second;
}

int	CommandHandler::manage_cmds(Client *client, std::string cmd)
{
	std::stringstream	ss_cmd(cmd);
	std::string			parsed;
	int					size;
	std::string			cmd_name;

	while (std::getline(ss_cmd, parsed))
	{
		std::string	msg;
		size = parsed.length();
		if (parsed[parsed.length() - 1] == '\n')
			size -= 1; 
		parsed = parsed.substr(0, size);
		cmd_name = parsed.substr(0, parsed.find(' '));
		try
		{
			Command						*command = this->commands.at(cmd_name);
			std::vector<std::string>	args;
			std::stringstream			ss_args(parsed.substr(cmd_name.length(), parsed.length()));
			std::string					arg;

			// if he has no authorization
			if (client->getStatus() < command->getAuth())
			{
				if (!client->getStatus())
					client->send_msg(ERRORNOTREGISTERED(client->getNickname()));
				else
					client->send_msg(ERRORNOTADMIN(client->getNickname(), client->getChannel()->getName()));
				return (0);
			}
			while (ss_args >> arg)
				args.push_back(arg);
			// exec the command
			command->exec(client, args);
		}
		catch (const std::out_of_range &err)
		{

			client->send_msg(ERRORCOMMAND(client->getNickname(), cmd_name));
		}
	}
	return (0);
}
