/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/13 14:20:32 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"
#include <fstream>

/**
 * * @brief Gives operator role to user if username / password are correct
 * 
 * @param username 
 * @param password 
 * @param user 
 * @return RPL_YOUREOPER on success 
 */
int OPER(const std::string &username, const std::string &password, user *u)
{
	
	std::ifstream infile("conf");
	std::string a, b;

	if (username.empty() || password.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "OPER", "", "", ""), u, ERR_NEEDMOREPARAMS);
	if (infile.is_open() == 0)
		return -1;
	while (infile >> a >> b)
	{
	    if (a == std::string(username) && b == std::string(password))
		{
			u->modes[OPERATOR_MODE] = TRUE;
			return send_message(create_msg(RPL_YOUREOPER, u,"", "", "", ""), u , RPL_YOUREOPER);
		}
	}
	return send_message(create_msg(ERR_PASSWDMISMATCH, u, "", "", "", ""), u, ERR_PASSWDMISMATCH);
}