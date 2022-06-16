/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_commands.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 00:03:44 by jcueille          #+#    #+#             */
/*   Updated: 2022/06/16 21:37:34 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"

extern struct s_channel *channels;

/**
 * @brief Creates a message that will be sent back to users on a channel
 * 
 * @param msg 
 * @param u 
 * @return std::string 
 */
std::string channel_message(const std::string &msg, user *u)
{
	std::string	full_msg;

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
int JOIN(std::vector<std::string> chan, std::vector<std::string> keys, const std::string &option, user *u, pollfd *fds, int nfds)
{
	if (chan.empty() || !u)
		return (send_message(create_msg(ERR_NEEDMOREPARAMS, u, "JOIN","", "", ""), u, ERR_NEEDMOREPARAMS));
	std::vector<std::string>::iterator it = chan.begin();
	std::vector<std::string>::iterator ite = chan.end();
	std::vector<std::string>::iterator k = keys.begin();
	std::string							nicks;
	channel *tmp = NULL;
	
	if (option == "0")
	{
		for (std::vector<channel *>::iterator it = u->channels.begin(); it != u->channels.end(); it++)
		{
			std::vector<std::string> v;
			std::vector<std::string> r;
			v.push_back((*it)->name);
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
				ft_free_exit("Error creating channel", -1, fds, nfds);
			}
			tmp->name = (*it);
			tmp->creator = u;
		}
		if ((find_u_in_chan(u->nickname, tmp)))
				continue ;
		if (tmp->modes[BAN_MODE])
		{
			if (is_banned(tmp, u))
			{
					send_message(create_msg(ERR_BANNEDFROMCHAN, u, tmp->name, "", "", ""), u, ERR_BANNEDFROMCHAN);
					continue;
			}	
		}
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
			delete_chan_in_u(*it, u);
			send_message(channel_message("PART " + (*it) + reason, u), u, 0);
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
	print_channels();
	std::cout << "chan name is: "<< chan << std::endl;
	channel *c = find_channel_by_name(chan);
	if (!c)
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "TOPIC", "", "", ""), u, ERR_NEEDMOREPARAMS);
	if (!find_u_in_chan(u->nickname, c))
		return send_message(create_msg(ERR_NOTONCHANNEL, u, c->name, "", "", ""), u, ERR_NOTONCHANNEL);
	if (topic == "")
	{
		std::cout << "empty topic" << std::endl;

		if (c->topic == "")
			return send_message(create_msg(RPL_NOTOPIC, u, c->name, "", "", ""), u, RPL_NOTOPIC);
		return send_message(create_msg(RPL_TOPIC, u, c->name, c->topic, "", ""), u, RPL_TOPIC);
	}
	if (c->modes[TOPIC_LOCKED_MODE] && !is_chan_ope(c, u))
		return send_message(create_msg(ERR_CHANOPRIVSNEEDED, u, c->name, "", "", ""), u, ERR_CHANOPRIVSNEEDED);
	c->topic = topic.erase(0,1);
	std::vector<user *>::iterator it = c->users.begin();
	for (; it != c->users.end(); it++)
		send_message(channel_message("TOPIC " + topic, u), (*it), 0);
	return 0;
}

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
			return send_message(create_msg(RPL_ENDOFNAMES, u, (*it), "", "", ""), u, RPL_ENDOFNAMES);
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
				name_list += names_and_modes(c, *us);
			}
			send_message(create_msg(RPL_NAMREPLY, u, c->name, name_list, "", ""), u, RPL_NAMREPLY);
			send_message(create_msg(RPL_ENDOFNAMES, u, c->name, "", "", ""), u, RPL_ENDOFNAMES);
		}
	}
	return 0;
}

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

int INVITE(const std::string &us, const std::string &c, user *u)
{
	const channel *tmp_chan = find_channel_by_name(us);
	user *tmp_user;
	
	if (us.empty() || c.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "INVITE ", "", "", ""), u, ERR_NEEDMOREPARAMS);
	if (!(tmp_chan))
		return send_message(create_msg(ERR_NOSUCHCHANNEL, u, us, "", "", ""), u, ERR_NOSUCHCHANNEL);
	if (!(tmp_user = find_u_in_chan(u->nickname, tmp_chan)))
		return send_message(create_msg(ERR_NOTONCHANNEL, u, tmp_chan->name, "", "", ""), u, ERR_NOTONCHANNEL);
	if (tmp_chan->modes[INVITE_ONLY_MODE] && is_chan_ope(tmp_chan, u))
		return send_message(create_msg(ERR_CHANOPRIVSNEEDED, u, tmp_chan->name, "", "", ""), u, ERR_CHANOPRIVSNEEDED);
	if (find_u_in_chan(us, tmp_chan))
		return send_message(create_msg(ERR_USERONCHANNEL, u, u->nickname, tmp_chan->name, "", ""), u, ERR_USERONCHANNEL);
	send_message(channel_message("INVITE ", u), tmp_user, 0);
	return send_message(create_msg(RPL_INVITING, u, tmp_chan->name, tmp_user->nickname, "", ""), u, RPL_INVITING);
}

