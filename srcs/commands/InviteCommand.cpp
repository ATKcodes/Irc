#include "Command.hpp"

InviteCommand::InviteCommand(Server *server, int auth) : Command(server, auth) {}

InviteCommand::~InviteCommand() {}

void InviteCommand::exec(Client *client, std::vector<std::string> args)
{
	if ((int)args.size() < 2)
	{
		client->send_msg(ERRORPARAMS(client->getNickname(), "INVITE"));
		return ;
	}

	std::string	nickname = args.at(0);
	std::string	channelName = args.at(1);

	Client	*dst = this->server->getClient(nickname);
	if (dst == nullp)
	{
		client->send_msg(ERRORNICKNOTFOUND(client->getNickname(), nickname));
		return ;
	}
	Channel	*channel = this->server->getChannel(channelName);
	if (channel == nullp)
	{
		client->send_msg(ERRORCHANNELNOTFOUND(client->getNickname(), channelName));
		return ;
	}
	if (!channel->isMember(client))
	{
		client->send_msg(ERRORWRONGCHANNEL(client->getNickname(), channelName));
		return ;
	}
	if (channel->getLimit() != -1 && channel->getLimit() <= channel->getMembers())
	{
		client->send_msg(EERRORFULLCHANNEL(client->getNickname(), channelName));
		return ;
	}
	if (dst->getChannel() != nullp)
	{
		client->send_msg(ERRORUSERALREADYIN(client->getNickname(), dst->getNickname(), channelName));
		return ;
	}
	client->invite(dst, channel);
}