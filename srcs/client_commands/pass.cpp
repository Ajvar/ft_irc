/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/08/25 13:02:27 by jcueille         ###   ########.fr       */
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
	if (user_password.compare(server_password) != 0)
		return send_message(create_msg(ERR_PASSWDMISMATCH, u, "", "", "", ""), u, ERR_PASSWDMISMATCH);
	u->auth = 1;
	return 0;
}
