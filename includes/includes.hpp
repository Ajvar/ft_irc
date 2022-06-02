/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 16:17:45 by jcueille          #+#    #+#             */
/*   Updated: 2022/06/02 18:54:13 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
# define INCLUDES_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <poll.h>

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

#define FALSE 0
#define TRUE 1
# define PROTOCOL_VERSION "0210"
#define VERSION "42IRC"

//modes
enum e_user_mode {
	AWAY_MODE,
	INVISIBLE_MODE,
	WALLOPS_MODE,
	RESTRICTED_MODE,
	OPERATOR_MODE
};

enum e_chan_mode {
	INVITE_ONLY_MODE,
    MODERATED_MODE,
	OUTSIDE_PRIVMSG_MODE,
	PRIVATE_MODE,
    TOPIC_LOCKED_MODE,
    KEY_LOCKED_MODE,
    USER_LIMIT_MODE	
};

typedef struct s_channel channel;
typedef struct s_user user;

struct s_channel {
	std::string						name;
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


//create / delete
int		new_client(int id, struct pollfd *fd);
void	delete_client(int id);
channel	*new_channel(std::string name);
void	delete_channel(std::string name);

//client commands
int PASS(const std::string server_password, const std::string user_password, user *user);
int NICK(const std::string nickname, user *user);
int	USER(std::string username, std::string realname, user *user);
int OPER(std::string username, std::string password, user *user);
int MODE(std::string nickname, char sign, char mode, user *user);
int QUIT(std::string msg, pollfd *fds, int *nfds, user *u);

//optionnal commands
int AWAY(std::string away_msg, user *user);
int	DIE(user *user, pollfd *fds, int nfds);
int RESTART(user *user, pollfd *fds, int nfds, int *restart);
int WALLOPS(std::string msg, user *u);
int ISON(std::vector<std::string> nicknames, user *user);

//channel commands
int JOIN(std::vector<std::string> chan, std::vector<std::string> keys, int option, user *u, pollfd *fds, int nfds);

//utils
user *find_user_by_fd(int fd);
user *find_user_by_nickname(std::string nickname);
user *find_u_in_chan(std::string nickname, channel *c);
channel *find_channel_by_name(std::string name);
int send_message(std::string s, user *user, int ret);
std::string		ft_to_string(int value);
void compress_array(pollfd *fds, int *nfds);
std::string create_msg(int code, user *u, std::string arg1, std::string arg2, std::string arg3, std::string arg4);

//exit
void ft_exit(std::string s, int err, int *sock);
void ft_free_exit(std::string s, int err, pollfd *fds, int nfds);
void free_channels(void);
void free_users(void);
void free_fds(pollfd *fds, int nfds);

//debug
void print_user(user *user);

#endif