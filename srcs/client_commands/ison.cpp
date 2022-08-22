/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ison.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/13 14:27:22 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

/**
 * * @brief Take a list of nicknames
 * * and send back a list of those who are connected on the server
 * 
 * @param nicknames 
 * @param u 
 * @return RPL_ISON on success 
 */
int ISON(const std::vector<std::string> nicknames, user *u)
{
	std::string user_list = NULL;

	if (nicknames.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "ISON","", "", ""), u, ERR_NEEDMOREPARAMS);
	for (std::vector<std::string >::const_iterator it = nicknames.begin(); it != nicknames.end(); it++)
		if (find_user_by_nickname((*it)))
			user_list = user_list + (*it);
	return send_message(create_msg(RPL_ISON, u, user_list, "", "", ""), u, RPL_ISON);
}