/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/13 14:17:14 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"
extern user *users;

/**
 * * @brief Gives nickname to user or changes nickname from existing user
 * 
 * @param nickname 
 * @param user 
 * @return int 
 */
int NICK(const std::string &nickname, user *user)
{
	s_user *tmp = users;
	std::string buff;
	
	// pp("IN NICK", RED);

	if (nickname.empty())
		return send_message(create_msg(ERR_NONICKNAMEGIVEN, user,"", "", "", ""), user, ERR_NONICKNAMEGIVEN);

	while (tmp)
	{
		if (tmp->nickname == nickname && tmp->id != user->id)
			return send_message(create_msg(ERR_NICKNAMEINUSE, user, nickname, "", "", ""), user, ERR_NICKNAMEINUSE);
		tmp = tmp->next;
	}
	if (user->nickname.empty() == false)
		send_to_all_serv(":" + user->nickname + " NICK " + nickname);
	user->nickname = nickname;
	return 0;
}