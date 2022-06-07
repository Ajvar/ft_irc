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

class Command
{
	public:
		Command() : _command(NULL), _trail(NULL);
		Command(const std::string cmd)
		{
			splitandsort(cmd);
			if (!checkCommand())
				;//senderror?
			if (!checkArgs())
				;
		}
		~Command();

		bool	splitandsort(std::string cmd)
		{
			while ((pos = cmd.find(" ")) != std::string::npos)
			{
				if (cmp[0] != ":")
					std::string tmp = cmd.substr(0, pos);
				else if (_command)
				{
					tmp = cmd;
					tmp.erase(0, 1);
					_args.append(tmp);
					break;
				}
				if (!_command)
					_command = tmp;
				else
					_args.append(tmp);
				cmd.erase(0, pos + 1);
			}
			return (true);
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
		void	parse(user* user)
		{
			switch (_command)
			{
				//****client cmds
				case "PASS":
					if (_args.size() < 1)
						PASS(NULL, NULL, user);
					PASS(_args[0], /*pswd*/, user);
					break;
				case "NICK":
					//check nb of args
					NICK(_args[0] , user);
					break;
				case "USER":
					//check args
					USER();
					break;
				case "OPER":
					//args
					OPER();
					break;
				case "MODE":
					//args
					MODE();
					break;
				case "QUIT":
					//args
					QUIT();
					break;
				case "AWAY":
				//*****channel cmds
				case "DIE":
				case "RESTART":
				case "WALLOPS":
				case "ISON":
				case "JOIN":
					//check args: chan & pass
					JOIN();
					break;
				case "PART":
					//check args
					PART();
					break;
				case "TOPIC":
				case "NAME":
				case "LIST":
				case "INVITE":
				case "KICK":
				//****msg cmd
				case "NOTICE":
				case "PRIVMSG":

			}
			//case command sort
			// check nb of args (case sensitive)
			// send to fct
		}

	private:
		std::string					_command;
		std::vector<std::string>	_args;
};

#endif