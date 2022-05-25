/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 16:17:45 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/25 12:15:39 by jcueille         ###   ########.fr       */
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

//modes
enum e_mode {
	AWAY_MODE,
	INVISIBLE_MODE,
	WALLOPS_MODE,
	RESTRICTED_MODE,
	OPERATOR_MODE
};

struct s_channel {
	std::string						name;
	std::string 					password;
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
	std::string							away_msg;
	std::vector<struct s_channel *> 	channels;
	struct pollfd						*fd;
	struct s_user						*next;
	struct s_user						*prev;

};

typedef struct s_user user;
typedef struct s_channel channel;

//commands
int PASS(const std::string server_password, const std::string user_password, user *user);
int NICK(const std::string nickname, user *user);
int	USER(std::string username, char* realname, user *user);
int OPER(std::string username, std::string password, user *user);
int MODE(std::string nickname, char sign, char mode, user *user);
int QUIT(pollfd *fds, int *nfds, user *u);

int AWAY(std::string away_msg, user *user);
int	DIE(user *user);
int RESTART(user *user, pollfd *fds, int nfds, int *restart);
int WALLOPS(std::string msg, user *u);
int ISON(std::vector<std::string> nicknames, user *user);


//int JOIN(std::stringchannel ,user *user);


//utils
user *find_user_by_id(int fd);
user *find_user_by_nickname(std::string nickname);
int send_message(std::string s, user *user, int ret);
void compress_array(pollfd *fds, int *nfds);

//exit
void ft_exit(std::string s, int err, int *sock);
void ft_free_exit(std::string s, int err, pollfd *fds, int nfds);
void free_channels(void);
void free_users(void);
void free_fds(pollfd *fds, int nfds);

//debug
void print_user(user *user);

#endif