/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/06/21 12:46:06 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"
#include <fstream>

extern user *users;
extern channel *channels;

/**
 * * @brief  Compares server password with user provided password
 * 
 * @param server_password 
 * @param user_password 
 * @param user 
 * @return 0 on success, 1 on failure. 
 */
int PASS(const std::string &server_password, const std::string &user_password, user *user)
{
	if (user_password.empty())
	{
		std::cout << "moreparam"<< user_password << std::endl;
		return ERR_NEEDMOREPARAMS;
	}
	std::cout << "server pass : "<< server_password << " user pass :" << user_password << std::endl;
	if (user_password.compare(server_password))
	{
		send(user->fd->fd, "Wrong password", 16, 0);
		return 1;
	}
	return 0;
}

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
	
	std::cout << "NIIIIIICK" << std::endl;

	if (nickname.empty())
		return send_message(create_msg(ERR_NONICKNAMEGIVEN, user,"", "", "", ""), user, ERR_NONICKNAMEGIVEN);

	while (tmp)
	{
		if (tmp->nickname == nickname && tmp->id != user->id)
			return send_message("ERR_NICKNAMEINUSE", user, ERR_NICKNAMEINUSE);
		tmp = tmp->next;
	}
	if (user->nickname.empty() == false)
		send(user->fd->fd, buff.c_str(), buff.size(), 0);
	user->nickname = nickname;
	return 0;
}

/**
 * * @brief Gives username and real name to new user	
 * 
 * @param username 
 * @param realname 
 * @param user 
 * @return int 
 */
int USER(const std::string &username, const std::string &realname, user *user)
{
	struct s_user *tmp = users;

	while (tmp)
	{
		if (tmp->username == username)
			return send_message("ERR_ALREADYREGISTRED", user, ERR_ALREADYREGISTRED);
		tmp = tmp->next;
	}
	user->username = username;
	user->realname = realname;
	return 0;
}

/**
 * * @brief Gives operator role to user if nicname / password are correct
 * 
 * @param username 
 * @param password 
 * @param user 
 * @return RPL_YOUREOPER on success 
 */
int OPER(const std::string &username, const std::string &password, user *user)
{
	std::cout << "Inside oper sir user nick: " << user->nickname << std::endl;
	
	std::ifstream infile("conf");
	std::string a, b;

	if (username.empty() || password.empty())
		return send_message("Error: Too few parameters.\nUsage OPER nickname password.", user, ERR_NEEDMOREPARAMS);
	if (infile.is_open() == 0)
		return -1;
	while (infile >> a >> b)
	{
	    if (a == std::string(username) && b == std::string(password))
		{
			user->modes[OPERATOR_MODE] = TRUE;
			return send_message(create_msg(RPL_YOUREOPER, user,"", "", "", ""), user , RPL_YOUREOPER);
		}
	}
	return send_message("ERR_PASSWDMISMATCH", user, ERR_PASSWDMISMATCH);
}

/**
 * * @brief Changes user modes
 * 
 * @param nickname the target's nickname
 * @param sign + or -
 * @param mode a, i, w, r, o
 * @param user 
 * @return 0 on success 
 */
int MODE(const std::string &nickname, char sign, char mode, user *user)
{
	short local_sign;

	sign == '+' ? local_sign = 1 : local_sign = 0;
	if (nickname.empty() || !sign || !mode)
		return send_message("ERR_NEEDMOREPARAMS", user, ERR_NEEDMOREPARAMS);
	if (std::string(nickname) != user->nickname)
		return send_message("ERR_USERSDONTMATCH", user, ERR_USERSDONTMATCH);
	switch (mode)
	{
	case 'i' :
		user->modes[INVISIBLE_MODE] = local_sign;
		break;
	case 'w' :
		user->modes[WALLOPS_MODE] = local_sign;
		break;
	case 'r':
		if (local_sign)
			return 0;
		user->modes[RESTRICTED_MODE] = local_sign;
		break;
	case 'o':
		if (local_sign)
			return 0;
		user->modes[OPERATOR_MODE] = local_sign;
		break;
	
	default:
		return send_message("ERR_UMODEUNKNOWNFLAG", user, ERR_UMODEUNKNOWNFLAG);
		break;
	}
	return 0;
}

