#include "Command.hpp"

KickCommand::KickCommand(Server *server, int auth) : Command(server, auth) {}

KickCommand::~KickCommand() {}

void	KickCommand::exec(Client *client, std::vector<std::string> args)
{
	if ((int)args.size() < 2)
	{
		client->send_msg(ERRORPARAMS(client->getNickname(), "KICK"));
		return ;
	}
	
	std::string	reason = "No reason specified";

	if (args.size() >= 3 && (args[2][0] != ':' || args[2].size() > 1))
	{
		reason = "";
		for (std::vector<std::string>::iterator it = args.begin() + 2; it != args.end(); it++)
			reason.append(*it + " ");
	}

	Channel	*channel = client->getChannel();
	if (channel == nullp || channel->getName() != args.at(0))
	{
		client->send_msg(ERRORWRONGCHANNEL(client->getNickname(), args.at(0)));
		return ;
	}

	Client	*dst = this->server->getClient(args.at(1));
	if (dst == nullp)
	{
		client->send_msg(ERRORNICKNOTFOUND(client->getNickname(), args.at(1)));
		return;
	}

	if (dst->getChannel() == nullp || dst->getChannel() != channel)
	{
		client->send_msg(ERRORKICKNOTFOUND(client->getNickname(), dst->getNickname(), args.at(0)));
		return;
	}

	channel->kick(client, dst, reason);
}
