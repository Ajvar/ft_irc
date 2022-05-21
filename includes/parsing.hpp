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
// - join
// - kick(op)
// - list
// - help
// - error
// - notice (->?)
// - part
// - pv
// - quit
// - who

class Command
{
	public:
		Command() : _command(NULL), _trail(NULL);
		Command(const char* cmd);
		~Command();

		bool	checkCommand();
		bool	checkTrail();
		bool	checkArgs();
		void	parse(user* user);

	private:
		char*			_command;
		char*			_trail;
		std::deque<char*>	_args;
};

#endif