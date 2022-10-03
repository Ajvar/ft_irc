/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 14:13:47 by anon              #+#    #+#             */
/*   Updated: 2022/10/03 15:54:50 by anon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"
#include <sstream>

/**
 * Sends infos about an user
 **/
int WHOIS(const std::string arg, user *u)
{
    if (arg.empty())
        return send_message(create_msg(ERR_NONICKNAMEGIVEN, u, "", "", "", ""), u, ERR_NONICKNAMEGIVEN);
    
    user                *tmp;
    time_t              t; //Stores current time
    std::stringstream   ss; //Stores idle time
    std::stringstream   sss; //Stores signon time
    std::string         chans; //Stores user's channel list and prefixes
    std::string         modes = "+"; //Stores user's modes
    
    if (!(tmp = find_user_by_nickname(arg)))
        return send_message(create_msg(ERR_NOSUCHNICK, u, arg, "", "", ""), u, ERR_NOSUCHNICK);
    send_message(create_msg(RPL_WHOISUSER, u, tmp->nickname, tmp->username, tmp->hostname, tmp->realname), u, RPL_WHOISUSER);
    if (tmp->modes[OPERATOR_MODE])
        send_message(create_msg(RPL_WHOISOPERATOR, u, arg, "", "", "") ,u, RPL_WHOISOPERATOR);
    time(&t);
    ss << t - tmp->idle;
    sss << tmp->signon;
    send_message(create_msg(RPL_WHOISIDLE, u, arg, ss.str(), sss.str(), ""), u, RPL_WHOISIDLE);
    
    for (std::vector<channel *>::iterator it = tmp->channels.begin(); it != tmp->channels.end(); it++)
    {
        std::string prefix = "+";

        if ((*it)->creator == tmp)
            prefix += "q";
        else if (is_chan_ope((*it), tmp))
            prefix += "@";
        else if (is_chan_voice((*it), tmp))
            prefix += "v";
        chans += (prefix + (*it)->name);
    }
    send_message(create_msg(RPL_WHOISCHANNELS, u, arg, chans, "", ""), u, RPL_WHOISCHANNELS);
    if (tmp->modes[AWAY_MODE])
        modes += "a";
    if (tmp->modes[INVISIBLE_MODE])
        modes += "i";
    if (tmp->modes[WALLOPS_MODE])
        modes += "w";
    if (tmp->modes[RESTRICTED_MODE])
        modes += "r";
    if (tmp->modes[OPERATOR_MODE])
        modes += "o";
    send_message(create_msg(RPL_WHOISMODES, u, arg, modes, "", ""), u, RPL_WHOISMODES);
    return send_message(create_msg(RPL_ENDOFWHOIS, u, arg, "", "", ""), u, RPL_ENDOFWHOIS);
}