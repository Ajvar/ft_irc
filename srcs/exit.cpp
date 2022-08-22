/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 12:30:02 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/24 13:54:38 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

extern struct s_user *users;
extern struct s_channel *channels;

void ft_exit(std::string s, int err, int *sock)
{
	if (err != 0)
		std::cerr << "\033[1;31m" << "Error: " << s << "\033[0m" << std::endl;
	if (sock)
		close(*sock);
	exit(err);
}

void free_users(void)
{
	user *tmp_users;
	
	while (users)
	{
		tmp_users = users;
		users = users->next;
		std::vector<channel *>().swap(tmp_users->channels);
		delete tmp_users;
	}
}

void free_channels(void)
{
	channel *tmp_channels = channels;

	while (channels)
	{
		tmp_channels = channels;
		channels = channels->next;
		std::vector<user *>().swap(tmp_channels->users);
		delete tmp_channels;
	}
}

void free_fds(pollfd *fds, int nfds)
{
	if (fds)
	{
		for (int i = 0; i < nfds; i++)
		{
			if(fds[i].fd >= 0)
				close(fds[i].fd);
		}
	}
}

void ft_free_exit(std::string s, int err, pollfd *fds, int nfds)
{
	if (err != 0)
		std::cerr << "\033[1;31m" << "Error: " << s << "\033[0m" << std::endl;
	free_users();
	free_channels();
	free_fds(fds, nfds);
	exit(err);
}