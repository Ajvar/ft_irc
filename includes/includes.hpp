/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 16:17:45 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/21 23:28:14 by jcueille         ###   ########.fr       */
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
int PASS(const char *server_password, const char *user_password, user *user);
int NICK(const char *nickname, user *user);
int USER(char *username, char* realname, user *user);
int OPER(char *username, char *password, user *user);
int MODE(char *nickname, char sign, char mode, user *user);
//int JOIN(char *channel ,user *user);


//utils
user *find_user(int fd);
int send_message(char *s, user *user, int ret);

//debug
void print_user(user *user);

#endif