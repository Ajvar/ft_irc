/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wallops.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/13 14:24:38 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

extern user *users;

/**
 * * @brief Sends message to all users with WALLOPS_MODE activated.
 * 
 * @param msg message to send
 * @param u the user sending the message
 * @return 0 on success, ERR_NEEDMOREPARAMS is msg is empty
 */
int WALLOPS(const std::string &msg, user *u)
{
	user *tmp = users;
	if (msg.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "WALLOPS","", "", ""), u, ERR_NEEDMOREPARAMS);
	while (tmp)
	{
		if (tmp != u && tmp->modes[WALLOPS_MODE] == 1)
			send_message(msg, u, 0);
		tmp = tmp->next;
	}
	return 0;
}