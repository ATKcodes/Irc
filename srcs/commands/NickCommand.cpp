#include "Command.hpp"

NickCommand::NickCommand(Server *server, int auth) : Command(server, auth) {}

NickCommand::~NickCommand() {}

void NickCommand::exec(Client *client, std::vector<std::string> args)
{
	if (args.empty() || args.at(0).empty())
	{
		client->send_msg(ERRORBLANKNICK(client->getNickname()));
		return;
	}
	if (this->server->getClient(args.at(0)))
	{
		client->send_msg(ERRORDOUBLENICK(client->getNickname()));
		return;
	}
	client->setNickname(args.at(0));
	client->welcome();
}
