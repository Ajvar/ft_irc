/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 17:46:28 by jcueille          #+#    #+#             */
/*   Updated: 2022/08/24 22:31:34 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"


std::string names_and_modes(channel *c, user *u)
{
	std::string ret;

	if (is_chan_ope(c, u))
		ret += "@";
	else if (is_chan_voice(c, u))
		ret += "+";
	ret += u->nickname;
	return ret;
}

int NAMES(std::vector<std::string>chan, user *u)
{
	channel *c;
	std::string nick_list;
	std::vector<std::string>::iterator it = chan.begin();
	for (; it != chan.end(); it++)
	{
		if (!(c = find_channel_by_name((*it))))
			return send_message(create_msg(ERR_NOSUCHCHANNEL, u, (*it), "", "", ""), u, ERR_NOSUCHCHANNEL);
		else
		{
			std::string name_list = "";
			const channel *part_of_c = find_chan_in_u((*it), u);
			//Si u n'est pas dans le channel et que le channel est secret
			if (!part_of_c && c->modes[SECRET_MODE])
			{
				send_message(create_msg(RPL_ENDOFNAMES, u, (*it), "", "", ""), u, RPL_ENDOFNAMES);
				continue ;
			}
			std::vector<user *>::iterator us = c->users.begin();
			for (; us != c->users.end(); us++)
			{
				if (!part_of_c && (*us)->modes[INVISIBLE_MODE])
					continue ;
				name_list += (name_list.empty() ? "" : " ") + names_and_modes(c, *us);
			}
			send_message(create_msg(RPL_NAMREPLY, u, c->name, name_list, "", ""), u, RPL_NAMREPLY);
			send_message(create_msg(RPL_ENDOFNAMES, u, c->name, "", "", ""), u, RPL_ENDOFNAMES);
		}
	}
	return 0;
}