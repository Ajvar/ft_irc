/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 13:42:50 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/12 16:53:05 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

user *users = NULL;
channel *channels = NULL;

void ft_exit(std::string s, int err, int *sock)
{
	if (err != 0)
		std::cerr << "\033[1;31m" << "Error: " << s << "\033[0m" << std::endl;
	if (sock)
		close(*sock);
	exit(err);
}

int check_args(int ac, char **av)
{
	long int n;

	if (ac != 3)
		ft_exit("Wrong number of arguments\nHow to use: ./ircserv port password", 1, NULL);
	n = strtol(av[1], NULL, 10);
	if (n < 0 || n > 65535 || errno == ERANGE)
		ft_exit("Wrong port number, please pick a number between 0 and 65535.", errno == ERANGE ? errno : 1, NULL);
	return n;
}

template <typename T>
void destroy_vector(std::vector <T> v)
{
	
}

void ft_free_exit(std::string s, int err, int *sock, pollfd *fds, int nfds)
{
	user *tmp_users;
	channel *tmp_channels = channels;
	while (users)
	{
		tmp_users = users;
		users = users->next;
		std::vector<channel *>().swap(tmp_users->channels);
		free(tmp_users);
	}
	while (channels)
	{
		tmp_channels = channels;
		channels = channels->next;
		std::vector<user *>().swap(tmp_channels->users);
		free(tmp_channels);
	}
	if (fds)
	{
		for (int i = 0; i < nfds; i++)
		{
			if(fds[i].fd >= 0)
				close(fds[i].fd);
		}
	}
	ft_exit(s, err, sock);
}

int new_client(int id)
{
	struct s_user *tmp = users;
	struct s_user* new_user = new struct s_user();
	if (new_user == nullptr)
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
	std::cout << "new clien id: " << new_user->id << std::endl;
	return 0;
	
}

void delete_client(int id)
{
	struct s_user * tmp = users;
	while (tmp != NULL)
	{
		if (tmp->id == id)
		{
			tmp->next->prev = tmp->prev;
			tmp->prev->next = tmp->next;
			std::vector<channel *>().swap(tmp->channels);
			delete tmp;
		}
		tmp = tmp->next;
	}

}

int main (int argc, char *argv[])
{
	int    len, rc, on = 1, new_client_id = 0;
	int    listen_sd = -1, new_sd = -1;
	int    desc_ready, end_server = FALSE, compress_array = FALSE;
	int    close_conn;
	char   buffer[80];
	struct sockaddr_in6   addr;
	int    timeout;
	struct pollfd fds[SOMAXCONN];
	int    nfds = 1, current_size = 0, i, j;

	int port = check_args(argc, argv);

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
					(char *)&on, sizeof(on))) < 0)
	ft_exit(" setsockopt failed.", errno, &listen_sd);


	/*************************************************************/
	/* Set socket to be nonblocking. All of the sockets for      */
	/* the incoming connections will also be nonblocking since   */
	/* they will inherit that state from the listening socket.   */
	/*************************************************************/
	if ( (rc = ioctl(listen_sd, FIONBIO, (char *)&on)) < 0)
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
	/* Initialize the timeout to 3 minutes. If no                */
	/* activity after 3 minutes this program will end.           */
	/* timeout value is based on milliseconds.                   */
	/*************************************************************/
	timeout = (3 * 60 * 1000);

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
		printf("Waiting on poll()...\n");
		if ((rc = poll(fds, nfds, timeout)) < -1)
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
			std::cout << "forloop" << std::endl;
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
					nfds++;
					if (new_client(new_client_id) == -1)
						ft_free_exit(" user creation failed.", -1, &listen_sd, fds, nfds);
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
				rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
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

				/*****************************************************/
				/* Data was received                                 */
				/*****************************************************/
				len = rc;
				printf("  %d bytes received\n", len);

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
				/* clean up process includes removing the              */
				/* descriptor.                                         */
				/*******************************************************/
				if (close_conn)
				{
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_array = TRUE;
				}


				}  /* End of existing connection is readable             */
		} /* End of loop through pollable descriptors              */

		/***********************************************************/
		/* If the compress_array flag was turned on, we need       */
		/* to squeeze together the array and decrement the number  */
		/* of file descriptors. We do not need to move back the    */
		/* events and revents fields because the events will always*/
		/* be POLLIN in this case, and revents is output.          */
		/***********************************************************/
		if (compress_array)
		{
			compress_array = FALSE;
			for (i = 0; i < nfds; i++)
			{
			if (fds[i].fd == -1)
			{
				for(j = i; j < nfds-1; j++)
					fds[j].fd = fds[j+1].fd;
				i--;
				nfds--;
			}
			}
		}

	} while (end_server == FALSE); /* End of serving running.    */

	ft_free_exit("0", 0, &listen_sd, fds, nfds);
}
