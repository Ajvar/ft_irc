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
		Command() : _command(NULL), _trail(NULL);
		Command(const std::string cmd)
		{
			splitandsort(cmd, " ", _args, GEN_USE);
			if (!checkCommand())
				;//senderror?
			if (!checkArgs())
				;
		}
		~Command();

		std::vector<std::string>	splitandsort(std::string cmd, std::string delim, std::vector<std::string>	cont, int opt)
		{
			while ((pos = cmd.find(delim)) != std::string::npos) //ca inclut le dernier maillon?
			{
				std::string token = cmd.substr(0, pos);
				if (!_command && opt)
					_command = token;
				else
					cont.append(token);
				cmd.erase(0, pos + 1);
			}
			if (cmd[0] == ":" && opt)
			{
				cmd.erase(0, 1);
				cont.append(cmd);
				return (cont);
			}
			cont.append(cmd);
			return (cont);
		}
		bool	checkCommand()
		{
			//check length
			//check syntax
		}
		bool	checkArgs()
		{
			//while
			// check length
			// check syntax
		}
		void	parse(pollfd *fds, int *nfds, user* user, std::string serv_pass, int *restart)
		{
			//****client cmds
			if (_command == "PASS")
			{		
				if (_args.size() < 1)
					PASS(NULL, NULL, user);
				else
					PASS(serv_pass, _args[0], user);
			}
			else if (_command == "NICK")
			{	if (_args.size() < 1)
					NICK(NULL, user);
				else
					NICK(_args[0] , user);
			}
			else if (_command == "USER")
			{
				if (_args.size() < 4)
					USER(NULL, NULL, user);
				else
				USER(_args[0], _args[3], user);
			}
			else if (_command == "OPER")
			{
			if (_args.size() < 2)
				OPER(NULL, NULL, user);
			else
				OPER(_args[0], _args[1]);
			}

			else if (_command == "MODE")
			{
				if (_args.size() < 3)
					MODE(NULL, NULL, NULL, user);
				else
					MODE(_args[0], _args[1], _args[2], user);
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
				DIE(user, fds, nfds);

			else if (_command == "RESTART")
				RESTART(user, fds, nfds, restart);

			else if (_command == "WALLOPS")
				if (_args.size() < 1)
					WALLOPS(NULL, user);
				else
					WALLOPS(_args[0], user);

			else if (_command == "ISON")
			{
				if (_args.size() < 1)
					ISON(NULL, user);
				else
					ISON(splitandsort(_args[0], ",", ))
			}
			else if (_command == "JOIN")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 1)
					JOIN(NULL, NULL, NULL, user, fds, nfds);
				else if (_args.size() < 2)
					JOIN(splitandsort(_args[0], ",", v1, 0), NULL, NULL, user, fds, nfds);
				else if (_args.size() < 3)
					JOIN(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), NULL, user, fds, nfds);
				else
					JOIN(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), _args[2], user, fds, nfds);
			}
			else if (_command == "PART")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 1)
					PART(NULL, NULL, user);
				else if (_args.size() < 2)
					PART(splitandsort(_args[0], ",", v1, 0), NULL, user);
				else
					PART(splitandsort(_args[0], ",", v1, 0), _args[1], user);
			}

			else if (_command == "TOPIC")
				if (_args.size() < 1)
					TOPIC(NULL, NULL, user);
				else if (_args.size() < 2)
					TOPIC(_args[0], NULL, user);
				else
					TOPIC(_args[0], _args[1], user);

			else if (_command == "NAMES")
				if (_args.size() < 1)
					NAMES(NULL, user);
				else
				{
					std::vector<std::string>	v1;
					NAMES(splitandsort(_args[0], ",", v1, 0), user);
				}

			else if (_command == "LIST")
			{
				std::vector<std::string>	v1, v2;
				if (_args.size() < 1)
					LIST(NULL, NULL, user);
				else if (_args.size() < 2)
					LIST(splitandsort(_args[0], ",", v1, 0), NULL, user);
			else
				LIST(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), user);
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
					KICK(NULL, NULL, NULL, user);
	
				else if (_args.size() < 3)
					KICK(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), NULL, user);
				else
				KICK(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), _args[2], user);
			}

			//****msg cmd
			else if (_command == "NOTICE")
			{	if (_args.size() < 2)
					NOTICE(NULL, NULL, user);
				else
				NOTICE(_args[0], _args[1], user);
			}
			else if (_command == "PRIVMSG")
			{
				if (_args.size() < 2)
					PRIVMSG(NULL, NULL, user);
				else
					PRIVMSG(_args[0], _args[1], user);
			}
			else
				send_message(create_msg(ERR_UNKNOWNCOMMAND, user, _command, "", "", ""), user, ERR_UNKNOWNCOMMAND);
		}

	private:
		std::string					_command;
		std::vector<std::string>	_args;
};

#endif