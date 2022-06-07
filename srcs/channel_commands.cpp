/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_commands.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 00:03:44 by jcueille          #+#    #+#             */
/*   Updated: 2022/06/08 01:09:27 by jcueille         ###   ########.fr       */
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
int JOIN(std::vector<std::string> chan, std::vector<std::string> keys, const int &option, user *u, pollfd *fds, int nfds)
{
	if (chan.empty() || !u)
		return (send_message(create_msg(ERR_NEEDMOREPARAMS, u, "JOIN","", "", ""), u, ERR_NEEDMOREPARAMS));
	std::vector<std::string>::iterator it = chan.begin();
	std::vector<std::string>::iterator ite = chan.end();
	std::vector<std::string>::iterator k = keys.begin();
	std::string							nicks;
	channel *tmp = NULL;
	
	if (option)
	{
		for (std::vector<channel *>::iterator it = u->channels.begin(); it != u->channels.end(); it++)
		{
			std::vector<std::string> v;
			std::vector<std::string> r;
			v.push_back((*it)->name);
			PART(v, r, u);
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

/**
 * @brief Quit a server and sends a message with a reason why
 * 
 * @param chan list of channels to leave
 * @param reasons list of reasons to leave
 * @param u the user leavinf the channels
 * @return int 
 */
int PART(std::vector<std::string> chan, std::vector<std::string> reasons, user *u)
{
	std::vector<std::string>::iterator r = reasons.begin();
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
			send_message(channel_message("PART " + (*it) + (r != reasons.end() ? (*r++) : ""), u), u, 0);
		}
	}
	return 0;
}


/**
 * @brief	Print topic if topic is empty
 * 			clear channel topic if topic = :
 * 			set channel topic if : + string
 * 
 * @param topic 
 * @param chan target channel name
 * @param u user launching the command
 * @return int 
 */
int TOPIC(std::string &topic, const std::string &chan, user *u)
{
	channel *c = find_channel_by_name(chan);
	if (!c)
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "TOPIC", "", "", ""), u, ERR_NEEDMOREPARAMS);
	if (!find_u_in_chan(u->nickname, c))
		return send_message(create_msg(ERR_NOTONCHANNEL, u, c->name, "", "", ""), u, ERR_NOTONCHANNEL);
	if (topic == "")
	{
		if (c->topic == "")
			return send_message(create_msg(RPL_NOTOPIC, u, c->name, "", "", ""), u, RPL_NOTOPIC);
		return send_message(create_msg(RPL_TOPIC, u, c->name, c->topic, "", ""), u, RPL_TOPIC);
	}
	if (!is_chan_ope(c, u))
		return send_message(create_msg(ERR_CHANOPRIVSNEEDED, u, c->name, "", "", ""), u, ERR_CHANOPRIVSNEEDED);
	c->topic = topic.erase(0,1);
	std::vector<user *>::iterator it = c->users.begin();
	for (; it != c->users.end(); it++)
		send_message(channel_message("TOPIC " + topic, u), (*it), 0);
	return 0;
}