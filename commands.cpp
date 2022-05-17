/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/17 16:42:14 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "replies.hpp"

extern user *users;
extern channel *channels;


int pass(const char *server_password, const char *user_password, user *user)
{
	char buffer[4096];
	if (!user_password)
		return ERR_NEEDMOREPARAMS;
	
	if (strcmp(user_password, server_password) != 0)
	{
		send(user->fd->fd, "Wrong password", 16, 0);
		return 1;
	}
	return 0;
}

int nick(const char *previous, const char *nickname, user *user)
{
	s_user *tmp = users;

	if (!(nickname))
		return ERR_NONICKNAMEGIVEN;

	while (tmp)
	{
		if (previous && tmp->nickname == previous)
		{
			tmp->nickname = nickname;
			return 0;
		}
		if (!previous && tmp->nickname == nickname)
			return ERR_NICKNAMEINUSE;
	}
	tmp->nickname = nickname;
	return 0;
	
}