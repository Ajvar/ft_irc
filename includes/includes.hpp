/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 16:17:45 by jcueille          #+#    #+#             */
/*   Updated: 2022/06/30 18:52:14 by jcueille         ###   ########.fr       */
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
#define SERVER_NAME "42IRC"
#define VERSION "1.0.0"

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
	BAN_MODE,
    MODERATED_MODE,
	OUTSIDE_PRIVMSG_MODE,
	SECRET_MODE,
    TOPIC_LOCKED_MODE,
    KEY_LOCKED_MODE,
    USER_LIMIT_MODE,
	NO_EXTERN_MSG_MODE
};

//typedef struct s_channel channel;
typedef struct s_user user;

typedef struct s_channel {
	std::string						name; //includes #, &
	std::string 					key;
	std::string						topic;
	time_t							creation;
	int								user_limit;
	unsigned						modes[9];
	user							*creator;
	std::vector<std::string>		banned;
	std::vector<std::string>		invites;
	std::vector<struct s_user *>	operators;
	std::vector<struct s_user *>	voice;
	std::vector<struct s_user *>	users;
	
	struct s_channel				*next;
	struct s_channel				*prev;
} channel;



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
int PASS(const std::string &server_password, const std::string &user_password, user *user);
int NICK(const std::string &nickname, user *user);
int	USER(const std::string &username, const std::string &realname, user *user);
int OPER(const std::string &username, const std::string &password, user *user);
int QUIT(const std::string &msg, pollfd *fds, int *nfds, user *u);

//mode
int MODE(const std::string &target, const char &sign, const char &mode, user *user);

//optionnal commands
int AWAY(const std::string &away_msg, user *user);
int	DIE(user *user, pollfd *fds, int nfds);
int RESTART(user *user, pollfd *fds, int nfds, int *restart);
int WALLOPS(const std::string &msg, user *u);
int ISON(std::vector<std::string> nicknames, user *user);

//channel commands
int JOIN(std::vector<std::string> chan, std::vector<std::string> keys, const std::string &option, user *u, pollfd *fds, int nfds);
int PART(std::vector<std::string> channels, const std::string &reason, user *u);
int TOPIC(const std::string &topic, const std::string &chan, user *u);
int NAMES(std::vector<std::string>chan, user *u);
int LIST(const std::vector<std::string> c, user *u);
int INVITE(const std::string &us, const std::string &c, user *u);
int KICK(const std::vector<std::string> &c, std::vector<std::string> us, const std::string &comment ,user *u);
int WHO(const std::string &mask, user *u);

//messages
int PRIVMSG(std::vector<std::string> targets, const std::string &text, user *u);
int NOTICE(std::vector<std::string> targets, const std::string &text, user *u);

//utils
user 		*find_user_by_fd(int fd);
user 		*find_user_by_nickname(const std::string &nickname);
user 		*find_u_in_chan(const std::string &nickname, const channel *c);
channel		*find_chan_in_u(const std::string &name, user *u);
channel		*find_channel_by_name(const std::string &name);
void		delete_chan_in_u(const std::string &name, user *u);
void		delete_u_in_chan(const std::string &nickname, channel *c);
int 		send_message(const std::string &s, user *user, int ret);
void 		compress_array(pollfd *fds, int *nfds);
std::string create_msg(int code, user *u, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4);
std::string	ft_to_string(int value);
int			is_chan_ope(const channel *c, const user *u);
int 		is_chan_voice(const channel* c, const user *u);
int 		is_banned(const channel *c, const user *u);
void		send_to_all_serv(const std::string &s);
std::string channel_message(const std::string &msg, user *u);

//exit
void ft_exit(std::string s, int err, int *sock);
void ft_free_exit(std::string s, int err, pollfd *fds, int nfds);
void free_channels(void);
void free_users(void);
void free_fds(pollfd *fds, int nfds);

//debug
void print_channels();
void print_str_vec(std::vector<std::string> v, std::string info);
void printer(std::string s);
void pp(std::string s);
void print_user(user *u);
#endif