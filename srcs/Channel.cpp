#include "Channel.hpp"

Channel::Channel(std::string const &name, std::string const &password)
{
	this->name = name;
	this->password = password;
	this->limit = -1;
	this->topic = "Welcome to " + name;
	this->inviteOnly = false;
	this->topicOnly = false;
}

Channel::~Channel() {}

void	Channel::setLimit(int limit)
{
	this->limit = limit;
}

int	Channel::getLimit() const
{
	return (this->limit);
}

bool	Channel::isInviteOnly() const
{
	return (this->inviteOnly);
}

bool	Channel::isTopicOnly() const
{
	return (this->topicOnly);
}

std::vector<std::string>	Channel::getNicknames()
{
	std::vector<std::string>	nicknames;

	for (std::vector<Client *>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		nicknames.push_back((*it)->getNickname());
	}
	return (nicknames);
}

void	Channel::broadcast(std::string const &msg, Client *except)
{
	for (std::vector<Client *>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (*it == except)
			continue ;
		(*it)->reply(msg);
	}
}

void	Channel::addClient(Client *client)
{
	this->clients.push_back(client);
	if (this->clients.size() == 1)
		client->setStatus(2);
}

void	Channel::removeClient(Client *client)
{
	std::vector<Client *>::iterator	it;
	std::string						msg;

	// remove the client
	for (it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if ((*it) == client)
		{
			this->clients.erase(it);
			break ;
		}
	}
	client->setChannel(nullp);
	// set status
	if (client->getStatus() == 2)
	{
		client->setStatus(1);
		if (this->clients.size() > 0)
		{
			this->clients.at(0)->setStatus(2);
			msg.append(this->clients.at(0)->getNickname()).append(" is now the admin of ").append(this->name);
			console_log(msg);
		}
	}
}

void	Channel::kick(Client *client, Client *target, std::string const &reason)
{
	std::string	tmp;

	this->broadcast(RPL_KICK(client->getPrefix(), this->name, target->getNickname(), reason));
	this->removeClient(target);

	tmp = client->getNickname() + " kicked " + target->getNickname() + " form channel " + this->name;
	console_log(tmp);
}

void	Channel::invite(Client *client, Client *target)
{
	if (client->getStatus() != 2)
	{
		client->msgReply(ERR_CHANOPRIVSNEEDED(client->getNickname(), this->name));
		return ;
	}
	target->msgReply(client->getNickname() + " invited " + target->getNickname() + " to channel " + this->name);
	target->join(this);
}

int	Channel::isMember(Client *client)
{
	for (std::vector<Client *>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if ((*it) == client)
			return (1);
	}
	return (0);
}

int Channel::getMembers() const
{
	return (this->clients.size());
}

void	Channel::setTopic(std::string const &topic)
{
	this->topic = topic;
}

std::string	Channel::getTopic() const
{
	return (this->topic);
}

std::vector<Client *>	Channel::getClients() const
{
	return (this->clients);
}

std::string	Channel::getMode() const
{
	std::string	mode;

	if (this->limit != -1)
	{
		std::stringstream ss;
		ss << "+l " << this->limit;
		mode.append(ss.str());
	}
	return (mode);
}

int	Channel::isOperator(Client *client)
{
	for (std::vector<Client *>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if ((*it) == client)
			return (1);
	}
	return (0);
}

void	Channel::setOperator(Client *client)
{
	client->setStatus(2);
}

void	Channel::removeOperator(Client *client)
{
	client->setStatus(1);
}

void	Channel::addOperator(Client *client)
{
	client->setStatus(2);
}

void	Channel::setKey(std::string const &key)
{
	this->password = key;
}

void	Channel::setInviteOnly(bool inviteOnly)
{
	this->inviteOnly = inviteOnly;
}

void	Channel::setTopicOnly(bool topicOnly)
{
	this->topicOnly = topicOnly;
}