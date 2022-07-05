/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 16:46:15 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/05 13:58:43 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"

static int USR_MODE(const std::string &target, const char &sign, const char &mode, user *u)
{
	short local_sign;

	if (!find_user_by_nickname(target))
		return send_message(create_msg(ERR_NOSUCHNICK, u, target, "", "", ""), u, ERR_NOSUCHNICK);
	if (target != u->nickname)
		return send_message(create_msg(ERR_USERSDONTMATCH, u, "", "", "", ""), u, ERR_USERSDONTMATCH);
	if (sign)
	{
		if (sign != '+' && sign != '-')
			return send_message(create_msg(ERR_UMODEUNKNOWNFLAG, u, "", "", "", ""), u, ERR_UMODEUNKNOWNFLAG); 
		sign == '+' ? local_sign = 1 : local_sign = 0;
		switch (mode)
		{
		case 'i' :
			u->modes[INVISIBLE_MODE] = local_sign;
			break;
		case 'w' :
			u->modes[WALLOPS_MODE] = local_sign;
			break;
		case 'r':
			if (local_sign)
				return 0;
			u->modes[RESTRICTED_MODE] = local_sign;
			break;
		case 'o':
			if (local_sign)
				return 0;
			u->modes[OPERATOR_MODE] = local_sign;
			break;
		
		default:
			return send_message("ERR_UMODEUNKNOWNFLAG", u, ERR_UMODEUNKNOWNFLAG);
			break;
		}	
	}
	else 
	{
		std::string str;
		
		if (u->modes[INVISIBLE_MODE])
			str += "i";
		if (u->modes[WALLOPS_MODE])
			str += "w";
		if(u->modes[RESTRICTED_MODE])
			str += "r";
		if(u->modes[OPERATOR_MODE])
			str += "o";
		return send_message(create_msg(RPL_UMODEIS, u, str, "", "", ""), u, RPL_UMODEIS);
	}
}

static int CHAN_MODE(const std::string &target, const char &sign, const char &mode, user *u)
{
	const channel *c = find_channel_by_name(target);
	short local_sign;

	if (!c)
		return send_message(create_msg(ERR_NOSUCHCHANNEL, u, target, "", "", ""), u, ERR_NOSUCHCHANNEL);
	if (!sign)
	{
		if (!is_chan_ope(c, u))
			return send_message(create_msg(ERR_CHANOPRIVSNEEDED, u, target, "", "", ""), u, ERR_CHANOPRIVSNEEDED);
		if (sign != '+' && sign != '-')
			return send_message(create_msg(ERR_UMODEUNKNOWNFLAG, u, "", "", "", ""), u, ERR_UMODEUNKNOWNFLAG); 
		sign == '+' ? local_sign = 1 : local_sign = 0;
		switch (mode)
		{
		case 'i' :
			u->modes[INVISIBLE_MODE] = local_sign;
			break;
		case 'w' :
			u->modes[WALLOPS_MODE] = local_sign;
			break;
		case 'r':
			if (local_sign)
				return 0;
			u->modes[RESTRICTED_MODE] = local_sign;
			break;
		case 'o':
			if (local_sign)
				return 0;
			u->modes[OPERATOR_MODE] = local_sign;
			break;
		
		default:
			return send_message("ERR_UMODEUNKNOWNFLAG", u, ERR_UMODEUNKNOWNFLAG);
			break;
	}
	else
	{
		
	}
}

/**
 * * @brief Changes user modes
 * 
 * @param nickname the target's nickname
 * @param sign + or -
 * @param mode a, i, w, r, o
 * @param u 
 * @return 0 on success 
 */
int MODE(const std::string &target, const char &sign, const char &mode, user *u)
{
	if (target.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "MODE", "", "", ""), u, ERR_NEEDMOREPARAMS);
	
	if (target[0] == '#' || target[0] == '&')
		return CHAN_MODE(target, sign, mode, u);
	else
		return USR_MODE(target, sign, mode, u);
	return 0;
}