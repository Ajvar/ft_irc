/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/17 15:48:37 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "replies.hpp"

int pass(const char *server_password, const char *user_password)
{
	if (!user_password)
		return ERR_NEEDMOREPARAMS;
	
	if (strcmp(user_password, server_password) != 0)
		return 1;
	return 0;
}