#include "Command.hpp"

PrivMsgCommand::PrivMsgCommand(Server *server, int auth) : Command(server, auth) {}

PrivMsgCommand::~PrivMsgCommand() {}

void	PrivMsgCommand::exec(Client *client, std::vector<std::string> args)
{
	if (args.size() < 2 || args.at(0).empty() || args.at(1).empty())
	{
		client->send_msg(ERRORPARAMS(client->getNickname(), "PRIVMSG"));
		return;
	}

	std::string	msg;

	for (std::vector<std::string>::iterator it = args.begin() + 1; it != args.end(); ++it)
	{
		msg.append(*it + " ");
	}

	if (msg.at(0) == ':')
		msg = msg.substr(1);

	if (args.at(0).at(0) == '#')
	{

		Channel	*channel = client->getChannel();
		if (channel == nullp)
			return;

		channel->send_all(REPLYCMDPRIVMSG(client->getPrefix(), args.at(0), msg), client);
		return;
	}

	Client	*dst = this->server->getClient(args.at(0));
	if (dst == nullp)
	{
		client->send_msg(ERRORNICKNOTFOUND(client->getNickname(), args.at(0)));
		return;
	}

	dst->reply(REPLYCMDPRIVMSG(client->getPrefix(), args.at(0), msg));
}
