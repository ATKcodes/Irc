#ifndef UTILS_HPP
#define UTILS_HPP

#pragma once

#include <iostream>
#include <time.h>
#include <signal.h>
#include <cstring>
#include <cstdio>
#include <vector>
#include <map>

// error replies
#define ERR_UNKNOWNCOMMAND(source, command)				"421 " + source + " " + command + " :Unknown command"
#define ERR_NEEDMOREPARAMS(source, command)				"461 " + source + " " + command + " :Not enough parameters"
#define ERR_NOTREGISTERED(source)						"451 " + source + " :You have not registered"
#define ERR_ALREADYREGISTERED(source)					"462 " + source + " :You may not reregister"
#define ERR_PASSWDMISMATCH(source)						"464 " + source + " :Password incorrect"
#define ERR_NONICKNAMEGIVEN(source)						"431 " + source + " :Nickname not given"
#define ERR_NICKNAMEINUSE(source)						"433 " + source + " " + source  + " :Nickname is already in use"
#define ERR_TOOMANYCHANNELS(source, channel)			"405 " + source + " " + channel + " :You have joined too many channels"
#define ERR_NOTONCHANNEL(source, channel)				"442 " + source + " " + channel + " :You're not on that channel"
#define ERR_NOSUCHCHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel"
#define ERR_BADCHANNELKEY(source, channel)				"475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_NOSUCHNICK(source, nickname)				"401 " + source + " " + nickname + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(source, nickname, channel)	"441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_CHANOPRIVSNEEDED(source, channel)			"482 " + source + " " + channel + " :You're not channel operator"
#define ERR_CANNOTSENDTOCHAN(source, channel)			"404 " + source + " " + channel + " :Cannot send to channel"
#define ERR_INVITEONLYCHAN(source, channel)				"473 " + source + " " + channel + " :Cannot join channel (+i)"
#define ERR_CHANNELISFULL(source, channel)				"471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_USERONCHANNEL(source, nickname, channel)	"443 " + source + " " + nickname + " " + channel + " :is already on channel"
#define ERR_UNKNOWNMODE(source, mode)					"472 " + source + " " + mode + " :is unknown mode char to me"
#define ERR_NOTADMIN(source)							"481 " + source + " :Permission Denied- You're not an IRC operator"

// numeric replies
#define RPL_WELCOME(source)							"001 " + source + " :Welcome " + source + " to the ft_irc network"
#define RPL_NAMREPLY(source, channel, users)		"353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)				"366 " + source + " " + channel + " :End of /NAMES list."
#define RPL_INVITING(source, nickname, channel)		"341 " + source + " " + nickname + " " + channel + " :Inviting"
#define RPL_NOTOPIC(source, channel)				"331 " + source + " " + channel + " :No topic is set"

// command replies
#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)					":" + source + " PART :" + channel
#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message
#define RPL_QUIT(source, message)					":" + source + " QUIT :Quit: " + message
#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason


#define MAC_OS 0
#define DEBUG 0


void print_time(const std::string& message);

const class nullptr_t
{
	public:
		template<class T>
		operator T*() const { return (0); }

		template<class C, class T>
			operator T C::*() const { return (0); }	

	private:
		void operator&() const;

} nullp = {};


#endif
