#include "utils.hpp"
#include <iostream>

void	print_time(std::string const &msg)
{
	time_t		raw;
	std::string      t;
	struct tm	*time_info;
	char		buf[100];

	time(&raw);
	time_info = localtime(&raw);
	strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", time_info);
	t = buf;
	std::cout << "[" << t << "] " << msg << "\n";
}