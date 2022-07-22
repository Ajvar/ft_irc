/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 17:46:28 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/12 17:50:11 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

/**
 * @brief Kick user from channel.
 * 
 * @param c 
 * @param us 
 * @param comment 
 * @param u 
 * @return int 
 */
int KICK(const std::vector<std::string> &c, std::vector<std::string> us, const std::string &comment ,user *u)
{
	if (c.empty() || us.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "KICK ", "", "", ""), u, ERR_NEEDMOREPARAMS);
	std::vector<std::string>::const_iterator c_it = c.begin();
	for (; c_it != c.end(); c_it++)
	{
		channel *tmp_chan = find_channel_by_name((*c_it));
		user	*tmp_user;
		if (!tmp_chan)
			return send_message(create_msg(ERR_NOSUCHCHANNEL, u, (*c_it), "", "", ""), u, ERR_NOSUCHCHANNEL);
		if (!is_chan_ope(tmp_chan, u))
			return send_message(create_msg(ERR_CHANOPRIVSNEEDED, u, tmp_chan->name, "", "", ""), u, ERR_CHANOPRIVSNEEDED);
		std::vector<std::string>::iterator us_it = us.begin();
		if (!find_u_in_chan(u->nickname, tmp_chan))
			return send_message(create_msg(ERR_NOTONCHANNEL, u, tmp_chan->name, "", "", ""), u, ERR_NOTONCHANNEL);
		for (; us_it != us.end(); us_it++)
		{
			if (!(tmp_user = find_u_in_chan((*us_it), tmp_chan)))
				send_message(create_msg(ERR_USERNOTINCHANNEL, u, (*us_it), tmp_chan->name, "", ""), u, ERR_USERNOTINCHANNEL);
			else
			{
				delete_chan_in_u(tmp_chan->name, tmp_user);
				delete_u_in_chan(tmp_user->nickname, tmp_chan);
				send_message(channel_message("KICK " + comment, u), tmp_user, 0);
			}
		}
		
	}
	return 0;
}