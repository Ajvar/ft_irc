/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 14:02:54 by jcueille          #+#    #+#             */
/*   Updated: 2022/08/15 15:41:31 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

#define	RPL_WELCOME 1
#define	RPL_YOURHOST 2
#define	RPL_CREATED 3
#define	RPL_MYINFO 4
#define	RPL_BOUNCE 5

#define	ERR_NOSUCHNICK 401
#define	ERR_NOSUCHSERVER	402
#define	ERR_NOSUCHCHANNEL	403
#define	ERR_CANNOTSENDTOCHAN	404
#define	ERR_TOOMANYCHANNELS	405
#define	ERR_WASNOSUCHNICK	406
#define	ERR_TOOMANYTARGETS	407
#define	ERR_NOORIGIN	409
#define	ERR_NORECIPIENT	411
#define	ERR_NOTEXTTOSEND	412
#define	ERR_NOTOPLEVEL	413
#define	ERR_WILDTOPLEVEL	414
#define	ERR_UNKNOWNCOMMAND	421
#define	ERR_NOMOTD	422
#define	ERR_NOADMININFO	423
#define	ERR_FILEERROR	424
#define	ERR_NONICKNAMEGIVEN	431
#define	ERR_ERRONEUSNICKNAME	432
#define	ERR_NICKNAMEINUSE	433
#define	ERR_NICKCOLLISION	436
#define	ERR_USERNOTINCHANNEL	441
#define	ERR_NOTONCHANNEL	442
#define	ERR_USERONCHANNEL	443
#define	ERR_NOLOGIN	444
#define	ERR_SUMMONDISABLED	445
#define	ERR_USERSDISABLED	446
#define	ERR_NOTREGISTERED	451
#define	ERR_NEEDMOREPARAMS	461
#define	ERR_ALREADYREGISTRED	462
#define	ERR_NOPERMFORHOST	463
#define	ERR_PASSWDMISMATCH	464
#define	ERR_YOUREBANNEDCREEP	465
#define	ERR_KEYSET	467
#define	ERR_CHANNELISFULL	471
#define	ERR_UNKNOWNMODE	472
#define	ERR_INVITEONLYCHAN	473
#define	ERR_BANNEDFROMCHAN	474
#define	ERR_BADCHANNELKEY	475
#define	ERR_NOPRIVILEGES	481
#define	ERR_CHANOPRIVSNEEDED	482
#define	ERR_CANTKILLSERVER	483
#define	ERR_NOOPERHOST	491
#define	ERR_UMODEUNKNOWNFLAG	501
#define	ERR_USERSDONTMATCH	502

#define	RPL_NONE	300
#define	RPL_CREATIONTIME 329
#define	RPL_USERHOST	302
#define	RPL_ISON	303
#define	RPL_AWAY	301
#define	RPL_UNAWAY	305
#define	RPL_NOWAWAY	306
#define	RPL_WHOISUSER	311
#define	RPL_WHOISSERVER	312
#define	RPL_WHOISOPERATOR	313
#define	RPL_WHOISIDLE	317
#define	RPL_ENDOFWHOIS	318
#define	RPL_WHOISCHANNELS	319
#define	RPL_WHOWASUSER	314
#define	RPL_ENDOFWHOWAS	369
#define	RPL_LISTSTART	321
#define	RPL_LIST	322
#define	RPL_LISTEND	323
#define	RPL_CHANNELMODEIS	324
#define	RPL_NOTOPIC	331
#define	RPL_TOPIC	332
#define	RPL_INVITING	341
#define	RPL_SUMMONING	342
#define	RPL_VERSION	351
#define	RPL_WHOREPLY	352
#define	RPL_ENDOFWHO	315
#define	RPL_NAMREPLY	353
#define	RPL_ENDOFNAMES	366
#define	RPL_LINKS	364
#define	RPL_ENDOFLINKS	365
#define	RPL_BANLIST	367
#define	RPL_ENDOFBANLIST	368
#define	RPL_INFO	371
#define	RPL_ENDOFINFO	374
#define	RPL_MOTDSTART	375
#define	RPL_MOTD	372
#define	RPL_ENDOFMOTD	376
#define	RPL_YOUREOPER	381
#define	RPL_REHASHING	382
#define	RPL_TIME	391
#define	RPL_USERSSTART	392
#define	RPL_USERS	393
#define	RPL_ENDOFUSERS	394
#define	RPL_NOUSERS	395
#define	RPL_TRACELINK	200
#define	RPL_TRACECONNECTING	201
#define	RPL_TRACEHANDSHAKE	202
#define	RPL_TRACEUNKNOWN	203
#define	RPL_TRACEOPERATOR	204
#define	RPL_TRACEUSER	205
#define	RPL_TRACESERVER	206
#define	RPL_TRACENEWTYPE	208
#define	RPL_TRACELOG	261
#define	RPL_STATSLINKINFO	211
#define	RPL_STATSCOMMANDS	212
#define	RPL_STATSCLINE	213
#define	RPL_STATSNLINE	214
#define	RPL_STATSILINE	215
#define	RPL_STATSKLINE	216
#define	RPL_STATSYLINE	218
#define	RPL_ENDOFSTATS	219
#define	RPL_STATSLLINE	241
#define	RPL_STATSUPTIME	242
#define	RPL_STATSOLINE	243
#define	RPL_STATSHLINE	244
#define	RPL_UMODEIS	221
#define	RPL_LUSERCLIENT	251
#define	RPL_LUSEROP	252
#define	RPL_LUSERUNKNOWN	253
#define	RPL_LUSERCHANNELS	254
#define	RPL_LUSERME	255
#define	RPL_ADMINME	256
#define	RPL_ADMINLOC1	257
#define	RPL_ADMINLOC2	258
#define	RPL_ADMINEMAIL	259
#define	RPL_TRACECLASS        	209  
#define	RPL_STATSQLINE	217
#define	RPL_SERVICEINFO       	231  
#define	RPL_ENDOFSERVICES	232
#define	RPL_SERVICE           	233  
#define	RPL_SERVLIST	234
#define	RPL_SERVLISTEND	235
#define	RPL_WHOISCHANOP       	316  
#define	RPL_KILLDONE	361
#define	RPL_CLOSING           	362  
#define	RPL_CLOSEEND	363
#define	RPL_INFOSTART         	373  
#define	RPL_MYPORTIS	384
#define	ERR_YOUWILLBEBANNED   	466  
#define	ERR_BADCHANMASK	476
#define	ERR_NOSERVICEHOST	492

# define MSG_ERR_NOSUCHNICK(pseudo) (pseudo + " :No such nick/channel\r\n") //401
# define MSG_ERR_NOSUCHSERVER(server) (server + " :No such server\r\n") //402
# define MSG_ERR_NOSUCHCHANNEL(channel) (channel + " :No such channel\r\n") //403
# define MSG_ERR_CANNOTSENDTOCHAN(channel) (channel + " :Cannot send to channel\r\n") //404
# define MSG_ERR_TOOMANYCHANNELS(channel) (channel + " :You have joined too many channels\r\n") //405
# define MSG_ERR_WASNOSUCHNICK(nickname) (nickname + " :There was no such nickname\r\n") //406
# define MSG_ERR_TOOMANYTARGETS(dest) (dest + ":Duplicate recipients. No message delivered\r\n") //407
# define MSG_ERR_NOORIGIN() (":No origin specified\r\n") //409
# define MSG_ERR_NORECIPIENT(command) (":No recipient given (" + command + ")\r\n")
# define MSG_ERR_NOTEXTTOSEND() (":No text to send\r\n")
# define MSG_ERR_NOTOPLEVEL(mask) (mask + " :No toplevel domain specified\r\n")
# define MSG_ERR_WILDTOPLEVEL(mask) (mask + " :Wildcard in toplevel domain\r\n")
# define MSG_ERR_UNKNOWNCOMMAND(command) (command + " :Unknown command\r\n")
# define MSG_ERR_NOMOTD() (":MOTD File is missing\r\n")
# define MSG_ERR_NOADMININFO(server) (server + " :No administrative info available\r\n")
# define MSG_ERR_FILEERROR(operation, file) (":File error doing " + operation + " on " + file + "\r\n")
# define MSG_ERR_NONICKNAMEGIVEN() (":No nickname given\r\n")
# define MSG_ERR_ERRONEUSNICKNAME(pseudo) (pseudo + " :Erroneus nickname\r\n")
# define MSG_ERR_NICKNAMEINUSE(nick) (nick + " :Nickname is already in use\r\n")
# define MSG_ERR_NICKCOLLISION(nick, user, host) (nick + " :Nickname collision KILL from " + user + "@" + host + "\r\n")
# define MSG_ERR_USERNOTINCHANNEL(pseudo, channel) (pseudo + " :Is not on channel " + channel + "\r\n")
# define MSG_ERR_NOTONCHANNEL(channel) (channel + " :You're not on that channel\r\n")
# define MSG_ERR_USERONCHANNEL(user, channel) (user + channel + " :User is already on that channel\r\n")
# define MSG_ERR_NOLOGIN(user) (user + " :User not logged in\r\n")
# define MSG_ERR_SUMMONDISABLED() (":SUMMON has been disabled\r\n")
# define MSG_ERR_USERSDISABLED() (":USERS has been disabled\r\n")
# define MSG_ERR_NOTREGISTERED() (":You have not registered\r\n")
# define MSG_ERR_NEEDMOREPARAMS(command) (command + " :Not enough parameters\r\n")
# define MSG_ERR_ALREADYREGISTRED() (":Unauthorized command (already registered)\r\n")
# define MSG_ERR_NOPERMFORHOST() (":Your host isn't among the privileged\r\n")
# define MSG_ERR_PASSWDMISMATCH() (":Password incorrect\r\n")
# define MSG_ERR_YOUREBANNEDCREEP() (":You are banned from this server\r\n")
# define MSG_ERR_KEYSET(channel) (channel + " :Channel key already set\r\n")
# define MSG_ERR_CHANNELISFULL(channel) (channel + " :Cannot join channel (+l)\r\n")
# define MSG_ERR_UNKNOWNMODE(character) (character + " :is unknown mode char to me\r\n")
# define MSG_ERR_INVITEONLYCHAN(channel) (channel + " :Cannot join channel (+i)\r\n")
# define MSG_ERR_BANNEDFROMCHAN(channel) (channel + " :Cannot join channel (+b)\r\n")
# define MSG_ERR_BADCHANNELKEY(channel) (channel + " :Cannot join channel (+k)\r\n")
# define MSG_ERR_NOPRIVILEGES() (":Permission Denied- You're not an IRC operator\r\n")
# define MSG_ERR_CHANOPRIVSNEEDED(channel) (channel + " :You're not channel operator\r\n")
# define MSG_ERR_CANTKILLSERVER() (":You cant kill a server!\r\n")
# define MSG_ERR_NOOPERHOST() (":No O-lines for your host\r\n")
# define MSG_ERR_UMODEUNKNOWNFLAG() (":Unknown MODE flag\r\n")
# define MSG_ERR_USERSDONTMATCH() (":Cannot change mode for other users\r\n")
# define MSG_ERR_INVALIDCAP(command) (command + " :Invalid CAP command\r\n")
# define MSG_ERR_NOTOPIC(channel) (channel + " :No topic is set\r\n")
# define MSG_ERR_BADCHANMASK(channel) (channel + " :Bad Channel Mask\r\n")
# define MSG_ERR_ALREADYBAN(channel, user) (channel + " " + user + " b :Channel " + channel + " list already contains " + user + "\r\n")
# define MSG_ERR_SERVERISFULL(serv_name) (serv_name + " Server is full\r\n");
# define MSG_ERR_KILLDENY(mssg) (":" + mssg + "\r\n")

/*
**  reponse for command file
*/
# define MSG_RPL_WELCOME(nick) (":Welcome to the Internet Relay Network " + nick + "\r\n")
# define MSG_RPL_YOURHOST(servername, version) (":Your host is " + servername + ", running version " + version + "\r\n")
# define MSG_RPL_MYINFO(servername, version, usr_modes, chann_modes) (":" + servername + " " + version + " " + usr_modes + " " + chann_modes + "\r\n")
# define MSG_RPL_CREATED(date) (":This server was created " + date + "\r\n");
# define MSG_RPL_BOUNCE(server_name, port) ("Try server " + server_name + ", port " + port + "\r\n")
# define MSG_RPL_NONE() ("\r\n") //300
# define MSG_RPL_USERHOST(userhost_list) (userhost_list + "\r\n") //<réponse> ::= <pseudo>['*'] '=' <'+'|'-'><hôte>
# define MSG_RPL_ISON(user_list) (user_list + "\r\n")
# define MSG_RPL_AWAY(pseudo, str) (pseudo + " " + str + "\r\n")
# define MSG_RPL_UNAWAY() (":You are no longer marked as being away\r\n")
# define MSG_RPL_NOWAWAY() (":You have been marked as being away\r\n")
# define MSG_RPL_WHOISUSER(nick, user, host, realname) (nick + " " + user + " " + host + " * :" + realname + "\r\n")
//# define MSG_RPL_WHOISSERVER(pseudo, server) (pseudo + " " + server + " :<info serveur>\r\n")
# define MSG_RPL_WHOISOPERATOR(pseudo) (pseudo + " :is an IRC operator\r\n")
# define MSG_RPL_WHOISIDLE(pseudo, idle, signon) (pseudo + " " + idle + " " + signon + " :seconds idle, signon time\r\n")
# define MSG_RPL_WHOREPLY(arg) (arg + "\r\n")
# define MSG_RPL_ENDOFWHOIS(pseudo) (pseudo + " :End of /WHOIS list\r\n")
# define MSG_RPL_WHOISCHANNELS(nick, chann_info) (nick + " :" + chann_info + "\r\n")
# define MSG_RPL_WHOWASUSER(nick, user, host, realname) (nick + " " + user + " " + host + " * :" + realname + "\r\n")
# define MSG_RPL_ENDOFWHOWAS(nick) (nick + " :End of WHOWAS\r\n")
# define MSG_RPL_LISTSTART() ("Channel :Users Name\r\n")
# define MSG_RPL_LIST(channel, topic) (channel + " :" + topic + "\r\n")
# define MSG_RPL_LISTEND() (":End of LIST\r\n")
# define MSG_RPL_CHANNELMODEIS(channel, mode, params) (channel + " " + mode + " " + params + "\r\n")
# define MSG_RPL_NOTOPIC(channel) (channel + " :No topic is set\r\n")
# define MSG_RPL_TOPIC(channel, subject) (channel + " :" + subject + "\r\n")
# define MSG_RPL_INVITING(channel, pseudo) (channel + " " + pseudo + "\r\n")
# define MSG_RPL_SUMMONING(user) (user + " :Summoning user to IRC\r\n")
# define MSG_RPL_VERSION(version, debuglevel, server, comment) (version + "." + debuglevel + " " + server + " :" + comment + "\r\n")
//# define MSG_RPL_WHOREPLY(channel, user, host, server, pseudo) (channel + " " + user + " " + host + " " + server + " " + pseudo + " <H|G>[*][@|+] :<compteur de distance> <vrai nom>\r\n")
# define MSG_RPL_ENDOFWHO(name) (name + " :End of WHO list\r\n")
# define MSG_RPL_NAMREPLY(channel, nick_list) (channel + " :" + nick_list + "\r\n") //modif_ici
# define MSG_RPL_ENDOFNAMES(channel) (channel + " :End of NAMES list\r\n")
# define MSG_RPL_LINKS(mask, server_name, hopcount, info) (mask + " " + server_name + " " + hopcount + " " + info + "\r\n")
# define MSG_RPL_ENDOFLINKS(mask) (mask + " :End of LINKS list\r\n")
# define MSG_RPL_BANLIST(channel, c_time, name) (channel  + " " + name + "\r\n")
# define MSG_RPL_ENDOFBANLIST(channel) (channel + " :End of list\r\n")
# define MSG_RPL_INFO(info) (":" + info + "\r\n")
# define MSG_RPL_ENDOFINFO() (":End of INFO list\r\n")
# define MSG_RPL_MOTDSTART(server) (":- " + server + " Message of the day - \r\n")
# define MSG_RPL_MOTD(comment) (":- " + comment + "\r\n")
# define MSG_RPL_ENDOFMOTD() (":End of MOTD command\r\n")
# define MSG_RPL_YOUREOPER() (":You are now an IRC operator\r\n")
# define MSG_RPL_REHASHING(config_file) (config_file + " :Rehashing\r\n")
# define MSG_RPL_TIME(server, time_serveur) (server + " :" + time_serveur + "\r\n")
# define MSG_RPL_USERSSTART() (":UserID Terminal Hôte\r\n")
//# define MSG_RPL_USERS() (":%-8s %-9s %-8s\r\n")
# define MSG_RPL_ENDOFUSERS() (":End of users\r\n")
# define MSG_RPL_NOUSERS() (":Nobody logged in\r\n")
# define MSG_RPL_TRACELINK(dest, next_serv) ("Link " + std::string(VERSION) + " " + dest + " " + next_serv + "\r\n")
# define MSG_RPL_TRACECONNECTING(classe, server) ("Try. " + classe + " " + server + "\r\n")
# define MSG_RPL_TRACEHANDSHAKE(classe, server) ("H.S. " + classe + " " + serveur + "\r\n")
# define MSG_RPL_TRACEUNKNOWN(pseudo) ("???? 1 " + pseudo + "\r\n")
# define MSG_RPL_TRACEOPERATOR(pseudo) ("Oper 1 " + pseudo + "\r\n")
# define MSG_RPL_TRACEUSER(pseudo) ("User 1 " + pseudo + "\r\n")
# define MSG_RPL_TRACESERVER(server_name) ("Serv 1 0S 0C " + server_name + " *!*@" + server_name + " V" + PROTOCOL_VERSION + "\r\n")
# define MSG_RPL_TRACENEWTYPE(new_type, name) (new_type + " 0 " + name + "\r\n")
# define MSG_RPL_TRACELOG(file_log, debug_level) ("File " + file_log + " " + debug_level + "\r\n")
# define MSG_RPL_TRACEEND(server_name, version_debug) (server_name + " " + version_debug + " :End of TRACE\r\n")
# define MSG_RPL_STATSLINKINFO(arg) (arg + "\r\n")
# define MSG_RPL_STATSCOMMANDS(arg) (arg + "\r\n")
# define MSG_RPL_STATSCLINE(host, name, port, classe) ("C " + host + " * " + name + " " + port + " " + classe + "\r\n")
# define MSG_RPL_STATSNLINE(host, name, port, classe) ("N " + host + " * " + name + " " + port + " " + classe + "\r\n")
# define MSG_RPL_STATSILINE(host, port, classe) ("I " + host + " * " + host + " " + port + " " + classe + "\r\n")
# define MSG_RPL_STATSKLINE(host, user_name, port, classe) ("K " + host + " * " + user_name + " " + port + " " + classe + "\r\n")
# define MSG_RPL_STATSYLINE(classe, frq_ping, frq_connection, sendq_max) ("Y " + classe + " " + frq_ping + " " + frq_connection + " " + sendq_max + "\r\n")
# define MSG_RPL_ENDOFSTATS(stat_letter) (stat_letter + " :End of STATS report\r\n")
# define MSG_RPL_STATSLLINE(host_mask, server_name, max_depth) ("L " + host_mask + " * " + server_name + max_depth "\r\n")
# define MSG_RPL_STATSUPTIME(arg) (":Server Up " + arg + "\r\n")
# define MSG_RPL_STATSOLINE(host_mask, name) ("O " + host_mask + " * " + name + "\r\n")
# define MSG_RPL_STATSHLINE(host_mask, server_name) ("H " + host_mask + " * " + server_name + "\r\n")
# define MSG_RPL_UMODEIS(user_mode) (user_mode + "\r\n")
# define MSG_RPL_LUSERCLIENT(int1, int2, int3) (":There are " + int1 + " users and " + int2 + " invisible on " + int3 + " servers\r\n")
# define MSG_RPL_LUSEROP(int1) (int1 + " :operator(s) online\r\n")
# define MSG_RPL_LUSERUNKNOWN(int1) (int1 + " :unknown connection(s)\r\n")
# define MSG_RPL_LUSERCHANNELS(int1) (int1 + " :channels formed\r\n")
# define MSG_RPL_LUSERME(int1, int2) (":I have " + int1 + " clients and " + int2 + " servers\r\n")
# define MSG_RPL_ADMINME(server) (server + " :Administrative info\r\n")
# define MSG_RPL_ADMINLOC1(info) (":" + info + "\r\n")
# define MSG_RPL_ADMINLOC2(info) (":" + info + "\r\n")
# define MSG_RPL_ADMINEMAIL(info) (":" + info + "\r\n")
# define MSG_RPL_CREATIONTIME(channel, c_time) ("Channel " + channel + " created on " + c_time + "\r\n")
# define MSG_RPL_YOURESERVICE(servicename) (":You are service " + servicename + "\r\n")

#endif