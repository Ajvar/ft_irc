/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 17:46:28 by jcueille          #+#    #+#             */
/*   Updated: 2022/08/24 22:13:39 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"


void print_array(std::vector<s_user *> array)
{
	typename std::vector<s_user *>::iterator it = array.begin();
	for (; it != array.end(); it++)
		std::cout << (*it)->nickname << std::endl;
}

/**
 * @brief Leave a channel
 * 
 * @param chan list of channels to leave
 * @param reasons optionnal reason to leave
 * @param u the user leaving the channel(s)
 * @return int 
 */
int PART(std::vector<std::string> chan, const std::string &reason, user *u)
{
	if (chan.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "", "", "", ""), u, ERR_NEEDMOREPARAMS);
	for (std::vector<std::string>::iterator it = chan.begin(); it != chan.end(); it++)
	{
		channel *tmp = NULL;
		if (!(tmp = find_channel_by_name((*it))))
			send_message(create_msg(ERR_NOSUCHCHANNEL, u, (*it), "", "", ""), u, ERR_NOSUCHCHANNEL);
		else
		{
			if (!(tmp = find_chan_in_u((*it), u)))
				return send_message(create_msg(ERR_NOTONCHANNEL, u, (*it), "", "", ""), u, ERR_NOTONCHANNEL);
			send_to_all_chan(channel_message("PART " + (*it) + " " + reason, u), tmp);
			delete_u_in_chan(u->nickname, tmp);
			delete_chan_in_u(*it, u);
			/* if (tmp->users.empty())
				delete_channel(tmp->name); */
			pp(RED, "array: ");
			print_array(tmp->users);
		}
	}
	return 0;
}