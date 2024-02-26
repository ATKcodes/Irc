#include "Command.hpp"

PartCommand::PartCommand(Server *server, int auth) : Command(server, auth) {}

PartCommand::~PartCommand() {}

void	PartCommand::exec(Client *client, std::vector<std::string> args)
{
	Channel *channel = client->getChannel();
	if (channel == nullp)
	{
		client->send_msg(ERRORWRONGCHANNEL(client->getNickname(), ""));
		return;
	}
	std::string hexfix = "#" + channel->getName();
	if (args.size() != 0 && args.at(0) != hexfix)
	{
		client->leave();
	}
	else
	{
		std::string msg = "PART " + args.at(0) + " " + args.at(1);
		channel->send_all(msg);
		client->leave();
	}
}