/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 13:33:22 by jcueille          #+#    #+#             */
/*   Updated: 2022/06/02 20:59:06 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"

extern struct s_user *users;
extern struct s_channel *channels;

/*****************************************************/
/* Sends message to user's client		             */
/*****************************************************/
int send_message(const std::string &s, user *user, int ret)
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

user *find_user_by_nickname(const std::string &nickname)
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

channel *find_channel_by_name(const std::string &name)
{
	channel *tmp = channels;

	if (!tmp)
		return NULL;
	while (tmp)
	{
		if (std::string(name) == tmp->name)
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

user *find_u_in_chan(const std::string &nickname, channel *c)
{
	std::vector<user *>::iterator it = c->users.begin();
	std::vector<user *>::iterator ite = c->users.end();
	for (; it != ite; it++)
	{
		if ((*it)->nickname == nickname)
			return (*it);
	}
	return NULL;
}

std::string create_msg(int code, user *u, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	std::string code_str;
	std::string reply;
	
	if (code < 10)
		code_str.append("00");
	else if (code < 100)
		code_str.append("0");
	reply = ":127.0.0.1 " + code_str + ft_to_string(code) + " " + u->nickname + " ";
	switch (code)
	{
		case 1:
			return reply + MSG_RPL_WELCOME(arg1);
		case 2:
			return reply + MSG_RPL_YOURHOST(arg1, arg2);
		case 3:
			return reply + MSG_RPL_CREATED(arg1);
		case 4:
			return reply + MSG_RPL_MYINFO(arg1, arg2, arg3, arg4);
		case 5:
			return reply + MSG_RPL_BOUNCE(arg1, arg2);
		case 200:
			return reply + MSG_RPL_TRACELINK(arg1, arg2);
		case 203:
			return reply + MSG_RPL_TRACEUNKNOWN(arg1);
		case 204:
			return reply + MSG_RPL_TRACEOPERATOR(arg1);
		case 205:
			return reply + MSG_RPL_TRACEUSER(arg1);
		case 206:
			return reply + MSG_RPL_TRACESERVER(arg1);
		case 211:
			return reply + MSG_RPL_STATSLINKINFO(arg1);
		case 212:
			return reply + MSG_RPL_STATSCOMMANDS(arg1);
		case 219:
			return reply + MSG_RPL_ENDOFSTATS(arg1);
		case 221:
			return reply + MSG_RPL_UMODEIS(arg1);
		case 242:
			return reply + MSG_RPL_STATSUPTIME(arg1);
		case 251:
			return reply + MSG_RPL_LUSERCLIENT(arg1, arg2, arg3);
		case 252:
			return reply + MSG_RPL_LUSEROP(arg1);
		case 253:
			return reply + MSG_RPL_LUSERUNKNOWN(arg1);
		case 254:
			return reply + MSG_RPL_LUSERCHANNELS(arg1);
		case 255:
			return reply + MSG_RPL_LUSERME(arg1, arg2);
		case 256:
			return reply + MSG_RPL_ADMINME(arg1);
		case 257:
			return reply + MSG_RPL_ADMINLOC1(arg1);
		case 258:
			return reply + MSG_RPL_ADMINLOC2(arg1);
		case 259:
			return reply + MSG_RPL_ADMINEMAIL(arg1);
		case 262:
			return reply + MSG_RPL_TRACEEND(arg1, arg2);
		case 301:
			return reply + MSG_RPL_AWAY(arg1, arg2);
		case 302:
			return reply + MSG_RPL_USERHOST(arg1);
		case 303:
			return reply + MSG_RPL_ISON(arg1);
		case 305:
			return reply + MSG_RPL_UNAWAY();
		case 306:
			return reply + MSG_RPL_NOWAWAY();
		case 311:
			return reply + MSG_RPL_WHOISUSER(arg1, arg2, arg3, arg4);
		case 313:
			return reply + MSG_RPL_WHOISOPERATOR(arg1);
		case 314:
			return reply + MSG_RPL_WHOWASUSER(arg1, arg2, arg3, arg4);
		case 315:
			return reply + MSG_RPL_ENDOFWHO(arg1);
		case 317:
			return reply + MSG_RPL_WHOISIDLE(arg1, arg2, arg3);
		case 318:
			return reply + MSG_RPL_ENDOFWHOIS(arg1);
		case 319:
			return reply + MSG_RPL_WHOISCHANNELS(arg1, arg2);
		case 322:
			return reply + MSG_RPL_LIST(arg1, arg2);
		case 323:
			return reply + MSG_RPL_LISTEND();
		case 324:
			return reply + MSG_RPL_CHANNELMODEIS(arg1, arg2, arg3);
		case 329:
			return reply + MSG_RPL_CREATIONTIME(arg1, arg2);
		case 331:
			return reply + MSG_ERR_NOTOPIC(arg1);
		case 332:
			return reply + MSG_RPL_TOPIC(arg1, arg2);
		case 341:
			return reply + MSG_RPL_INVITING(arg1, arg2);
		case 351:
			return reply + MSG_RPL_VERSION(arg1, arg2, arg3, arg4);
		case 352:
			return reply + MSG_RPL_WHOREPLY(arg1);
		case 353:
			return reply + MSG_RPL_NAMREPLY(arg1, arg2);
		case 364:
			return reply + MSG_RPL_LINKS(arg1, arg2, arg3, arg4);
		case 365:
			return reply + MSG_RPL_ENDOFLINKS(arg1);
		case 366:
			return reply + MSG_RPL_ENDOFNAMES(arg1);
		case 367:
			return reply + MSG_RPL_BANLIST(arg1);
		case 368:
			return reply + MSG_RPL_ENDOFBANLIST(arg1);
		case 369:
			return reply + MSG_RPL_ENDOFWHOWAS(arg1);
		case 371:
			return reply + MSG_RPL_INFO(arg1);
		case 372:
			return reply + MSG_RPL_MOTD(arg1);
		case 374:
			return reply + MSG_RPL_ENDOFINFO();
		case 375:
			return reply + MSG_RPL_MOTDSTART(arg1);
		case 376:
			return reply + MSG_RPL_ENDOFMOTD();
		case 381:
			return reply + MSG_RPL_YOUREOPER();
		case 382:
			return reply + MSG_RPL_REHASHING(arg1);
		case 391:
			return reply + MSG_RPL_TIME(arg1, arg2);
		case 401:
			return reply + MSG_ERR_NOSUCHNICK(arg1);
		case 402:
			return reply + MSG_ERR_NOSUCHSERVER(arg1);
		case 403:
			return reply + MSG_ERR_NOSUCHCHANNEL(arg1);
		case 404:
			return reply + MSG_ERR_CANNOTSENDTOCHAN(arg1);
		case 406:
			return reply + MSG_ERR_WASNOSUCHNICK(arg1);
		case 409:
			return reply + MSG_ERR_NOORIGIN();
		case 410:
			return reply + MSG_ERR_INVALIDCAP(arg1);
		case 411:
			return reply + MSG_ERR_NORECIPIENT(arg1);
		case 412:
			return reply + MSG_ERR_NOTEXTTOSEND();
		case 421:
			return reply + MSG_ERR_UNKNOWNCOMMAND(arg1);
		case 422:
			return reply + MSG_ERR_NOMOTD();
		case 423:
			return reply + MSG_ERR_NOADMININFO(arg1);
		case 424:
			return reply + MSG_ERR_FILEERROR(arg1, arg2);
		case 431:
			return reply + MSG_ERR_NONICKNAMEGIVEN();
		case 432:
			return reply + MSG_ERR_ERRONEUSNICKNAME(arg1);
		case 433:
			return reply + MSG_ERR_NICKNAMEINUSE(arg1);
		case 436:
			return reply + MSG_ERR_NICKCOLLISION(arg1, arg2, arg3);
		case 441:
			return reply + MSG_ERR_USERNOTINCHANNEL(arg1, arg2);
		case 442:
			return reply + MSG_ERR_NOTONCHANNEL(arg1);
		case 443:
			return reply + MSG_ERR_USERONCHANNEL(arg1, arg2);
		case 451:
			return reply + MSG_ERR_NOTREGISTERED();
		case 461:
			return reply + MSG_ERR_NEEDMOREPARAMS(arg1);
		case 462:
			return reply + MSG_ERR_ALREADYREGISTRED();
		case 471:
			return reply + MSG_ERR_CHANNELISFULL(arg1);
		case 472:
			return reply + MSG_ERR_UNKNOWNMODE(arg1);
		case 473:
			return reply + MSG_ERR_INVITEONLYCHAN(arg1);
		case 474:
			return reply + MSG_ERR_BANNEDFROMCHAN(arg1);
		case 475:
			return reply + MSG_ERR_BADCHANNELKEY(arg1);
		case 476:
			return reply + MSG_ERR_BADCHANMASK(arg1);
		case 481:
			return reply + MSG_ERR_NOPRIVILEGES();
		case 482:
			return reply + MSG_ERR_CHANOPRIVSNEEDED(arg1);
		case 483:
			return reply + MSG_ERR_CANTKILLSERVER();
		case 491:
			return reply + MSG_ERR_NOOPERHOST();
		case 501:
			return reply + MSG_ERR_UMODEUNKNOWNFLAG();
		case 502:
			return reply + MSG_ERR_USERSDONTMATCH();
		case 691:
			return reply + MSG_ERR_ALREADYBAN(arg1, arg2);
		case 692:
			return reply + MSG_ERR_KILLDENY(arg1);
		default:
			return std::string("");
	}
}

