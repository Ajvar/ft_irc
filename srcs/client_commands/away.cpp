/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/13 14:22:12 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

/**
 * * @brief Sets / unset AWAY_MODE on user and stores automatic
 * away reply
 * 
 * @param away_msg away message to send back to user sending PRIVMSG
 * @param user 
 * @return RPL_NOWAYWAY or RPL_UNAWAY on success, -1 on failure 
 */
int AWAY(const std::string &away_msg, user *user)
{
	if (user && user->modes[AWAY_MODE] == 0)
	{
		user->modes[AWAY_MODE] = 1;
		user->away_msg = std::string(away_msg);
		send_message(create_msg(RPL_NOWAWAY, user,"", "", "", ""), user, RPL_NOWAWAY);
	}
	if (user && user->modes[AWAY_MODE] == 1)
	{
		user->modes[AWAY_MODE] = 0;
		user->away_msg = "";
		return send_message(create_msg(RPL_UNAWAY, user,"", "", "", ""), user, RPL_UNAWAY);
	}
	return -1;
}
