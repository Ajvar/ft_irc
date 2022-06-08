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
		void	parse(pollfd *fds, int *nfds, user* user)
		{
			switch (_command)
			{
				//****client cmds
				case "PASS":
					if (_args.size() < 1)
					{
						PASS(NULL, NULL, user);
						break;
					}
					PASS(_args[0], /*pswd*/, user);
					break;
				case "NICK":
					if (_args.size() < 1)
					{
						NICK(NULL, user);
						break;
					}
					NICK(_args[0] , user);
					break;
				case "USER":
					if (_args.size() < 4)
					{
						USER(NULL, NULL, user);
						break;
					}
					USER(_args[0], _args[3], user);
					break;
				case "OPER":
					if (_args.size() < 2)
					{
						OPER(NULL, NULL, user);
						break;
					}
					OPER(_args[0], _args[1]);
					break;
				case "MODE":
					if (_args.size() < 3)
					{
						MODE(NULL, NULL, NULL, user);
						break;
					}
					MODE(_args[0], _args[1], _args[2], user);
					break;
				case "QUIT":
					if (_args.size() < 1)
					{
						QUIT(NULL, fds, nfds, user);
						break;
					}
					QUIT(_args[0], fds, nfds, user);
					break;
				case "AWAY":
					if (_args.size() < 1)
					{
						AWAY(NULL, user);
						break;
					}
					AWAY(_args[0], user);
					break;
				//*****channel cmds
				case "DIE":
					DIE(user, fds, nfds);
					break;
				case "RESTART":
					RESTART(user, fds, nfds); //restart in missing?
					break;
				case "WALLOPS":
					if (_args.size() < 1)
					{
						WALLOPS(NULL, user);
						break;
					}
					WALLOPS(_args[0], user);
					break;
				case "ISON":
					if (_args.size() < 1)
					{
						ISON(NULL, user);
						break;
					}
					ISON(splitandsort(_args[0], ",", ))
				case "JOIN":
					std::vector<std::string>	v1, v2;
					if (_args.size() < 1)
					{
						JOIN(NULL, NULL, NULL, user, fds, nfds);
						break;
					}
					if (_args.size() < 2)
					{
						JOIN(splitandsort(_args[0], ",", v1, 0), NULL, NULL, user, fds, nfds);
						break;
					}
					if (_args.size() < 3)
					{
						JOIN(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), NULL, user, fds, nfds);
						break;
					}
					JOIN(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), _args[2], user, fds, nfds);
					break;
				case "PART":
					std::vector<std::string>	v1, v2;
					if (_args.size() < 1)
					{
						PART(NULL, NULL, user);
						break;
					}
					if (_args.size() < 2)
					{
						PART(splitandsort(_args[0], ",", v1, 0), NULL, user);
						break;
					}
					PART(splitandsort(_args[0], ",", v1, 0), _args[1], user);
					break;
				case "TOPIC":
					if (_args.size() < 1)
					{
						TOPIC(NULL, NULL, user);
						break;
					}
					if (_args.size() < 2)
					{
						TOPIC(_args[0], NULL, user);
						break;
					}
					TOPIC(_args[0], _args[1], user);
					break;
				case "NAMES":
					if (_args.size() < 1)
					{
						NAMES(NULL, user);
						break;
					}
					std::vector<std::string>	v1;
					NAMES(splitandsort(_args[0], ",", v1, 0), user);
					break;
				case "LIST":
					std::vector<std::string>	v1, v2;
					if (_args.size() < 1)
					{
						LIST(NULL, NULL, user);
						break;
					}
					if (_args.size() < 2)
					{
						LIST(splitandsort(_args[0], ",", v1, 0), NULL, user);
						break;
					}
					LIST(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), user);
					break;
				case "INVITE":
					if (_args.size() < 1)
					{
						INVITE(NULL, NULL, user);
						break;
					}
					if (_args.size() < 2)
					{
						INVITE(_args[0], NULL, user);
						break;
					}
					INVITE(_args[0], _args[1], user);
					break;
				case "KICK":
					std::vector<std::string>	v1, v2;
					if (_args.size() < 2)
					{
						KICK(NULL, NULL, NULL, user);
						break;
					}
					if (_args.size() < 3)
					{
						KICK(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), NULL, user);
						break;
					}
					KICK(splitandsort(_args[0], ",", v1, 0), splitandsort(_args[1], ",", v2, 0), _args[2], user);
					break;
				//****msg cmd
				case "NOTICE":
					if (_args.size() < 2)
					{
						NOTICE(NULL, NULL, user);
						break;
					}
					NOTICE(_args[0], _args[1], user);
					break;
				case "PRIVMSG":
					if (_args.size() < 2)
					{
						PRIVMSG(NULL, NULL, user);
						break;
					}
					PRIVMSG(_args[0], _args[1], user);
					break;
				default:
					//err unknown command
			}
		}

	private:
		std::string					_command;
		std::vector<std::string>	_args;
};

#endif