/**
 * * @brief Closes user's connexion
 * ! Are we supposed to send the msg to every other users ?
 * @param msg
 * @param fds
 * @param nfds
 * @param u
 * @return int
 */
int QUIT(const std::string &msg, pollfd *fds, int *nfds, user *u)
{
	send_message(msg, u, 0);
	close(u->fd->fd);
	u->fd->fd = -1;
	compress_array(fds, nfds);
	return 0;
}

/**
 * * @brief Sets / unset AWAY_MODE on user and stores automatic
 * away reply
 * 
 * @param away_msg away message to send back to user sending PRIVMSG
 * @param user 
 * @return RPL_NOWAYWAY or RPL_UNAWAY on success, -1 on failure 
 */
int AWAY(const std::string &away_msg, user *user)
{
	std::cout << "in away" << std::endl;
	if (user && user->modes[AWAY_MODE] == 0)
	{
		user->modes[AWAY_MODE] = 1;
		user->away_msg = std::string(away_msg);
		send_message(create_msg(RPL_NOWAWAY, user,"", "", "", ""), user, RPL_NOWAWAY);
	}
	if (user && user->modes[AWAY_MODE] == 1)
	{
		user->modes[AWAY_MODE] = 0;
		user->away_msg = "";
		return send_message(create_msg(RPL_UNAWAY, user,"", "", "", ""), user, RPL_UNAWAY);
	}
	return -1;
}

/**
 * * @brief Terminates the server
 * 
 * @param user 
 * @param fds 
 * @param nfds 
 * @return ERR_NOPRIVILEGES on failure
 */
int	DIE(user *user, pollfd *fds, int nfds)
{
	if (user->modes[OPERATOR_MODE] == 0)
		return send_message(create_msg(ERR_NOPRIVILEGES, user,"", "", "", ""), user, ERR_NOPRIVILEGES);
	send_message("Killing server.", user, 0);
	ft_free_exit("Killing server", 0, fds, nfds);
	return 0;
}


/**
 * * @brief Restarts server
 * 
 * @param user 
 * @param fds 
 * @param nfds 
 * @param restart pointer to restart from main
 * @return 0 on success, ERR_NOPRIVILEGES on failure.
 */
int RESTART(user *user, pollfd *fds, int nfds, int *restart)
{
	if (user->modes[OPERATOR_MODE] == 0)
		return send_message(create_msg(ERR_NOPRIVILEGES, user,"", "", "", ""), user, ERR_NOPRIVILEGES);
	free_users();
	free_channels();
	free_fds(fds, nfds);
	users = NULL;
	channels = NULL;
	(*restart) = 1;
	return 0;
}

/**
 * * @brief Sends message to all users with WALLOPS_MODE activated.
 * 
 * @param msg message to send
 * @param u the user sending the message
 * @return 0 on success, ERR_NEEDMOREPARAMS is msg is empty
 */
int WALLOPS(const std::string &msg, user *u)
{
	user *tmp = users;
	if (msg.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, u, "WALLOPS","", "", ""), u, ERR_NEEDMOREPARAMS);
	while (tmp)
	{
		if (tmp != u && tmp->modes[WALLOPS_MODE] == 1)
			send_message(msg, u, 0);
		tmp = tmp->next;
	}
	return 0;
}


/**
 * * @brief Take a list of nicknames
 * * and send back a list of those who are connected on the server
 * 
 * @param nicknames 
 * @param user 
 * @return RPL_ISON on success 
 */
int ISON(std::vector<std::string > nicknames, user *user)
{
	std::string ret = NULL;

	if (nicknames.empty())
		return send_message(create_msg(ERR_NEEDMOREPARAMS, user, "ISON","", "", ""), user, ERR_NEEDMOREPARAMS);
	for (std::vector<std::string >::iterator it = nicknames.begin(); it != nicknames.end(); it++)
	{
		if (find_user_by_nickname((*it)))
			ret = ret + (*it);
	}
	return send_message(ret, user, RPL_ISON);
}

