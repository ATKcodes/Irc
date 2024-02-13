#include "Command.hpp"

ModeCommand::ModeCommand(Server *server, int auth) : Command(server, auth) {}
ModeCommand::~ModeCommand() {}

void ModeCommand::execute(Client *client, std::vector<std::string> args)
{
	Channel	*channel = client->getChannel();
	// for (size_t i = 0; i < args.size(); i++)
	// {
	// 	std::cout << "args[" << i << "] = " << args[i] << std::endl;
	// }
	if (channel == nullp)
	{
		client->msgReply(client->getNickname() + " :You're not on a channel");
		return ;
	}
	if (args.empty())
	{
		client->msgReply("MODE " + channel->getName() + " " + channel->getMode());
		return ;
	}
	if (client->getStatus() != 2)
	{
		client->msgReply(ERR_NOTADMIN(client->getNickname()));
		return ;
	}
	int i;
	if (args[0] == channel->getName()) //dumb shit to fix hexchat
	{
		i = 1;
	}
	else
	{
		i = 0;
	}
	std::string mode = args[i];
	if (mode == "+o" || mode == "-o")
	{
		if (args.size() < (2 + (unsigned int)i))
		{
			client->msgReply("MODE " + channel->getName() + " " + channel->getMode());
			return ;
		}
		i++;
		std::string nickname = args[i];
		Client *target = this->server->find_client(nickname);
		if (target == nullp)
		{
			client->msgReply(ERR_NOSUCHNICK(client->getNickname(), nickname));
			return ;
		}
		if (mode == "+o")
		{
			if (!channel->isOperator(client))
			{
				client->msgReply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()));
				return ;
			}
			channel->addOperator(target);
			client->msgChannel(channel, "MODE " + channel->getName() + " +o " + target->getNickname());
		}
		else
		{
			if (!channel->isOperator(target))
			{
				client->msgReply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()));
				return ;
			}
			channel->removeOperator(target);
			client->msgChannel(channel, "MODE " + channel->getName() + " -o " + target->getNickname());
		}
	}
	else if (mode == "+l" || mode == "-l")
	{
		if (mode == "+l")
		{
			if (args.size() < (2 + (unsigned int)i))
			{
				client->msgReply("MODE " + channel->getName() + " " + channel->getMode());
				return ;
			}
			int limit;
			std::istringstream iss(args[(1 + i)]);
			iss >> limit;
			channel->setLimit(limit);
			std::stringstream ss;
			ss << limit;
			client->msgChannel(channel, "MODE " + channel->getName() + " +l " + ss.str());
		}
		else
		{
			channel->setLimit(-1);
			client->msgChannel(channel, "MODE " + channel->getName() + " -l");
		}
	}
	else if (mode == "+k" || mode == "-k")
	{
		if (mode == "+k")
		{
			if (args.size() < (2 + (unsigned int)i))
			{
				client->msgReply("MODE " + channel->getName() + " " + channel->getMode());
				return ;
			}
			std::string key = args[(1 + i)];
			channel->setKey(key);
			client->msgChannel(channel, "MODE " + channel->getName() + " +k " + key);
		}
		else
		{
			channel->setKey("");
			client->msgChannel(channel, "MODE " + channel->getName() + " -k");
		}
	}
	else if (mode == "+i" || mode == "-i")
	{
		if (mode == "+i")
		{
			channel->setInviteOnly(true);
			client->msgChannel(channel, "MODE " + channel->getName() + " +i");
		}
		else
		{
			channel->setInviteOnly(false);
			client->msgChannel(channel, "MODE " + channel->getName() + " -i");
		}
	}
	else if (mode == "+t" || mode == "-t")
	{
		if (mode == "+t")
		{
			channel->setTopicOnly(true);
			client->msgChannel(channel, "MODE " + channel->getName() + " +t");
		}
		else
		{
			channel->setTopicOnly(false);
			client->msgChannel(channel, "MODE " + channel->getName() + " -t");
		}
	}
	else
	{
		client->msgReply(ERR_UNKNOWNMODE(client->getNickname(), mode));
	}
}
