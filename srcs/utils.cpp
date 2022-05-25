/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 13:33:22 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/25 17:43:38 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"

extern struct s_user *users;
extern struct s_channel *channels;

/*****************************************************/
/* Sends message to user's client		             */
/*****************************************************/
int send_message(std::string s, user *user, int ret)
{
	if (s.empty() == 0)
		send(user->fd->fd, s.c_str(), s.size() + 1, MSG_NOSIGNAL);
	return ret;
}

/*****************************************************/
/* Find the user by its fd				             */
/*****************************************************/
user *find_user_by_fd(int fd)
{
	user *tmp = users;

	if (!tmp)
		return NULL;
	while (tmp)
	{
		if (tmp->fd->fd == fd)
			return tmp;
	}
	return NULL;
}

user *find_user_by_nickname(std::string nickname)
{
	user *tmp = users;

	if (!tmp)
		return NULL;
	while (tmp)
	{
		if (std::string(nickname) == tmp->nickname)
			return tmp;
		tmp = tmp->next;	
	}
	return NULL;
}

void compress_array(pollfd *fds, int *nfds)
{
	int j = 0;
	for (int i = 0; i < (*nfds); i++)
	{
		if (fds[i].fd == -1)
		{
			for(j = i; j < (*nfds)-1; j++)
				fds[j].fd = fds[j+1].fd;
			i--;
			(*nfds)--;
		}
	}
}

std::string		ft_to_string(int value)
{
	std::string output;
	std::string sign;
	char		nb[2];

	if (value < 0)
	{
		sign + "-";
		value = -value;
	}
	nb[1] = '\0';
	while (output.empty() || (value > 0))
	{
		nb[0] = value % 10 + '0';
		output.insert(0, std::string(nb));
		value /= 10;
	}

	return (sign + output);
}

