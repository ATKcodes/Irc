#include "Command.hpp"

PassCommand::PassCommand(Server *server, int auth) : Command(server, auth) {}

PassCommand::~PassCommand() {}

void	PassCommand::exec(Client *client, std::vector<std::string> args)
{
	if (client->getStatus() != 0)
	{
		client->reply(ERRORDOUBLEREGISTER(client->getNickname()));
		return ;
	}
	if (args.empty())
	{
		client->reply(ERRORPARAMS(client->getNickname(), "PASS"));
		return ;
	}
	if (args.at(0).compare(this->server->getPassword()))
	{
		client->reply(ERRORWRONGPASS(client->getNickname()));
		return ;
	}
	client->setStatus(1);
}
