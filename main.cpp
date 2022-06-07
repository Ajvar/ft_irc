/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 13:42:50 by jcueille          #+#    #+#             */
/*   Updated: 2022/06/08 00:42:38 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/includes.hpp"

user *users = NULL;
channel *channels = NULL;

static int check_args(int ac, char **av)
{
	long int n;

	if (ac != 3)
		ft_exit("Wrong number of arguments\nHow to use: ./ircserv port password", 1, NULL);
	n = strtol(av[1], NULL, 10);
	if (n < 0 || n > 65535 || errno == ERANGE)
		ft_exit("Wrong port number, please pick a number between 0 and 65535.", errno == ERANGE ? errno : 1, NULL);
	return n;
}

int new_client(int id, struct pollfd *fd)
{
	struct s_user *tmp = users;
	struct s_user* new_user = new struct s_user();
	if (new_user == NULL)
		return -1;
	new_user->next = NULL;
	new_user->id = id;
	if (!users)
		users = new_user;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_user;
		new_user->prev = tmp;
	}
	new_user->fd = fd;
	new_user->nickname = "";
	memset(new_user->modes, 0, sizeof(new_user->modes));
	new_user->hostname = "42irc.com";
	return 0;
	
}

void delete_client(int id)
{
	user * tmp = users;
	while (tmp != NULL)
	{
		if (tmp->id == id)
		{
			tmp->next->prev = tmp->prev;
			tmp->prev->next = tmp->next;
			std::vector<channel *>().swap(tmp->channels);
			delete tmp;
			return ;
		}
		tmp = tmp->next;
	}

}

void delete_channel(std::string name)
{
	channel * tmp = channels;
	while (tmp != NULL)
	{
		if (tmp->name == name)
		{
			tmp->next->prev = tmp->prev;
			tmp->prev->next = tmp->next;
			std::vector<user *>().swap(tmp->users);
			delete tmp;
			return ;
		}
		tmp = tmp->next;
	}

}

channel *new_channel(std::string name)
{
	channel *tmp = channels;
	channel* new_channel = new channel();
	if (new_channel == NULL)
		return NULL;
	new_channel->next = NULL;
	new_channel->name = name;
	if (!channels)
		channels = new_channel;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_channel;
		new_channel->prev = tmp;
	}
	new_channel->key = "";
	memset(new_channel->modes, 0, sizeof(new_channel->modes));
	time(&new_channel->creation);
	return new_channel;
	
}

int main (int argc, char *argv[])
{
	int port = check_args(argc, argv);
	int	restart; //set to 1 when RESTART is called
	do
	{
		int    len, rc, on = 1, new_client_id = 0;
		int    listen_sd = -1, new_sd = -1;
		int    end_server = FALSE, compress = FALSE;
		int    close_conn;
		char   buffer[513];
		struct sockaddr_in6   addr;
		struct pollfd fds[SOMAXCONN];
		int    nfds = 1, current_size = 0, i;

		restart = 0;
		/*************************************************************/
		/* Create an AF_INET6 stream socket to receive incoming      */
		/* connections on                                            */
		/*************************************************************/
		if ( (listen_sd = socket(AF_INET6, SOCK_STREAM, 0)) == -1)
			ft_exit(" socket creation failed.", errno, NULL);


		/*************************************************************/
		/* Allow socket descriptor to be reuseable                   */
		/*************************************************************/
		if ((rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
						(char*)&on, sizeof(on))) < 0)
		ft_exit(" setsockopt failed.", errno, &listen_sd);


		/*************************************************************/
		/* Set socket to be nonblocking. All of the sockets for      */
		/* the incoming connections will also be nonblocking since   */
		/* they will inherit that state from the listening socket.   */
		/*************************************************************/
		if ( (rc = ioctl(listen_sd, FIONBIO, (char*)&on)) < 0)
			ft_exit(" ioctl failed.", errno, &listen_sd);

		/*************************************************************/
		/* Bind the socket                                           */
		/*************************************************************/
		memset(&addr, 0, sizeof(addr));
		addr.sin6_family      = AF_INET6;
		memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
		addr.sin6_port        = htons(port);
		if ((rc = bind(listen_sd,
				(struct sockaddr *)&addr, sizeof(addr))) < 0)
			ft_exit(" bind failed.", errno, &listen_sd);

		/*************************************************************/
		/* Set the listen back log                                   */
		/*************************************************************/
		if ((rc = listen(listen_sd, SOMAXCONN)) < 0)
			ft_exit(" listen failed.", errno, &listen_sd);

		/*************************************************************/
		/* Initialize the pollfd structure                           */
		/*************************************************************/
		memset(fds, 0 , sizeof(fds));

		/*************************************************************/
		/* Set up the initial listening socket                        */
		/*************************************************************/
		fds[0].fd = listen_sd;
		fds[0].events = POLLIN;


		/*************************************************************/
		/* Loop waiting for incoming connects or for incoming data   */
		/* on any of the connected sockets.                          */
		/*************************************************************/
		//char astring[4000];

		do
		{
			/***********************************************************/
			/* Call poll() and wait 3 minutes for it to complete.      */
			/***********************************************************/
			//printf("Waiting on poll()...\n");
			if ((rc = poll(fds, nfds, -1)) < -1)
				ft_exit(" poll failed.", errno, &listen_sd);

			/***********************************************************/
			/* Check to see if the 3 minute time out expired.          */
			/***********************************************************/
			if (rc == 0)
			{
				printf("  poll() timed out.  End program.\n");
				break;
			}


			/***********************************************************/
			/* One or more descriptors are readable.  Need to          */
			/* determine which ones they are.                          */
			/***********************************************************/
			current_size = nfds;
			for (i = 0; i < current_size; i++)
			{
				/*********************************************************/
				/* Loop through to find the descriptors that returned    */
				/* POLLIN and determine whether it's the listening       */
				/* or the active connection.                             */
				/*********************************************************/
				if(fds[i].revents == 0)
					continue;

				/*********************************************************/
				/* If revents is not POLLIN, it's an unexpected result,  */
				/* log and end the server.                               */
				/*********************************************************/
				if(fds[i].revents != POLLIN)
				{
					printf("  Error! revents = %d\n", fds[i].revents);
					end_server = TRUE;
					break;
				}
				if (fds[i].fd == listen_sd)
				{
					/*******************************************************/
					/* Listening descriptor is readable.                   */
					/*******************************************************/
					printf("  Listening socket is readable\n");

					/*******************************************************/
					/* Accept all incoming connections that are            */
					/* queued up on the listening socket before we         */
					/* loop back and call poll again.                      */
					/*******************************************************/
					do
					{
						/*****************************************************/
						/* Accept each incoming connection. If               */
						/* accept fails with EWOULDBLOCK, then we            */
						/* have accepted all of them. Any other              */
						/* failure on accept will cause us to end the        */
						/* server.                                           */
						/*****************************************************/
						new_sd = accept(listen_sd, NULL, NULL);
						if (new_sd < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								perror("  accept() failed");
								end_server = TRUE;
							}
							break;
						}

						/*****************************************************/
						/* Add the new incoming connection to the            */
						/* pollfd structure                                  */
						/*****************************************************/
						printf("  New incoming connection - %d\n", new_sd);
						fds[nfds].fd = new_sd;
						fds[nfds].events = POLLIN;
						if (new_client(new_client_id, &fds[nfds]) == -1)
							ft_free_exit(" user creation failed.", -1, fds, nfds);
						nfds++;
						new_client_id++;

						/*****************************************************/
						/* Loop back up and accept another incoming          */
						/* connection                                        */
						/*****************************************************/
					} while (new_sd != -1);
				}

				/*********************************************************/
				/* This is not the listening socket, therefore an        */
				/* existing connection must be readable                  */
				/*********************************************************/

				else
				{
					printf("  Descriptor %d is readable\n", fds[i].fd);
					close_conn = FALSE;
					/*******************************************************/
					/* Receive all incoming data on this socket            */
					/* before we loop back and call poll again.            */
					/*******************************************************/
					do
					{
						/*****************************************************/
						/* Receive data on this connection until the         */
						/* recv fails with EWOULDBLOCK. If any other         */
						/* failure occurs, we will close the                 */
						/* connection.                                       */
						/*****************************************************/

						memset(buffer, 0, sizeof(buffer));
						rc = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
						buffer[512] = '\0';
						if (rc < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								perror("  recv() failed");
								close_conn = TRUE;
							}
							break;
						}

						/*****************************************************/
						/* Check to see if the connection has been           */
						/* closed by the client                              */
						/*****************************************************/
						if (rc == 0)
						{
							printf("  Connection closed\n");
							close_conn = TRUE;
							break;
						}
						std::vector<std::string> nicknames;
						nicknames.push_back("test1");
						nicknames.push_back("test2");
						/*****************************************************/
						/* Data was received                                 */
						/*****************************************************/
						len = rc;
						printf("  %d bytes received\n", len);
						std::cout << buffer << std::endl;
						user *tmp = find_user_by_fd(fds[i].fd);
						tmp->nickname = "another";
						std::vector<std::string> chanz;
						std::vector<std::string> keys;
						chanz.push_back("#test");
						if (std::string(buffer).find("AWAY") != std::string::npos)
							AWAY("I'm away", tmp);
						else if (std::string(buffer).find("OPER") != std::string::npos)
							OPER("nickname1", "password1", tmp);
						else if (std::string(buffer).find("PASS") != std::string::npos)
							PASS(argv[2], "lol", tmp);
						else if (std::string(buffer).find("NICK") != std::string::npos)
							NICK("testttt", tmp);
						else if (std::string(buffer).find("USER") != std::string::npos)
							USER("usertest", "realtest", tmp);
						else if (std::string(buffer).find("MODE") != std::string::npos)
							MODE(tmp->nickname, '+',  'i', tmp);
						else if (std::string(buffer).find("QUIT") != std::string::npos)
							QUIT("bye", fds, &nfds, tmp);
						else if (std::string(buffer).find("DIE") != std::string::npos)
							DIE(tmp, fds, nfds);
						else if (std::string(buffer).find("RESTART") != std::string::npos)
							RESTART(tmp, fds, nfds, &restart);
						else if (std::string(buffer).find("WALLOPS") != std::string::npos)
							WALLOPS("wallopstest", tmp);
						else if (std::string(buffer).find("ISON") != std::string::npos)
							ISON(nicknames, tmp);
						else if (std::string(buffer).find("JOIN") != std::string::npos)
							JOIN(chanz, keys, 0, tmp, fds, nfds);
						else if (std::string(buffer).find("PART") != std::string::npos)
							PART(chanz, keys, tmp);
						else if (std::string(buffer).find("TOPIC") != std::string::npos)
							TOPIC("Hello", "test", tmp);
						
						//PARSER
						

						/*****************************************************/
						/* Echo the data back to the client                  */
						/*****************************************************/
						rc = send(fds[i].fd, buffer, len, 0);
						if (rc < 0)
						{
							perror("  send() failed");
							close_conn = TRUE;
							break;
						}
						break;

					} while(TRUE);

					/*******************************************************/
					/* If the close_conn flag was turned on, we need       */
					/* to clean up this active connection. This            */
					/*clean up process includes removing the              */
					/* descriptor.                                         */
					/*******************************************************/
					if (close_conn)
					{
						close(fds[i].fd);
						fds[i].fd = -1;
						compress = TRUE;
					}


					}  /* End of existing connection is readable             */
			} /* End of loop through pollable descriptors              */

			/***********************************************************/
			/* If the compress flag was turned on, we need       */
			/* to squeeze together the array and decrement the number  */
			/* of file descriptors. We do not need to move back the    */
			/* events and revents fields because the events will always*/
			/* be POLLIN in this case, and revents is output.          */
			/***********************************************************/
			if (compress)
			{
				compress = FALSE;
				compress_array(fds, &nfds);
			}

		} while (end_server == FALSE); /* End of serving running.    */

		ft_free_exit("0", 0, fds, nfds);
	} while (restart == 1);
	

}
