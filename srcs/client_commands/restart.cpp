/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/13 14:15:37 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

extern user *users;
extern channel *channels;

/**
 * * @brief Restarts server
 * 
 * @param user 
 * @param fds 
 * @param nfds 
 * @param restart pointer to restart from main
 * @return 0 on success, ERR_NOPRIVILEGES on failure.
 */
int RESTART(user *user, pollfd *fds, int nfds, int *restart)
{
	if (user->modes[OPERATOR_MODE] == 0)
		return send_message(create_msg(ERR_NOPRIVILEGES, user,"", "", "", ""), user, ERR_NOPRIVILEGES);
	free_users();
	free_channels();
	free_fds(fds, nfds);
	users = NULL;
	channels = NULL;
	(*restart) = 1;
	return 0;
}