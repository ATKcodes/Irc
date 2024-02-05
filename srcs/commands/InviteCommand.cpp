#include "Command.hpp"

InviteCommand::InviteCommand(Server *server, int auth) : Command(server, auth) {}

InviteCommand::~InviteCommand() {}

void InviteCommand::execute(Client *client, std::vector<std::string> args)
{
	if ((int)args.size() < 2)
	{
		client->msgReply(ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE"));
		return ;
	}

	std::string	nickname = args.at(0);
	std::string	channelName = args.at(1);

	Client	*dst = this->server->getClient(nickname);
	if (dst == nullp)
	{
		client->msgReply(ERR_NOSUCHNICK(client->getNickname(), nickname));
		return ;
	}
	Channel	*channel = this->server->getChannel(channelName);
	if (channel == nullp)
	{
		client->msgReply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
		return ;
	}
	if (!channel->isMember(client))
	{
		client->msgReply(ERR_NOTONCHANNEL(client->getNickname(), channelName));
		return ;
	}
	if (channel->getLimit() != -1 && channel->getLimit() <= channel->getMembers())
	{
		client->msgReply(ERR_CHANNELISFULL(client->getNickname(), channelName));
		return ;
	}
	if (dst->getChannel() != nullp)
	{
		client->msgReply(ERR_USERONCHANNEL(client->getNickname(), dst->getNickname(), channelName));
		return ;
	}
	client->invite(dst, channel);
}