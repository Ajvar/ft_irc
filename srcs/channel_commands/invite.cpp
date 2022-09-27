/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 17:46:28 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/12 17:49:35 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"


/**
 * @brief Invite new user to channel
 * 
 * @param us 
 * @param c 
 * @param u 
 * @return int 
 */
int INVITE(const std::string &us, const std::string &c, user *u)
{
	channel *tmp_chan = find_channel_by_name(us);
	user *tmp_user;
	
	if (us.empty() || c.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "INVITE ", "", "", ""), u, ERR_NEEDMOREPARAMS);
	if (!(tmp_chan))
		return send_message(create_msg(ERR_NOSUCHCHANNEL, u, us, "", "", ""), u, ERR_NOSUCHCHANNEL);
	if (!(tmp_user = find_u_in_chan(u->nickname, tmp_chan)))
		return send_message(create_msg(ERR_NOTONCHANNEL, u, tmp_chan->name, "", "", ""), u, ERR_NOTONCHANNEL);
	if (tmp_chan->modes[INVITE_ONLY_MODE] && !is_chan_ope(tmp_chan, u))
		return send_message(create_msg(ERR_CHANOPRIVSNEEDED, u, tmp_chan->name, "", "", ""), u, ERR_CHANOPRIVSNEEDED);
	if (find_u_in_chan(us, tmp_chan))
		return send_message(create_msg(ERR_USERONCHANNEL, u, u->nickname, tmp_chan->name, "", ""), u, ERR_USERONCHANNEL);
	send_message(channel_message("INVITE ", u), tmp_user, 0);
	tmp_chan->invites.push_back(tmp_user->nickname);
	return send_message(create_msg(RPL_INVITING, u, tmp_chan->name, tmp_user->nickname, "", ""), u, RPL_INVITING);
}
