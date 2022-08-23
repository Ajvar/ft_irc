/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 13:33:22 by jcueille          #+#    #+#             */
/*   Updated: 2022/08/23 15:07:48 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"
#include "../includes/replies.hpp"

extern struct s_user *users;
extern struct s_channel *channels;

std::string current_time(void)
{
	/*time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	return std::string(asctime(timeinfo));
	*/

	  time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (buffer,80,"%a %b %d %T",timeinfo);
	return std::string(buffer);
}

/**
 * @brief Creates a message that will be sent back to users on a channel
 * 
 * @param msg 
 * @param u 
 * @return std::string 
 */
std::string channel_message(const std::string &msg, user *u)
{
	std::string	full_msg;

	full_msg = ":" + u->nickname + "!" + u->username +
	 			"@" + u->hostname + " " + msg + "\r\n";
	return full_msg;
}

/**
 * @brief Sends message to client
 * 
 * @param s message to be sent
 * @param user user to send message to
 * @param ret return value
 * @return int 
 */
int send_message(const std::string &s, user *user, int ret)
{
	std::cout << "Send message: " << s << std::endl;
	if (s.empty() == 0)
		send(user->fd->fd, s.c_str(), s.size(), MSG_NOSIGNAL);
	return ret;
}

/**
 * @brief Find user by it's fd inside the global users linked list
 * 
 * @param fd 
 * @return user* 
 */
