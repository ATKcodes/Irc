#include "Command.hpp"

InviteCommand::InviteCommand(Server *server, int auth) : Command(server, auth) {}

InviteCommand::~InviteCommand() {}

void InviteCommand::execute(Client *client, std::vector<std::string> args)
{
	if ((int)args.size() < 2)
	{
		client->msgReply(ERRORPARAMS(client->getNickname(), "INVITE"));
		return ;
	}

	std::string	nickname = args.at(0);
	std::string	channelName = args.at(1);

	Client	*dst = this->server->getClient(nickname);
	if (dst == nullp)
	{
		client->msgReply(ERRORNICKNOTFOUND(client->getNickname(), nickname));
		return ;
	}
	Channel	*channel = this->server->getChannel(channelName);
	if (channel == nullp)
	{
		client->msgReply(ERRORCHANNELNOTFOUND(client->getNickname(), channelName));
		return ;
	}
	if (!channel->isMember(client))
	{
		client->msgReply(ERRORWRONGCHANNEL(client->getNickname(), channelName));
		return ;
	}
	if (channel->getLimit() != -1 && channel->getLimit() <= channel->getMembers())
	{
		client->msgReply(EERRORFULLCHANNEL(client->getNickname(), channelName));
		return ;
	}
	if (dst->getChannel() != nullp)
	{
		client->msgReply(ERRORUSERALREADYIN(client->getNickname(), dst->getNickname(), channelName));
		return ;
	}
	client->invite(dst, channel);
}