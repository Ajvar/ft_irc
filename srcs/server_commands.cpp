/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 00:03:44 by jcueille          #+#    #+#             */
/*   Updated: 2022/06/06 14:33:26 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"

std::string channel_message(const std::string &msg, user *u)
{
	std::string				full_msg;

	full_msg = ":" + u->nickname + "!" + u->username +
	 			"@" + u->hostname + " " + msg + "\r\n";
	return full_msg;
}

int JOIN(std::vector<std::string> chan, std::vector<std::string> keys, int option, user *u, pollfd *fds, int nfds)
{
	if (chan.empty() || !u)
		return (send_message(create_msg(ERR_NEEDMOREPARAMS, u, "JOIN","", "", ""), u, ERR_NEEDMOREPARAMS));
	std::vector<std::string>::iterator it = chan.begin();
	std::vector<std::string>::iterator ite = chan.end();
	std::vector<std::string>::iterator k = keys.begin();
	std::string							nicks;
	channel *tmp = NULL;
	
	/*if (option)
	{
		for (std::vector<channel *>::iterator it = u->channels.begin(); it != u->channels.end(); it++)
		{
			send_message(create_msg())
		}
	}*/
	
	for (; it != ite; it++)
	{
		if (!(tmp = find_channel_by_name((*it))))
		{
			tmp = new_channel((*it));
			if (!tmp)
			{
				send_message("Error while creating new channel", u, -1);
				ft_free_exit("Error creating channel", -1, fds, nfds);
			}
			tmp->name = (*it);
			tmp->creator = u;
		}
		if ((find_u_in_chan(u->nickname, tmp)))
				continue ;
		if (tmp->modes[INVITE_ONLY_MODE])
		{
			for (std::vector<std::string>::iterator inv = tmp->invites.begin(); inv != tmp->invites.end(); inv++)
			{
				if ((*inv) == u->nickname)
					break;
				else if (inv == tmp->invites.end() && (*inv) != u->nickname)
					return send_message(create_msg(ERR_INVITEONLYCHAN, u, tmp->name,"", "", ""), u, ERR_INVITEONLYCHAN);
			}
		}
		if (tmp->modes[USER_LIMIT_MODE])
		{
			if (tmp->users.empty() == 1)
				if ((int)tmp->users.size() >= tmp->user_limit)
					return send_message(create_msg(ERR_CHANNELISFULL, u, tmp->name,"", "", ""), u, ERR_CHANNELISFULL);
		}
		for (std::vector<std::string>::iterator ban = tmp->banned.begin(); ban != tmp->banned.end(); ban++)
			if ((*ban) == u->nickname)
				return send_message(create_msg(ERR_BANNEDFROMCHAN, u, tmp->name,"", "", ""), u, ERR_BANNEDFROMCHAN);
		if (tmp->key != "" && tmp->key != (*k))
			return send_message(create_msg(ERR_BADCHANNELKEY, u, tmp->name,"", "", ""), u, ERR_BADCHANNELKEY);	
		k++;
		tmp->users.push_back(u);
		u->channels.push_back(tmp);
		std::cout << "sg: " << channel_message("JOIN " + tmp->name, u) << std::endl;
		send_message(channel_message("JOIN " + tmp->name, u), u, 0);
		if (tmp->topic != "")
			send_message(create_msg(RPL_TOPIC, u, tmp->topic, "", "", ""), u, 0);
		for (std::vector<user *>::iterator uz = tmp->users.begin(); uz !=  tmp->users.end(); uz++)
			nicks = nicks + (nicks == "" ? "" : " ") + (*uz)->nickname;
		std::cout << create_msg(RPL_NAMREPLY, u, tmp->name, nicks, "", "") << std::endl;
		send_message(create_msg(RPL_NAMREPLY, u, tmp->name, nicks, "", ""), u, 0);
		send_message(create_msg(RPL_ENDOFNAMES, u, tmp->name, "", "", ""), u, 0);
		
		(void)option;
		
	}
	return 0;
}

int PART(std::vector<std::string> chan, std::vector<std::string> reasons, user *u)
{
	(void)reasons;
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
			delete_chan_in_u(*it, u);
			send_message(channel_message("PART " + (*it), u), u, 0);
		}
	}
	return 0;
}