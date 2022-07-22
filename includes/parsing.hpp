#ifndef __PARSING__
# define __PARSING__

# include "../includes/includes.hpp"
# include "../includes/replies.hpp"
# include <deque>
# include <algorithm>

//command format
//:<Prefix> <Command> <Space Separated Args> :Optional trailing content here, like a chat message.

//commands tow do:
// - pass: done
// - nick: done
// - user: done
// - part
// - join
// - kick(op)
// - list
// - help
// - error
// - notice (->?)
// - pv
// - quit
// - who

# define GEN_USE	1

class Command
{
	public:
		Command() : _command(NULL){}
		Command(const std::string cmd)
		{
			splitandsort(cmd, " ", _args, GEN_USE);
			std::cout << "command " << _command << std::endl;
			print_str_vec(_args, "args: ");
		}
		~Command(){}
		std::vector<std::string>	splitandsort(std::string cmd, std::string delim, std::vector<std::string>	&cont, int opt)
		{
			std::size_t	pos;
  			cmd.erase(std::remove(cmd.begin(), cmd.end(), 13), cmd.end()); 
			while ((pos = cmd.find(delim)) != std::string::npos) //ca inclut le dernier maillon?
			{

				std::string token = cmd.substr(0, pos);
				if (_command == "" && opt)
					_command = token;
				else
					cont.push_back(token);
				cmd.erase(0, pos + 1);
			}
			if (cmd[0] == ':' && opt)
			{
				cmd.erase(0, 1);
				cont.push_back(cmd);
				return (cont);
			}
			cont.push_back(cmd);
			return (cont);
		}
		/*bool	checkCommand()
		{
			//check length
			//check syntax
		}
		bool	checkArgs()
		{
			//while
			// check length
			// check syntax
		}*/
		int	parse(pollfd *fds, int *nfds, user* u, const std::string &serv_pass, int *restart)
		{
			// pp("entering parser: " + this->_command, CYAN);
			//****client cmds
			if (_command == "PASS")
			{
				if (_args.size() < 1)
				{
					send_message(create_msg(ERR_NEEDMOREPARAMS, u, "PASS", "", "", ""), u, ERR_NEEDMOREPARAMS);
					return -1;
				}
				if (PASS(serv_pass, _args[0], u))
					return -1;
					
			}
			else if (_command == "NICK")
			{	if (_args.size() < 1)
					NICK("", u);
				else
					NICK(_args[0] , u);
			}
			else if (_command == "USER")
			{
				if (_args.size() < 4)
					USER("", "", u);
				else
					USER(_args[0], _args[3], u);
			}
			else if (_command == "OPER")
			{
			if (_args.size() < 2)
				OPER("", "", u);
			else
				OPER(_args[0], _args[1], u);
			}

			else if (_command == "MODE")
			{
				//pp("arg0" + _args[0] + "arg1" + _args[1], CYAN);
				if (_args.size() < 1)
					MODE("", "", u);
				else if (_args.size() == 1)
					MODE(_args[0], "", u);
				else 
					MODE(_args[0], _args[1], u);

			}
			else if (_command == "QUIT")
			{
				if (_args.size() < 1)
					QUIT(NULL, fds, nfds, u);
				else
					QUIT(_args[0], fds, nfds, u);
			}
			else if (_command == "AWAY")
			{
				if (_args.size() < 1)
					AWAY(NULL, u);
				else
					AWAY(_args[0], u);
			}
			//*****channel cmds
			else if (_command == "DIE")
				DIE(u, fds, *nfds);

			else if (_command == "RESTART")
				RESTART(u, fds, *nfds, restart);

			else if (_command == "WALLOPS")
				if (_args.size() < 1)
					WALLOPS(NULL, u);
				else
					WALLOPS(_args[0], u);

			else if (_command == "ISON")
			{
				std::vector<std::string>	v1;
				if (_args.size() < 1)
					ISON(std::vector<std::string>(), u);
				else
					ISON(splitandsort(_args[0], ",", v1, 0 ), u);
			}
			else if (_command == "JOIN")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 1)
					JOIN(std::vector<std::string>(), std::vector<std::string>(), "", u, fds, *nfds);
				else if (_args.size() < 2)
				{
					JOIN(splitandsort(_args[0], ",", v1, 0), std::vector<std::string>(), "", u, fds, *nfds);
				}
				else if (_args.size() < 3)
					JOIN(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), "", u, fds, *nfds);
				else
					JOIN(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), _args[2], u, fds, *nfds);
			}
			else if (_command == "PART")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 1)
					PART(std::vector<std::string>(), "", u);
				else if (_args.size() < 2)
					PART(splitandsort(_args[0], ",", v1, 0), NULL, u);
				else
					PART(splitandsort(_args[0], ",", v1, 0), _args[1], u);
			}

			else if (_command == "TOPIC")
				if (_args.size() < 1)
					TOPIC("", "", u);
				else if (_args.size() < 2)
					TOPIC(_args[0], NULL, u);
				else
					TOPIC(_args[0], _args[1], u);

			else if (_command == "NAMES")
				if (_args.size() < 1)
					NAMES(std::vector<std::string>(), u);
				else
				{
					std::vector<std::string>	v1;
					NAMES(splitandsort(_args[0], ",", v1, 0), u);
				}

			else if (_command == "LIST")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 1)
					LIST(std::vector<std::string>(), u);
				else if (_args.size() < 2)
					LIST(splitandsort(_args[0], ",", v1, 0), u);
			}
			else if (_command == "INVITE")
			{
				if (_args.size() < 1)
					INVITE(NULL, NULL, u);
				else if (_args.size() < 2)
					INVITE(_args[0], NULL, u);
				else
					INVITE(_args[0], _args[1], u);
			}

			else if (_command == "KICK")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 2)
					KICK(std::vector<std::string>(), std::vector<std::string>(), "", u);
	
				else if (_args.size() < 3)
					KICK(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), NULL, u);
				else
				KICK(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), _args[2], u);
			}
			else if (_command == "WHO")
			{
				if (_args.size() < 1)
					WHO("", u);
				else
					WHO(_args[0], u);
			}
/*
			****msg cmd
			else if (_command == "NOTICE")
			{	if (_args.size() < 2)
					NOTICE(std::vector<std::string>(), NULL, user);
				else
				NOTICE(_args[0], _args[1], user);
			}
			else if (_command == "PRIVMSG")
			{
				if (_args.size() < 2)
					PRIVMSG(std::vector<std::string>(), "", user);
				else
					PRIVMSG(_args[0], _args[1], user);
			}
			else
				send_message(create_msg(ERR_UNKNOWNCOMMAND, user, _command, "", "", ""), user, ERR_UNKNOWNCOMMAND);
		*/
		return 0;
	}

	private:
		std::string					_command;
		std::vector<std::string>	_args;
};

#endif