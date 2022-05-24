/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 15:35:51 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/24 20:55:28 by jcueille         ###   ########.fr       */
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
int PASS(const char *server_password, const char *user_password, user *user)
{
	if (!user_password)
		return ERR_NEEDMOREPARAMS;
	std::cout << "server pass : "<< server_password << " user pass :" << user_password << std::endl;
	if (strcmp(user_password, server_password) != 0)
	{
		send(user->fd->fd, "Wrong password", 16, 0);
		return 1;
	}
	return 0;
}

/**
 * * @brief Gives nickname to new user or changes nickname from existing user
 * 
 * @param nickname 
 * @param user 
 * @return int 
 */
int NICK(const char *nickname, user *user)
{
	s_user *tmp = users;
	std::string buff;
	
	if (!(nickname))
		return send_message((char *)"ERR_NONICKNAMEGIVEN", user, ERR_NONICKNAMEGIVEN);

	buff = "You are now known as " + std::string(nickname);
	while (tmp)
	{
		if (tmp->nickname == nickname && tmp->id != user->id)
			return send_message((char *)"ERR_NICKNAMEINUSE", user, ERR_NICKNAMEINUSE);

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
int USER(char *username, char* realname, user *user)
{
	struct s_user *tmp = users;

	while (tmp)
	{
		if (tmp->username == username)
			return send_message((char *)"ERR_ALREADYREGISTRED", user, ERR_ALREADYREGISTRED);
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
int OPER(char *username, char *password, user *user)
{
	
	std::ifstream infile("conf");
	std::string a, b;

	if (!username || !password)
		return send_message((char*)"Error: Too few parameters.\nUsage OPER nickname password.", user, ERR_NEEDMOREPARAMS);
	if (infile.is_open() == 0)
		return -1;
	while (infile >> a >> b)
	{
	    if (a == std::string(username) && b == std::string(password))
		{
			user->modes[OPERATOR_MODE] = TRUE;
			return send_message((char*)"RPL_YOUREOPER", user , RPL_YOUREOPER);
		}
	}
	return send_message((char*)"ERR_PASSWDMISMATCH", user, ERR_PASSWDMISMATCH);
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
int MODE(char *nickname, char sign, char mode, user *user)
{
	short local_sign;

	sign == '+' ? local_sign = 1 : local_sign = 0;
	if (!nickname || !sign || !mode)
		return send_message((char *)"ERR_NEEDMOREPARAMS", user, ERR_NEEDMOREPARAMS);
	if (std::string(nickname) != user->nickname)
		return send_message((char *)"ERR_USERSDONTMATCH", user, ERR_USERSDONTMATCH);
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
		return send_message((char *)"ERR_UMODEUNKNOWNFLAG", user, ERR_UMODEUNKNOWNFLAG);
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
int QUIT(char *msg, pollfd *fds, int *nfds, user *u)
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
int AWAY(char *away_msg, user *user)
{
	if (user && user->modes[AWAY_MODE] == 0)
	{
		user->modes[AWAY_MODE] = 1;
		user->away_msg = std::string(away_msg);
		send_message((char*)"RPL_NOWAYWAY", user, RPL_NOWAWAY);
	}
	if (user && user->modes[AWAY_MODE] == 1)
	{
		user->modes[AWAY_MODE] = 0;
		user->away_msg = "";
		return send_message((char*)"RPL_UNAWAY", user, RPL_UNAWAY);
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
		return send_message((char*)"ERR_NOPRIVILEGES", user, ERR_NOPRIVILEGES);
	send_message((char*)"Killing server.", user, 0);
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
		return send_message((char*)"ERR_NOPRIVILEGES", user, ERR_NOPRIVILEGES);
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
int WALLOPS(char *msg, user *u)
{
	user *tmp = users;
	if (!msg)
		return send_message((char*)"ERR_NEEDMOREPARAMS", u, ERR_NEEDMOREPARAMS);
	while (tmp)
	{
		if (tmp != u && tmp->modes[WALLOPS_MODE] == 1)
		{
			send_message(msg, u, 0);
		}
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
int ISON(std::vector<char *> nicknames, user *user)
{
	char *ret = NULL;

	if (nicknames.empty())
		return send_message((char *)"ERR_NEEDMOREPARAMS", user, ERR_NEEDMOREPARAMS);
	for (std::vector<char *>::iterator it = nicknames.begin(); it != nicknames.end(); it++)
	{
		if (find_user_by_nickname((*it)))
			ret = strcat(ret, (*it));
	}
	return send_message(ret, user, RPL_ISON);
}