/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/07/13 14:24:04 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"
#include "../../includes/replies.hpp"

/**
 * * @brief  Compares server password with user provided password
 * 
 * @param server_password 
 * @param user_password 
 * @param user 
 */
int PASS(const std::string &server_password, const std::string &user_password, user *u)
{
	std::cout << "server pass : "<< server_password << " user pass :" << user_password << std::endl;
	if (user_password.compare(server_password) != 0)
		return send_message(create_msg(ERR_PASSWDMISMATCH, u, "", "", "", ""), u, ERR_PASSWDMISMATCH);
	return 0;
}