int KICK(const std::string &c, std::vector<std::string> us, const std::string &comment ,user *u)
{
	channel *tmp_chan = find_channel_by_name(c);
	user	*tmp_user;
	if (c == "" || us.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "KICK ", "", "", ""), u, ERR_NEEDMOREPARAMS);
	if (!tmp_chan)
		return send_message(create_msg(ERR_NOSUCHCHANNEL, u, c, "", "", ""), u, ERR_NOSUCHCHANNEL);
	if (!is_chan_ope(tmp_chan, u))
		return send_message(create_msg(ERR_CHANOPRIVSNEEDED, u, tmp_chan->name, "", "", ""), u, ERR_CHANOPRIVSNEEDED);
	std::vector<std::string>::iterator it = us.begin();
	if (!find_u_in_chan(u->nickname, tmp_chan))
		return send_message(create_msg(ERR_NOTONCHANNEL, u, tmp_chan->name, "", "", ""), u, ERR_NOTONCHANNEL);
	for (; it != us.end(); it++)
	{
		if (!(tmp_user = find_u_in_chan((*it), tmp_chan)))
			send_message(create_msg(ERR_USERNOTINCHANNEL, u, (*it), tmp_chan->name, "", ""), u, ERR_USERNOTINCHANNEL);
		else
		{
			delete_chan_in_u(tmp_chan->name, tmp_user);
			delete_u_in_chan(tmp_user->nickname, tmp_chan);
			send_message(channel_message("KICK " + comment, u), tmp_user, 0);
		}
	}
	return 0;
}

int CHAN_PRIVMSG(const std::string& c, const std::string &text, user *u)
{
	channel *tmp = find_channel_by_name((c[0] == '@' || c[0] == '+') ? c.substr(1) : c);
	
	if (!tmp)
		return send_message(create_msg(ERR_NOSUCHNICK, u, c, "", "", ""), u, ERR_NOSUCHNICK);
	if (is_banned(tmp, u))
		return send_message(create_msg(ERR_CANNOTSENDTOCHAN, u, c, "", "", ""), u, ERR_CANNOTSENDTOCHAN);
	if (!(find_u_in_chan(u->nickname, tmp)) && tmp->modes[NO_EXTERN_MSG_MODE])
		return send_message(create_msg(ERR_CANNOTSENDTOCHAN, u, c, "", "", ""), u, ERR_CANNOTSENDTOCHAN);
	if (c[0] == '@' || c[0] == '+')
	{
		std::vector<user *>::iterator it = tmp->users.begin();
		for (; it != tmp->users.end(); it++)
		{
			if ((c[0] == '+' && (is_chan_ope(tmp, (*it)) || is_chan_voice(tmp, (*it)))) || (c[0] == '@' && is_chan_ope(tmp, (*it))))
			{
				if ((*it)->modes[AWAY_MODE])
					send_message(create_msg(RPL_AWAY, u, (*it)->nickname, (*it)->away_msg, "", ""), u, RPL_AWAY);
				else
					send_message(channel_message(text, u), (*it), 0);
			}
		}
	}
	
	return 0;
}

int PRIVMSG(std::vector<std::string> targets, const std::string &text, user *u)
{
	std::vector<std::string>::iterator it = targets.begin();

	if (targets.empty())
		return send_message(create_msg(ERR_NORECIPIENT, u, "PRIVMSG ",  "", "", ""), u, ERR_NORECIPIENT);
	if (text == "")
		return send_message(create_msg(ERR_NOTEXTTOSEND, u, "", "", "", ""), u, ERR_NOTEXTTOSEND);
	
	for (; it != targets.end(); it++)
	{
		if ((*it)[0] == '#' || (*it)[0] == '&' || (*it)[1] == '#' || (*it)[1] == '&' )
			CHAN_PRIVMSG((*it), text, u);
	//	else
			
	}
	return 0;
}