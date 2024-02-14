#include "Command.hpp"

PartCommand::PartCommand(Server *server, int auth) : Command(server, auth) {}

PartCommand::~PartCommand() {}

void	PartCommand::execute(Client *client, std::vector<std::string> args)
{
	Channel *channel = client->getChannel();
	if (channel == nullp)
	{
		client->msgReply(ERR_NOTONCHANNEL(client->getNickname(), ""));
		return;
	}
	std::string hexfix = "#" + channel->getName();
	std::cout <<"hexfix : " << hexfix << std::endl;
	if (args.size() != 0 && args.at(0) != hexfix)
	{
		client->leave();
	}
	else
	{
		std::string	name = args.at(0);
		std::string	reason = args.at(1);
		std::string msg = "PART " + name + " " + reason;
		channel->broadcast(msg);
		client->leave();
	}
}