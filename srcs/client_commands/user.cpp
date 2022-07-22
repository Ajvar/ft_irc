/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/22 17:28:50 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

extern user *users;

/**
 * * @brief Gives username and real name to new user	
 * 
 * @param username 
 * @param realname 
 * @param user 
 * @return int 
 */
int USER(const std::string &username, const std::string &realname, user *u)
{
	struct s_user *tmp = users;
	
	// pp("IN USER", RED);
	while (tmp)
	{
		if (tmp->username == username)
			return send_message(create_msg(ERR_ALREADYREGISTRED, u, "", "", "", ""), u, ERR_ALREADYREGISTRED);
		tmp = tmp->next;
	}
	u->username = username;
	u->realname = realname;
	send_message(create_msg(RPL_WELCOME, u, u->nickname, "", "", ""), u, 0);
	send_message(create_msg(RPL_YOURHOST, u, "42irc.com", VERSION, "", ""), u, 0);
	send_message(create_msg(RPL_CREATED, u, "today", "", "", ""), u, 0);
	send_message(create_msg(RPL_MYINFO, u, SERVER_NAME, VERSION, "aiwroO", "blikmstn"), u, 0);
	send_message(create_msg(RPL_ENDOFMOTD, u, "", "", "", ""), u, RPL_ENDOFMOTD);
	return 0;
}