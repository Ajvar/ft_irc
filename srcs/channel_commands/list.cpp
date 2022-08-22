/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 17:46:28 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/12 22:12:32 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

extern channel *channels;

/**
 * @brief Gives list of channels and their topics
 * 
 * @param c channel names list
 * @param u 
 * @return int 
 */
int LIST(const std::vector<std::string> c, user *u)
{
	channel *tmp;
	if (c.empty())
	{
		tmp = channels;
		if (tmp)
			send_message(create_msg(RPL_LISTSTART, u, "", "", "", ""), u, RPL_LISTSTART);
		while(tmp)
		{
			if (!tmp->modes[SECRET_MODE])
				send_message(create_msg(RPL_LIST, u, tmp->name, tmp->topic, "", ""), u, RPL_LIST);
			tmp = tmp->next;
		}
	}
	else
	{
		std::vector<std::string>::const_iterator it = c.begin();
		
		send_message(create_msg(RPL_LISTSTART, u, "", "", "", ""), u, RPL_LISTSTART);
		for (; it != c.end(); it++)
		{
			if ((tmp = find_channel_by_name((*it))))
				send_message(create_msg(RPL_LIST, u, (*it), tmp->topic, "", ""), u, RPL_LIST);
		}
	}
	return send_message(create_msg(RPL_LISTEND, u, "", "", "", ""), u, RPL_LISTEND);
}