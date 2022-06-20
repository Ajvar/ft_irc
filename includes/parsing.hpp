#ifndef __PARSING__
# define __PARSING__

# include "../includes/includes.hpp"
# include "../includes/replies.hpp"
# include <deque>

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
			std::cout << "command" << _command << std::endl;
			print_str_vec(_args, "args: ");
		}
		~Command(){}
		std::vector<std::string>	splitandsort(std::string cmd, std::string delim, std::vector<std::string>	cont, int opt)
		{
			std::size_t	pos;
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
			for (std::vector<std::string>::iterator it = cont.begin(); it != cont.end(); it++)
				std::cout << "vecz " << (*it) << std::endl;
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
		void	parse(pollfd *fds, int *nfds, user* user, const std::string &serv_pass, int *restart)
		{
			//****client cmds
			if (_command == "PASS")
			{		
				if (_args.size() < 1)
				{
					std::cout << "here " << std::endl;
					PASS("", "", user);
				
				}
				else
					PASS(serv_pass, _args[0], user);
			}
			else if (_command == "NICK")
			{	if (_args.size() < 1)
					NICK("", user);
				else
					NICK(_args[0] , user);
			}
			else if (_command == "USER")
			{
				if (_args.size() < 4)
					USER("", "", user);
				else
				USER(_args[0], _args[3], user);
			}
			else if (_command == "OPER")
			{
			if (_args.size() < 2)
				OPER("", "", user);
			else
				OPER(_args[0], _args[1], user);
			}

			else if (_command == "MODE")
			{
				if (_args.size() < 3)
					MODE("", 0, 0, user);
				else
					MODE(_args[0], _args[1][0], _args[2][0], user);
			}
			else if (_command == "QUIT")
			{
				if (_args.size() < 1)
					QUIT(NULL, fds, nfds, user);
				else
					QUIT(_args[0], fds, nfds, user);
			}
			else if (_command == "AWAY")
			{
				if (_args.size() < 1)
					AWAY(NULL, user);
				else
					AWAY(_args[0], user);
			}
			//*****channel cmds
			else if (_command == "DIE")
				DIE(user, fds, *nfds);

			else if (_command == "RESTART")
				RESTART(user, fds, *nfds, restart);

			else if (_command == "WALLOPS")
				if (_args.size() < 1)
					WALLOPS(NULL, user);
				else
					WALLOPS(_args[0], user);

			else if (_command == "ISON")
			{
				std::vector<std::string>	v1;
				if (_args.size() < 1)
					ISON(std::vector<std::string>(), user);
				else
					ISON(splitandsort(_args[0], ",", v1, 0 ), user);
			}
			else if (_command == "JOIN")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 1)
					JOIN(std::vector<std::string>(), std::vector<std::string>(), "", user, fds, *nfds);
				else if (_args.size() < 2)
					JOIN(splitandsort(_args[0], ",", v1, 0), std::vector<std::string>(), NULL, user, fds, *nfds);
				else if (_args.size() < 3)
					JOIN(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), NULL, user, fds, *nfds);
				else
					JOIN(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), _args[2], user, fds, *nfds);
			}
			else if (_command == "PART")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 1)
					PART(std::vector<std::string>(), "", user);
				else if (_args.size() < 2)
					PART(splitandsort(_args[0], ",", v1, 0), NULL, user);
				else
					PART(splitandsort(_args[0], ",", v1, 0), _args[1], user);
			}

			else if (_command == "TOPIC")
				if (_args.size() < 1)
					TOPIC("", "", user);
				else if (_args.size() < 2)
					TOPIC(_args[0], NULL, user);
				else
					TOPIC(_args[0], _args[1], user);

			else if (_command == "NAMES")
				if (_args.size() < 1)
					NAMES(std::vector<std::string>(), user);
				else
				{
					std::vector<std::string>	v1;
					NAMES(splitandsort(_args[0], ",", v1, 0), user);
				}

			else if (_command == "LIST")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 1)
					LIST(std::vector<std::string>(), user);
				else if (_args.size() < 2)
					LIST(splitandsort(_args[0], ",", v1, 0), user);
			}
			else if (_command == "INVITE")
			{
				if (_args.size() < 1)
					INVITE(NULL, NULL, user);
				else if (_args.size() < 2)
					INVITE(_args[0], NULL, user);
				else
					INVITE(_args[0], _args[1], user);
			}

			else if (_command == "KICK")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 2)
					KICK(std::vector<std::string>(), std::vector<std::string>(), "", user);
	
				else if (_args.size() < 3)
					KICK(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), NULL, user);
				else
				KICK(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), _args[2], user);
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
		*/}

	private:
		std::string					_command;
		std::vector<std::string>	_args;
};

#endif