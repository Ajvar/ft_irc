/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 12:23:57 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/21 12:25:41 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

void print_user(user *user)
{
	if (user)
	{
		std::cout << "nickname: " << user->next << " username: " << user->username << " realname: " << user->realname << std::endl;
	}
}