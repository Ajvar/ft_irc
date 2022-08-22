/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/08/22 20:46:00 by jcueille         ###   ########.fr       */
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
int AWAY(const std::string &away_msg, user *u)
{
	if (u && u->modes[AWAY_MODE] == 0)
	{
		u->modes[AWAY_MODE] = 1;
		u->away_msg = std::string(away_msg);
		send_message(create_msg(RPL_NOWAWAY, u,"", "", "", ""), u, RPL_NOWAWAY);
		return 0;
	}
	if (u && u->modes[AWAY_MODE] == 1)
	{
		u->modes[AWAY_MODE] = 0;
		u->away_msg = "";
		return send_message(create_msg(RPL_UNAWAY, u,"", "", "", ""), u, RPL_UNAWAY);
		return 0;
	}
	return -1;
}
