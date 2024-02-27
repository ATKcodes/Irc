#include "Command.hpp"

UserCommand::UserCommand(Server *server, int auth) : Command(server, auth) {}

UserCommand::~UserCommand() {}

void UserCommand::exec(Client *client, std::vector<std::string> args)
{
	if (args.size() < 4)
	{
		client->send_msg(client->getNickname() + " USER" + " : Usage: USER <username> <realname>\r\n");
		return;
	}
	client->setUsername(args.at(0));
	client->setRealname(args.at(1));
	client->welcome();
}
