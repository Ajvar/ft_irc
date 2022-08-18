/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 16:01:24 by jcueille          #+#    #+#             */
/*   Updated: 2022/08/18 16:14:09 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"

static int CHAN_PRIVMSG(const std::string& c, const std::string &text, user *u)
{
	pp("CHAN NAME: " + c + " text is  " + text, "");
	channel *tmp = find_channel_by_name(c);
	// find_channel_by_name((c[0] == '@' || c[0] == '+') ? c.substr(1) : c);
	
	if (!tmp)
		return send_message(create_msg(ERR_NOSUCHNICK, u, c, "", "", ""), u, ERR_NOSUCHNICK);
	if (is_banned(tmp, u->nickname))
		return send_message(create_msg(ERR_CANNOTSENDTOCHAN, u, c, "", "", ""), u, ERR_CANNOTSENDTOCHAN);
	if (!(find_u_in_chan(u->nickname, tmp)) && tmp->modes[NO_EXTERN_MSG_MODE])
		return send_message(create_msg(ERR_CANNOTSENDTOCHAN, u, c, "", "", ""), u, ERR_CANNOTSENDTOCHAN);
	std::vector<user *>::iterator it = tmp->users.begin();
	for (; it != tmp->users.end(); it++)
	{
	//	if ((c[0] == '+' && (is_chan_ope(tmp, (*it)) || is_chan_voice(tmp, (*it)))) || (c[0] == '@' && is_chan_ope(tmp, (*it))))
	//	{
			if ((*it)->modes[AWAY_MODE])
				send_message(create_msg(RPL_AWAY, u, (*it)->nickname, (*it)->away_msg, "", ""), u, RPL_AWAY);
			else
				send_message(channel_message("PRIVMSG " + c + " :" + text, u), (*it), 0);
	//	}
	}
	return 0;
}

static int USR_PRIVMSG(const std::string& us, const std::string &text, user *u)
{
	user *tmp = find_user_by_nickname(us);
	
	if (!tmp)
		return send_message(create_msg(ERR_NOSUCHNICK, u, us, "", "", ""), u, ERR_NOSUCHNICK);
	if (tmp->modes[AWAY_MODE])
			send_message(create_msg(RPL_AWAY, u, tmp->nickname, tmp->away_msg, "", ""), u, RPL_AWAY);
	else
		send_message(channel_message("PRIVMSG " + us + " :" + text, u), tmp, 0);
	return 0;
}

int PRIVMSG(std::vector<std::string> targets, const std::string &msg, user *u)
{
	std::vector<std::string>::iterator it = targets.begin();
	pp("target is: " + targets[0] + " test is: " + msg, "");
	if (targets.empty())
		return send_message(create_msg(ERR_NORECIPIENT, u, "PRIVMSG ",  "", "", ""), u, ERR_NORECIPIENT);
	if (msg == "")
		return send_message(create_msg(ERR_NOTEXTTOSEND, u, "", "", "", ""), u, ERR_NOTEXTTOSEND);
	
	for (; it != targets.end(); it++)
	{
		if ((*it)[0] == '#' || (*it)[0] == '&')
			CHAN_PRIVMSG((*it), msg, u);
		else if ((*it)[1] == '#' || (*it)[1] == '&' )
			CHAN_PRIVMSG((*it).substr(1), msg, u);
	else
		USR_PRIVMSG((*it), msg, u);
	}
	return 0;
}

static int CHAN_NOTICE(const std::string& c, const std::string &text, user *u)
{
	channel *tmp = find_channel_by_name((c[0] == '@' || c[0] == '+') ? c.substr(1) : c);
	
	if (!tmp)
		return 0;
	if (is_banned(tmp, u->nickname))
		return 0;
	if (!(find_u_in_chan(u->nickname, tmp)) && tmp->modes[NO_EXTERN_MSG_MODE])
		return 0;
	if (c[0] == '@' || c[0] == '+')
	{
		std::vector<user *>::iterator it = tmp->users.begin();
		for (; it != tmp->users.end(); it++)
		{
			if ((c[0] == '+' && (is_chan_ope(tmp, (*it)) || is_chan_voice(tmp, (*it)))) || (c[0] == '@' && is_chan_ope(tmp, (*it))))
			{
				if ((*it)->modes[AWAY_MODE])
					return 0;
				else
					send_message(channel_message(text, u), (*it), 0);
			}
		}
	}
	
	return 0;
}

static int USR_NOTICE(const std::string& us, const std::string &text, user *u)
{
	user *tmp = find_user_by_nickname(us);
	
	if (!tmp)
		return 1;
	if (tmp->modes[AWAY_MODE])
			return 2;
	else
		return send_message(channel_message(text, u), tmp, 0);
}

int NOTICE(std::vector<std::string> targets, const std::string &text, user *u)
{
	std::vector<std::string>::iterator it = targets.begin();

	if (targets.empty() || text == "")
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "NOTICE ",  "", "", ""), u, ERR_NEEDMOREPARAMS);

	for (; it != targets.end(); it++)
	{
		if ((*it)[0] == '#' || (*it)[0] == '&')
			CHAN_NOTICE((*it), text, u);
		else if ((*it)[1] == '#' || (*it)[1] == '&' )
			CHAN_NOTICE((*it).substr(1), text, u);
	else
		USR_NOTICE((*it), text, u);
	}
	return 0;
}