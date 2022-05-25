/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 13:33:22 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/25 12:12:54 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

extern struct s_user *users;
extern struct s_channel *channels;

/*****************************************************/
/* Sends message to user's client		             */
/*****************************************************/
int send_message(std::string s, user *user, int ret)
{
	if (s.empty() == 0)
		send(user->fd->fd, s.c_str(), s.size() + 1, MSG_NOSIGNAL);
	return ret;
}

/*****************************************************/
/* Find the user by its fd				             */
/*****************************************************/
user *find_user_by_id(int fd)
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

user *find_user_by_nickname(std::string nickname)
{
	user *tmp = users;

	if (!tmp)
		return NULL;
	while (tmp)
	{
		if (std::string(nickname) == tmp->nickname)
			return tmp;
		tmp = tmp->next;	
	}
	return NULL;
}

void compress_array(pollfd *fds, int *nfds)
{
	int j = 0;
	for (int i = 0; i < (*nfds); i++)
	{
		if (fds[i].fd == -1)
		{
			for(j = i; j < (*nfds)-1; j++)
				fds[j].fd = fds[j+1].fd;
			i--;
			(*nfds)--;
		}
	}
}

