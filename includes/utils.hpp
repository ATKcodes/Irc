#ifndef UTILS_HPP
#define UTILS_HPP

#pragma once

#include <iostream>
#include <time.h>
#include <signal.h>
#include <vector>
#include <cstring>
#include <map>
#include <cstdio>

#define DEBUG 0
#define MAC_OS 0

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

#define ERRORNICKNOTFOUND(source, nickname)				"401 " + source + " " + nickname + " : No such nick/channel"
#define ERRORCHANNELNOTFOUND(source, channel)			"403 " + source + " " + channel + " : No such channel"
#define ERRORTOOMANYCHANNELS(source, channel)			"405 " + source + " " + channel + " : You have joined too many channels"
#define ERRORCOMMAND(source, command)					"421 " + source + " " + command + " : Unknown command"
#define ERRORBLANKNICK(source)							"431 " + source + " : Nickname not given"
#define ERRORDOUBLENICK(source)							"433 " + source + " " + source  + " : Nickname is already in use"
#define ERRORKICKNOTFOUND(source, nickname, channel)	"441 " + source + " " + nickname + " " + channel + " : They aren't on that channel"
#define ERRORWRONGCHANNEL(source, channel)				"442 " + source + " " + channel + " : You're not on that channel"
#define ERRORUSERALREADYIN(source, nickname, channel)	"443 " + source + " " + nickname + " " + channel + " : is already on channel"
#define ERRORNOTREGISTERED(source)						"451 " + source + " : You have not registered"
#define ERRORPARAMS(source, command)					"461 " + source + " " + command + " : Not enough parameters"
#define ERRORDOUBLEREGISTER(source)						"462 " + source + " : You may not reregister"
#define ERRORWRONGPASS(source)							"464 " + source + " : Password incorrect"
#define EERRORFULLCHANNEL(source, channel)				"471 " + source + " " + channel + " : Cannot join channel (+l)"
#define ERRORWRONGMODE(source, mode)					"472 " + source + " " + mode + " : is unknown mode char to me"
#define ERRORINVITEONLY(source, channel)				"473 " + source + " " + channel + " : Cannot join channel (+i)"
#define ERRORWRONGPASSCHANNEL(source, channel)			"475 " + source + " " + channel + " : Cannot join channel (+k)"
#define ERRORNOTADMIN(source, channel)					"482 " + source + " " + channel + " : You're not channel operator"
#define REPLYMEET(source)								"001 " + source + " : Welcome " + source + " to the ft_irc network"
#define REPLYINVITING(source, nickname, channel)		"341 " + source + " " + nickname + " " + channel + " : Inviting"
#define REPLYNAMEDMESSAGE(source, channel, users)		"353 " + source + " = " + channel + " : " + users
#define REPLYLASTNAME(source, channel)					"366 " + source + " " + channel + " : End of /NAMES list."
#define REPLYCMDJOIN(source, channel)					":" + source + " JOIN: " + channel
#define REPLYCMDPRIVMSG(source, target, message)		":" + source + " PRIVMSG: " + target + " : " + message
#define REPLYCMDKICK(source, channel, target, reason)	":" + source + " KICK: " + channel + " " + target + " : " + reason
#define REPLYCMDPART(source, channel)					":" + source + " PART: " + channel

void print_time(const std::string& msg);

#endif
