/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 13:33:22 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/21 22:45:32 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

extern struct s_user *users;
extern struct s_channel *channels;

/*****************************************************/
/* Sends message to user's client		             */
/*****************************************************/
int send_message(char *s, user *user, int ret)
{
	send(user->fd->fd, s, strlen(s) + 1, 0);
	return ret;
}

/*****************************************************/
/* Find the user by its fd				             */
/*****************************************************/
user *find_user(int fd)
{
	user *tmp = users;

	if (!tmp)
		return NULL;
	while (tmp)
	{
		if (tmp->fd->fd == fd)
			return tmp;
	}
	return NULL;
}