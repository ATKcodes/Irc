#include "Command.hpp"

TopicCommand::TopicCommand(Server *server, int auth) : Command(server, auth) {}

TopicCommand::~TopicCommand() {}

void TopicCommand::execute(Client *client, std::vector<std::string> args)
{
	Channel	*channel = client->getChannel();
	if (channel == nullp)
	{
		client->msgReply(client->getNickname() + " :You're not on a channel");
		return ;
	}
	std::string old_topic = channel->getTopic();
	if (args.empty() || (args[0] == channel->getName() && args.size() == 1)) //dumb fixes for hexchat
	{
		client->msgReply("Topic for " + channel->getName() + " is : " + channel->getTopic());
		return ;
	}

	if (client->getStatus() == 1 && !channel->isTopicOnly()) //bypassing the else if
	{
		channel->setTopicOnly(false);
	}
	else if (client->getStatus() != 2)
	{
		client->msgReply(ERR_NOTADMIN(client->getNickname()));
		return ;
	}
	std::string topic;
	std::vector<std::string>::iterator it = args.begin();
	if (*it == channel->getName())
		it++;
	while (it != args.end())
	{
		topic += *it;
		it++;
		if (it != args.end())
			topic += " ";
	}

	std::string	channelName = channel->getName();

	channel->setTopic(topic);
	client->msgChannel(channel, "TOPIC " + channelName + " " + topic);
}