std::string create_msg(std::string msg, int code, user *u)
{
	std::string code_str;
	std::string reply;
	
	(void)msg;
	if (code < 10)
		code_str.append("00");
	else if (code < 100)
		code_str.append("0");
	reply = ":127.0.0.1 " + code_str + ft_to_string(code) + " " + u->nickname + " ";
	switch (code)
	{
		case 1:
			return reply + MSG_RPL_WELCOME(u->nickname);
		case 2:
			return reply + MSG_RPL_YOURHOST(u->nickname, u->username);
		case 3:
			return reply + MSG_RPL_CREATED(u->nickname);
		case 4:
			return reply + MSG_RPL_MYINFO(u->nickname, u->username, u->hostname, u->realname);
		case 5:
			return reply + MSG_RPL_BOUNCE(u->nickname, u->username);
		case 200:
			return reply + MSG_RPL_TRACELINK(u->nickname, u->username);
		case 203:
			return reply + MSG_RPL_TRACEUNKNOWN(u->nickname);
		case 204:
			return reply + MSG_RPL_TRACEOPERATOR(u->nickname);
		case 205:
			return reply + MSG_RPL_TRACEUSER(u->nickname);
		case 206:
			return reply + MSG_RPL_TRACESERVER(u->nickname);
		case 211:
			return reply + MSG_RPL_STATSLINKINFO(u->nickname);
		case 212:
			return reply + MSG_RPL_STATSCOMMANDS(u->nickname);
		case 219:
			return reply + MSG_RPL_ENDOFSTATS(u->nickname);
		case 221:
			return reply + MSG_RPL_UMODEIS(u->nickname);
		case 242:
			return reply + MSG_RPL_STATSUPTIME(u->nickname);
		case 251:
			return reply + MSG_RPL_LUSERCLIENT(u->nickname, u->username, u->hostname);
		case 252:
			return reply + MSG_RPL_LUSEROP(u->nickname);
		case 253:
			return reply + MSG_RPL_LUSERUNKNOWN(u->nickname);
		case 254:
			return reply + MSG_RPL_LUSERCHANNELS(u->nickname);
		case 255:
			return reply + MSG_RPL_LUSERME(u->nickname, u->username);
		case 256:
			return reply + MSG_RPL_ADMINME(u->nickname);
		case 257:
			return reply + MSG_RPL_ADMINLOC1(u->nickname);
		case 258:
			return reply + MSG_RPL_ADMINLOC2(u->nickname);
		case 259:
			return reply + MSG_RPL_ADMINEMAIL(u->nickname);
		case 262:
			return reply + MSG_RPL_TRACEEND(u->nickname, u->username);
		case 301:
			return reply + MSG_RPL_AWAY(u->nickname, u->username);
		case 302:
			return reply + MSG_RPL_USERHOST(u->nickname);
		case 303:
			return reply + MSG_RPL_ISON(u->nickname);
		case 305:
			return reply + MSG_RPL_UNAWAY();
		case 306:
			return reply + MSG_RPL_NOWAWAY();
		case 311:
			return reply + MSG_RPL_WHOISUSER(u->nickname, u->username, u->hostname, u->realname);
		case 313:
			return reply + MSG_RPL_WHOISOPERATOR(u->nickname);
		case 314:
			return reply + MSG_RPL_WHOWASUSER(u->nickname, u->username, u->hostname, u->realname);
		case 315:
			return reply + MSG_RPL_ENDOFWHO(u->nickname);
		case 317:
			return reply + MSG_RPL_WHOISIDLE(u->nickname, u->username, u->hostname);
		case 318:
			return reply + MSG_RPL_ENDOFWHOIS(u->nickname);
		case 319:
			return reply + MSG_RPL_WHOISCHANNELS(u->nickname, u->username);
		case 322:
			return reply + MSG_RPL_LIST(u->nickname, u->username);
		case 323:
			return reply + MSG_RPL_LISTEND();
		case 324:
			return reply + MSG_RPL_CHANNELMODEIS(u->nickname, u->username, u->hostname);
		case 329:
			return reply + MSG_RPL_CREATIONTIME(u->nickname, u->username);
		case 331:
			return reply + MSG_ERR_NOTOPIC(u->nickname);
		case 332:
			return reply + MSG_RPL_TOPIC(u->nickname, u->username);
		case 341:
			return reply + MSG_RPL_INVITING(u->nickname, u->username);
		case 351:
			return reply + MSG_RPL_VERSION(u->nickname, u->username, u->hostname, u->realname);
		case 352:
			return reply + MSG_RPL_WHOREPLY(u->nickname);
		case 353:
			return reply + MSG_RPL_NAMREPLY(u->nickname, u->username);
		case 364:
			return reply + MSG_RPL_LINKS(u->nickname, u->username, u->hostname, u->realname);
		case 365:
			return reply + MSG_RPL_ENDOFLINKS(u->nickname);
		case 366:
			return reply + MSG_RPL_ENDOFNAMES(u->nickname);
		case 367:
			return reply + MSG_RPL_BANLIST(u->nickname);
		case 368:
			return reply + MSG_RPL_ENDOFBANLIST(u->nickname);
		case 369:
			return reply + MSG_RPL_ENDOFWHOWAS(u->nickname);
		case 371:
			return reply + MSG_RPL_INFO(u->nickname);
		case 372:
			return reply + MSG_RPL_MOTD(u->nickname);
		case 374:
			return reply + MSG_RPL_ENDOFINFO();
		case 375:
			return reply + MSG_RPL_MOTDSTART(u->nickname);
		case 376:
			return reply + MSG_RPL_ENDOFMOTD();
		case 381:
			return reply + MSG_RPL_YOUREOPER();
		case 382:
			return reply + MSG_RPL_REHASHING(u->nickname);
		case 391:
			return reply + MSG_RPL_TIME(u->nickname, u->username);
		case 401:
			return reply + MSG_ERR_NOSUCHNICK(u->nickname);
		case 402:
			return reply + MSG_ERR_NOSUCHSERVER(u->nickname);
		case 403:
			return reply + MSG_ERR_NOSUCHCHANNEL(u->nickname);
		case 404:
			return reply + MSG_ERR_CANNOTSENDTOCHAN(u->nickname);
		case 406:
			return reply + MSG_ERR_WASNOSUCHNICK(u->nickname);
		case 409:
			return reply + MSG_ERR_NOORIGIN();
		case 410:
			return reply + MSG_ERR_INVALIDCAP(u->nickname);
		case 411:
			return reply + MSG_ERR_NORECIPIENT(u->nickname);
		case 412:
			return reply + MSG_ERR_NOTEXTTOSEND();
		case 421:
			return reply + MSG_ERR_UNKNOWNCOMMAND(u->nickname);
		case 422:
			return reply + MSG_ERR_NOMOTD();
		case 423:
			return reply + MSG_ERR_NOADMININFO(u->nickname);
		case 424:
			return reply + MSG_ERR_FILEERROR(u->nickname, u->username);
		case 431:
			return reply + MSG_ERR_NONICKNAMEGIVEN();
		case 432:
			return reply + MSG_ERR_ERRONEUSNICKNAME(u->nickname);
		case 433:
			return reply + MSG_ERR_NICKNAMEINUSE(u->nickname);
		case 436:
			return reply + MSG_ERR_NICKCOLLISION(u->nickname, u->username, u->hostname);
		case 441:
			return reply + MSG_ERR_USERNOTINCHANNEL(u->nickname, u->username);
		case 442:
			return reply + MSG_ERR_NOTONCHANNEL(u->nickname);
		case 443:
			return reply + MSG_ERR_USERONCHANNEL(u->nickname, u->username);
		case 451:
			return reply + MSG_ERR_NOTREGISTERED();
		case 461:
			return reply + MSG_ERR_NEEDMOREPARAMS(u->nickname);
		case 462:
			return reply + MSG_ERR_ALREADYREGISTRED();
		case 471:
			return reply + MSG_ERR_CHANNELISFULL(u->nickname);
		case 472:
			return reply + MSG_ERR_UNKNOWNMODE(u->nickname);
		case 473:
			return reply + MSG_ERR_INVITEONLYCHAN(u->nickname);
		case 474:
			return reply + MSG_ERR_BANNEDFROMCHAN(u->nickname);
		case 475:
			return reply + MSG_ERR_BADCHANNELKEY(u->nickname);
		case 476:
			return reply + MSG_ERR_BADCHANMASK(u->nickname);
		case 481:
			return reply + MSG_ERR_NOPRIVILEGES();
		case 482:
			return reply + MSG_ERR_CHANOPRIVSNEEDED(u->nickname);
		case 483:
			return reply + MSG_ERR_CANTKILLSERVER();
		case 491:
			return reply + MSG_ERR_NOOPERHOST();
		case 501:
			return reply + MSG_ERR_UMODEUNKNOWNFLAG();
		case 502:
			return reply + MSG_ERR_USERSDONTMATCH();
		case 691:
			return reply + MSG_ERR_ALREADYBAN(u->nickname, u->username);
		case 692:
			return reply + MSG_ERR_KILLDENY(u->nickname);
		default:
			return std::string("");
	}
}

