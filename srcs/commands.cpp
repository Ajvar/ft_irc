/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/20 20:58:28 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"

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
		return ERR_NONICKNAMEGIVEN;
	buff = "You are now known as " + std::string(nickname);
	while (tmp)
	{
		if (tmp->nickname == nickname && tmp->id != user->id)
			return ERR_NICKNAMEINUSE;
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
int USER(char *argv[2], user *user)
{
	struct s_user *tmp = users;

	while (tmp)
	{
		if (tmp->username == argv[0])
			return ERR_ALREADYREGISTRED;
		tmp = tmp->next;
	}
	user->username = argv[0];
	user->realname = argv[1];
	return 0;
}