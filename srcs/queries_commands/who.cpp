/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 17:46:28 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/12 17:50:35 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

/**
 * @brief Queries list of users on a server or a single user
 * 
 * @param mask 
 * @param u 
 * @return int 
 */
int WHO(const std::string &mask, user *u)
{
	if (mask != "" && (mask[0] == '#' || mask[0] == '&'))
	{
		const channel *tmp = find_channel_by_name(mask);
		if (!tmp)
			return send_message(create_msg(ERR_NOSUCHSERVER, u, mask, "", "", ""), u, ERR_NOSUCHSERVER);
		std::vector<user *>::const_iterator it = tmp->users.begin();
		for (; it != tmp->users.end(); it++)
		{
			std::string str;
			str += tmp->name + " ";
			str += (*it)->username + " ";
			str += (*it)->hostname + " ";
			str += (*it)->hostname + " ";
			str += (*it)->nickname + " ";
			if ((*it)->modes[AWAY_MODE])
				str += "G";
			else
				str += "H";
			if ((*it)->modes[OPERATOR_MODE])
				str += "*";
			if (is_chan_ope(tmp, (*it)))
				str += "@";
			else if (is_chan_voice(tmp, (*it)))
				str += "+";
			str += " ";
			str += ":0 ";
			str += (*it)->realname;
			send_message(create_msg(RPL_WHOREPLY, u, str, "", "", ""), u, RPL_WHOREPLY);
		}

	}
	else
	{
		const user * tmp = find_user_by_nickname(mask);
		if (!tmp)
			return send_message(create_msg(ERR_NOSUCHSERVER, u, mask, "", "", ""), u, ERR_NOSUCHSERVER);
					std::string str;
		str += "* ";
		str += (tmp)->username + " ";
		str += (tmp)->hostname + " ";
		str += (tmp)->hostname + " ";
		str += (tmp)->nickname + " ";
		if ((tmp)->modes[AWAY_MODE])
			str += "G";
		else
			str += "H";
		if ((tmp)->modes[OPERATOR_MODE])
			str += "*";
		str += " ";
		str += ":0 ";
		str += (tmp)->realname;
		send_message(create_msg(RPL_WHOREPLY, u, str, "", "", ""), u, RPL_WHOREPLY);
	}
	return send_message(create_msg(RPL_ENDOFWHO, u, "", "", "", ""), u, RPL_ENDOFWHO);
}