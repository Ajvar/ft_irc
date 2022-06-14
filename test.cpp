#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <poll.h>
#include <map>

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cerrno>
#include <string.h>
#include <sys/ioctl.h>
#include <cstddef>


typedef struct s_channel channel;
typedef struct s_user user;

struct s_channel {
	std::string						name; //includes #, &, !
	std::string 					key;
	std::string						topic;
	time_t							creation;
	int								user_limit;
	unsigned						modes[7];
	user							*creator;
	std::vector<std::string>		banned;
	std::vector<std::string>		invites;
	std::vector<struct s_user *>	operators;
	std::vector<struct s_user *>	users;
	struct s_channel				*next;
	struct s_channel				*prev;
};

struct s_user {
	int									id;
	unsigned							modes[5];
	std::string							nickname;
	std::string							username;
	std::string							realname;
	std::string							hostname;
	std::string							away_msg;
	std::vector<struct s_channel *> 	channels;
	struct pollfd						*fd;
	struct s_user						*next;
	struct s_user						*prev;

};

user *users = NULL;
channel *channels = NULL;

int new_client(int id, struct pollfd *fd)
{
	struct s_user *tmp = users;
	struct s_user* new_user = new struct s_user();
	if (new_user == NULL)
		return -1;
	new_user->next = NULL;
	new_user->id = id;
	if (!users)
		users = new_user;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_user;
		new_user->prev = tmp;
	}
	new_user->fd = fd;
	new_user->nickname = "";
	new_user->realname = "";
	memset(new_user->modes, 0, sizeof(new_user->modes));
	new_user->hostname = "42irc.com";
	return 0;
	
}

void delete_client(int id)
{
	user * tmp = users;
	while (tmp != NULL)
	{
		if (tmp->id == id)
		{
			tmp->next->prev = tmp->prev;
			tmp->prev->next = tmp->next;
			std::vector<channel *>().swap(tmp->channels);
			delete tmp;
			return ;
		}
		tmp = tmp->next;
	}

}

void delete_channel(std::string name)
{
	channel * tmp = channels;
	while (tmp != NULL)
	{
		if (tmp->name == name)
		{
			tmp->next->prev = tmp->prev;
			tmp->prev->next = tmp->next;
			std::vector<user *>().swap(tmp->users);
			delete tmp;
			return ;
		}
		tmp = tmp->next;
	}

}

channel *new_channel(std::string name)
{
	channel *tmp = channels;
	channel* new_channel = new channel();
	if (new_channel == NULL)
		return NULL;
	new_channel->next = NULL;
	new_channel->name = name;
	if (!channels)
		channels = new_channel;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_channel;
		new_channel->prev = tmp;
	}
	new_channel->key = "";
	new_channel->topic = "No topic"; 
	memset(new_channel->modes, 0, sizeof(new_channel->modes));
	time(&new_channel->creation);
	return new_channel;
	
}

int main()
{
	pollfd *fds;
	fds[0] = 4;
	new_client(1, &fds[0]);
	std::map<user *, int[5]> m;
	
	
	return 0;
}

/*
USER MODES :
- a : away
- i : invisible
- w : wallops
- r : restricted
- o : operator

CHANNEL MODES :
for users :
    - O : channel creator
    - o : channel operator
    - v : voice
for channels :
    - i : invite only
    - m : moderated
    - n : outside privmsg disabled
    - p : private
    - t : topic locked
    - k : key locked
    - l : user limit


CLIENT COMMANDS:

PASS OK 
NICK OK 
USER OK 
OPER OK 
MODE OK
QUIT OK 

CHANNEL COMMANDS:
JOIN OK 
PART
MODE
TOPIC
NAMES 
LIST 
INVITE 
KICK 

SENDING MESSAGES:
PRIVMSG 
NOTICE
*/