/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/22 18:21:51 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

/**
 * * @brief Closes user's connexion
 * ! Are we supposed to send the msg to every other users ?
 * @param msg
 * @param fds
 * @param nfds
 * @param u
 * @return int
 */
int QUIT(const std::string &msg, pollfd *fds, int *nfds, user *u)
{
	send_message(msg, u, 0);
	close(u->fd->fd);
	u->fd->fd = -1;
	delete_client(u);
	compress_array(fds, nfds);
	return 0;
}