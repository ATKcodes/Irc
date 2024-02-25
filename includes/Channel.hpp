#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#pragma once

class Channel;

#include "utils.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Channel
{
	private:
		std::string				name;
		std::string				password;
		std::vector<Client *>	clients;
		std::string				topic;
		bool					inviteOnly; // true = only invited clients can join
		bool					topicOnly; // true = only operators can change topic
		int 					limit; // -1 = no limit

	public:
		Channel(std::string const &name, std::string const &password);
		~Channel();

		std::string					getName() const { return (this->name); }
		std::string					getPassword() const { return (this->password); }

		std::vector<std::string>	getNicknames();
		void						broadcast(std::string const &msg, Client *except = nullp);
		void						addClient(Client *client);
		void						deleteclient(Client *client);
		void						kick(Client *client, Client *target, std::string const &reason);
		void						invite(Client *client, Client *target);
		int							isMember(Client *client); // 0 = not member, 1 = member
		void						setLimit(int limit);
		int							getLimit() const;
		int							getMembers() const;
		void						setTopic(std::string const &topic);
		std::string					getTopic() const;
		std::vector<Client *>		getClients() const;
		std::string					getMode() const;
		int							isOperator(Client *client);
		void						setOperator(Client *client);
		void						removeOperator(Client *client);
		void						addOperator(Client *client);
		void						setKey(std::string const &key);
		void						setInviteOnly(bool inviteOnly);
		bool						isInviteOnly() const;
		void						setTopicOnly(bool topicOnly);
		bool						isTopicOnly() const;
};

#endif