user *find_user_by_fd(int fd)
{
	user *tmp = users;
	if (!tmp)
		return NULL;
	while (tmp)
	{
		if (tmp->fd->fd == fd)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}

/**
 * @brief Find user by name in the global user linked list
 * 
 * @param nickname user to find
 * @return user* 
 */
user *find_user_by_nickname(const std::string &nickname)
{
	user *tmp = users;

	if (!tmp)
		return NULL;
	while (tmp)
	{
		if (nickname == tmp->nickname)
			return tmp;
		tmp = tmp->next;	
	}
	return NULL;
}

/**
 * @brief Find a channel by it's name
 * 
 * @param name channel to find
 * @return channel* 
 */
channel *find_channel_by_name(const std::string &name)
{
	channel *tmp = channels;

	if (!tmp)
		return NULL;
	while (tmp)
	{
		if (name == tmp->name)
			return tmp;
		tmp = tmp->next;	
	}
	return NULL;
}

/**
 * @brief Reduce array size after suppressing a socket
 * 
 * @param fds the array of socket descriptors
 * @param nfds the number of socket descriptors inside fds
 */
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

/**
 * @brief Converts int into std::string
 * 
 * @param value the value to convert
 * @return std::string 
 */
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

/**
 * @brief Check if user is joined to the channel
 * 
 * @param nickname name of the user to find
 * @param c channel to check
 * @return user* 
 */
user *find_u_in_chan(const std::string &nickname, const channel *c)
{
	std::vector<user *>::const_iterator it = c->users.begin();
	std::vector<user *>::const_iterator ite = c->users.end();
	for (; it != ite; it++)
	{
		if ((*it)->nickname == nickname)
			return (*it);
	}
	return NULL;
}

/**
 * @brief Search channel inside user's channel list
 * 
 * @param name name of the channel to search
 * @param u user to search in
 * @return channel* 
 */
channel *find_chan_in_u(const std::string &name, user *u)
{
	std::vector<channel *>::iterator it = u->channels.begin();
	std::vector<channel *>::iterator ite = u->channels.end();
	for (; it != ite; it++)
	{
		if ((*it)->name == name)
			return (*it);
	}
	return NULL;
}

/**
 * @brief Delete channel from user's channel list
 * 
 * @param name name of channel to delete
 * @param u user to suppress channel from
 */
void delete_chan_in_u(const std::string &name, user *u)
{
	std::vector<channel *>::iterator it = u->channels.begin();
	std::vector<channel *>::iterator ite = u->channels.end();
	for (; it != ite; it++)
		if ((*it)->name == name)
			u->channels.erase(it);
}

/**
 * @brief Deletes user from channel
 * 
 * @param nickname user to delete from channel 
 * @param c channel to delete user from
 */
void delete_u_in_chan(const std::string &nickname, channel *c)
{
	std::vector<user *>::iterator it = c->users.begin();
	std::vector<user *>::iterator ite = c->users.end();
	for (; it != ite; it++)
		if ((*it)->nickname == nickname)
			c->users.erase(it);
}

/**
 * @brief Check if user u is operator on channel c
 * 
 * @param c the channel
 * @param u the user
 * @return int 
 */
int is_chan_ope(const channel *c, const user *u)
{
	std::vector<user *>::const_iterator it = c->operators.begin();
	for( ; it != c->operators.end(); it++)
		if (u == (*it))
			return TRUE;
	return FALSE;
}

void remove_chan_ope(channel *c, user *u)
{
	std::vector<user *>::iterator it = c->operators.begin();
	for( ; it != c->operators.end(); it++)
		if (u == (*it))
		{
			c->operators.erase(it);
			return;
		}
}

int is_chan_voice(const channel* c, const user *u)
{
	std::vector<user *>::const_iterator it = c->voice.begin();
	for( ; it != c->voice.end(); it++)
		if (u == (*it))
			return TRUE;
	return FALSE;	
}

int is_banned(const channel *c, const std::string &nickname)
{
	std::vector<std::pair<std::string, std::string> >::const_iterator ban = c->banned.begin();
	for (; ban != c->banned.end(); ban++)
	{
		if ((*ban).first == nickname)
			return TRUE;
	}
	return FALSE;
}

void unban(channel *c, const std::string &nickname)
{
	std::vector<std::pair<std::string, std::string> >::iterator ban = c->banned.begin();
	for (; ban != c->banned.end(); ban++)
	{
		if ((*ban).first == nickname)
		{
			c->banned.erase(ban);
			return ;
		}
	}
}

void		send_to_all_serv(const std::string &s)
{
	user *tmp = users;
	
	for (; tmp ; tmp = tmp->next)
		send_message(s, tmp, 0);
}

void send_to_all_chan(const std::string &s, const channel *c)
{
	for (std::vector<struct s_user *>::const_iterator it = c->users.begin(); it != c->users.end(); it++)
		send_message(s, (*it), 0);
}

void list_chan_users(channel *c, user *u)
{
	std::string nicks;

	for (std::vector<user *>::iterator uz = c->users.begin(); uz !=  c->users.end(); uz++)
		nicks = nicks + (nicks == "" ? "" : " ") + (is_chan_ope(c, (*uz)) ? "@" : "") + (*uz)->nickname;
	send_to_all_chan(create_msg(RPL_NAMREPLY, u, c->modes[SECRET_MODE] ? "@ " : "= " + c->name, nicks, "", ""), c);
	send_to_all_chan(create_msg(RPL_ENDOFNAMES, u, c->name, "", "", ""), c);
}

std::string concatenate_vector(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end)
{
	std::string res;

	for (; start != end; start++)
		res += (res.empty() ? "" : " ") + (*start);
	return res;
}

/**
 * @brief Create a message string according to numeric reply
 * 
 * @param code the numeric reply
 * @param u 
 * @param arg1 
 * @param arg2 
 * @param arg3 
 * @param arg4 
 * @return std::string 
 */
std::string create_msg(int code, user *u, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	std::string code_str;
	std::string reply;
	
	if (code < 10)
		code_str.append("00");
	else if (code < 100)
		code_str.append("0");
	reply = ":42irc.com " + code_str + ft_to_string(code) + " " + u->nickname + " ";
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
			return reply + MSG_RPL_BANLIST(arg1, arg2, arg3);
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
		case 464:
			return reply + MSG_ERR_PASSWDMISMATCH();
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

