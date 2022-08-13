/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pingpong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 14:03:42 by jcueille          #+#    #+#             */
/*   Updated: 2022/08/13 14:20:47 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"

int PONG(const std::string &token, user *u)
{
	if (token == "")
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "PONG", "", "", ""), u, ERR_NEEDMOREPARAMS);
	return send_message("42IRC" + std::string(" :") + token, u, 0);
}