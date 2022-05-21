/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/21 23:34:16 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"
#include <fstream>

extern user *users;
extern channel *channels;

/*************************************************************/
/* 	Compares server password with user provided password     */
/*************************************************************/
int PASS(const char *server_password, const char *user_password, user *user)
{
	if (!user_password)
		return ERR_NEEDMOREPARAMS;
	std::cout << "server pass : "<< server_password << " user pass :" << user_password << std::endl;
	if (strcmp(user_password, server_password) != 0)
	{
		send(user->fd->fd, "Wrong password", 16, 0);
		return 1;
	}
	return 0;
}

/*************************************************************/
/* Gives nickname to new user or changes nickname 			 */
/* from existing user 										 */
/*************************************************************/
int NICK(const char *nickname, user *user)
{
	s_user *tmp = users;
	std::string buff;
	
	if (!(nickname))
		return send_message((char *)"ERR_NONICKNAMEGIVEN", user, ERR_NONICKNAMEGIVEN);

	buff = "You are now known as " + std::string(nickname);
	while (tmp)
	{
		if (tmp->nickname == nickname && tmp->id != user->id)
			return send_message((char *)"ERR_NICKNAMEINUSE", user, ERR_NICKNAMEINUSE);

	}
	if (user->nickname.empty() == false)
		send(user->fd->fd, buff.c_str(), buff.size(), 0);
	user->nickname = nickname;
	return 0;
}

/*************************************************************/
/* Gives username and real name to new user					 */
/* argv[0] username - argv[1] realname						 */
/*************************************************************/
int USER(char *username, char* realname, user *user)
{
	struct s_user *tmp = users;

	while (tmp)
	{
		if (tmp->username == username)
			return send_message((char *)"ERR_ALREADYREGISTRED", user, ERR_ALREADYREGISTRED);
		tmp = tmp->next;
	}
	user->username = username;
	user->realname = realname;
	return 0;
}

int OPER(char *username, char *password, user *user)
{
	
	std::ifstream infile("conf");
	std::string a, b;

	if (!username || !password)
		return send_message((char*)"Error: Too few parameters.\nUsage OPER nickname password.", user, ERR_NEEDMOREPARAMS);
		
	while (infile >> a >> b)
	{
	    if (a == std::string(username) && b == std::string(password))
		{
			user->modes[OPERATOR_MODE] = TRUE;
			return send_message((char*)"RPL_YOUREOPER", user , 0);			
		}
	}
	return send_message((char*)"ERR_PASSWDMISMATCH", user, ERR_PASSWDMISMATCH);
}

int MODE(char *nickname, char sign, char mode, user *user)
{
	short local_sign;

	sign == '+' ? local_sign = 1 : local_sign = 0;
	if (!nickname || !sign || !mode)
		return send_message((char *)"ERR_NEEDMOREPARAMS", user, ERR_NEEDMOREPARAMS);
	if (std::string(nickname) != user->nickname)
		return send_message((char *)"ERR_USERSDONTMATCH", user, ERR_USERSDONTMATCH);
	switch (mode)
	{
	case 'i' :
		user->modes[INVISIBLE_MODE] = local_sign;
		break;
	case 'w' :
		user->modes[WALLOPS_MODE] = local_sign;
		break;
	case 'r':
		if (local_sign)
			return 0;
		user->modes[RESTRICTED_MODE] = local_sign;
		break;
	case 'o':
		if (local_sign)
			return 0;
		user->modes[OPERATOR_MODE] = local_sign;
		break;
	
	default:
		return send_message((char *)"ERR_UMODEUNKNOWNFLAG", user, ERR_UMODEUNKNOWNFLAG);
		break;
	}
	return 0;
}
/*
int AWAY(char *away_msg, user *user)
{
	
}*/