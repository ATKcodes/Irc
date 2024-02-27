#include "Command.hpp"

JoinCommand::JoinCommand(Server *server, int auth) : Command(server, auth) {}

JoinCommand::~JoinCommand() {}

void	JoinCommand::exec(Client *client, std::vector<std::string> args)
{
	if (args.empty())
	{
		client->send_msg(ERRORPARAMS(client->getNickname(), "JOIN"));
		return ;
	}
	std::string	password = "";
	std::string	name = args.at(0);
	if (args.size() > 1)
		password = args.at(1);
	Channel	*channel = client->getChannel();
	if (channel != nullp)
	{
		client->send_msg(ERRORTOOMANYCHANNELS(client->getNickname(), args.at(0)));
		return ;
	}

	channel = this->server->getChannel(args.at(0));
	if (channel == nullp)
		channel = this->server->createChannel(args.at(0), password);

	if (password.compare(channel->getPassword()))
	{
		client->send_msg(ERRORWRONGPASSCHANNEL(client->getNickname(), args.at(0)));
		return ;
	}
	if(channel->getLimit() != -1 && channel->getLimit() <= channel->getMembers())
	{
		client->send_msg(EERRORFULLCHANNEL(client->getNickname(), args.at(0)));
		return ;
	}
	if(channel->isInviteOnly())
	{
		client->send_msg(ERRORINVITEONLY(client->getNickname(), args.at(0)));
		return ;
	}
	client->join(channel);
}
