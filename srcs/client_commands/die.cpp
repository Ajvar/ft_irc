/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/08/25 00:22:18 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

/**
 * * @brief Terminates the server
 * 
 * @param u the user sending the command
 * @param fds all the socket descriptors
 * @param nfds the number of socket descriptors
 * @return ERR_NOPRIVILEGES on failure
 */
int	DIE(user *u)
{
	if (u->modes[OPERATOR_MODE] == 0)
		return send_message(create_msg(ERR_NOPRIVILEGES, u,"", "", "", ""), u, ERR_NOPRIVILEGES);
	send_message("Killing server.", u, 0);
	ft_free_exit("Killing server", 0);
	return 0;
}