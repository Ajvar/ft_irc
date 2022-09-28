/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 17:46:28 by jcueille          #+#    #+#             */
/*   Updated: 2022/08/25 00:23:49 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

static int is_invited(channel* tmp, user *u)
{
	pp(RED, "invite only");
	std::vector<std::string>::iterator inv = tmp->invites.begin();
	for (; inv != tmp->invites.end(); inv++)
	{
		if ((*inv) == u->nickname)
			return 1;
	}
	return 0;
}

/**
 * @brief Join one or multiple servers using keys if necesary
 * 
 * @param chan list of channels to join
 * @param keys list of keys to join channels
 * @param option 0 option to leave all channels at once
 * @param u user joining the channels
 * @param fds list of fds used if exit needed
 * @param nfds number of fds to exit if needed
 * @return int 
 */
int JOIN(std::vector<std::string> chan, std::vector<std::string> keys, user *u)
{
	if (chan.empty() || !u)
		return (send_message(create_msg(ERR_NEEDMOREPARAMS, u, "JOIN","", "", ""), u, ERR_NEEDMOREPARAMS));
	std::vector<std::string>::iterator	it = chan.begin();
	std::vector<std::string>::iterator	ite = chan.end();
	std::vector<std::string>::iterator	k = keys.begin();
	std::string							nicks;
	channel 							*tmp = NULL;
	
	if ((*it) == "0")
	{
		while (!(u->channels.empty()))
		{
			std::vector<std::string> v;
			v.push_back(u->channels[0]->name);
			PART(v, "", u);
		}
		return 0;
	}

	for (; it != ite; it++)
	{
		if (!(tmp = find_channel_by_name((*it))))
		{
			tmp = new_channel((*it));
			if (!tmp)
			{
				send_message("Error while creating new channel", u, -1);
				ft_free_exit("Error creating channel", -1);
			}
			tmp->name = (*it);
			tmp->creator = u;
			tmp->operators.push_back(u);
		}
		if ((find_u_in_chan(u->nickname, tmp)))
				continue ;
		if (tmp->modes[BAN_MODE])
		{
			if (is_banned(tmp, u->nickname))
			{
					send_message(create_msg(ERR_BANNEDFROMCHAN, u, tmp->name, "", "", ""), u, ERR_BANNEDFROMCHAN);
					continue;
			}
		}
		if (tmp->modes[INVITE_ONLY_MODE] && !is_invited(tmp, u))
			return send_message(create_msg(ERR_INVITEONLYCHAN, u, tmp->name,"", "", ""), u, ERR_INVITEONLYCHAN);
		if (tmp->modes[USER_LIMIT_MODE])
		{
			if (tmp->users.empty() == 1)
				if ((int)tmp->users.size() >= tmp->user_limit)
					return send_message(create_msg(ERR_CHANNELISFULL, u, tmp->name,"", "", ""), u, ERR_CHANNELISFULL);
		}
		if (tmp->modes[KEY_LOCKED_MODE])
		{
			if (k == keys.end() || (tmp->key != "" && tmp->key != (*k)))
			{
				send_message(create_msg(ERR_BADCHANNELKEY, u, tmp->name,"", "", ""), u, ERR_BADCHANNELKEY);
				if (k != keys.end())
					k++;
				continue;
			}
			k++;
		}
		tmp->users.push_back(u);
		u->channels.push_back(tmp);
		send_to_all_chan(channel_message("JOIN " + tmp->name, u), tmp);
		if (tmp->topic != "")
			send_message(create_msg(RPL_TOPIC, u, tmp->topic, "", "", ""), u, 0);
		for (std::vector<user *>::iterator uz = tmp->users.begin(); uz !=  tmp->users.end(); uz++)
			nicks = nicks + (nicks == "" ? "" : " ") + (is_chan_ope(tmp, (*uz)) ? "@" : "") + (*uz)->nickname;
		send_message(create_msg(RPL_NAMREPLY, u, tmp->modes[SECRET_MODE] ? "@ " : "= " + tmp->name, nicks, "", ""), u, 0);
		send_message(create_msg(RPL_ENDOFNAMES, u, tmp->name, "", "", ""), u, 0);
	}
	return 0;
}