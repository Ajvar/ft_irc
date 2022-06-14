/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 12:23:57 by jcueille          #+#    #+#             */
/*   Updated: 2022/06/09 15:02:28 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

extern channel* channels;
extern user* users;

void print_user(user *user)
{
	if (user)
	{
		std::cout << "nickname: " << user->next << " username: " << user->username << " realname: " << user->realname << std::endl;
	}
}

void print_channels()
{
	channel *tmp = channels;
	while (tmp)
	{
		std::cout << "channel name: " <<tmp->name << std::endl;
		tmp = tmp->next;
	}
}