/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 13:42:50 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/11 18:13:01 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

user *users = NULL;
channel *channels = NULL;

void ft_exit(std::string s, int err, int *sock)
{
	if (err != 0)
		std::cerr << "\033[1;31m" << "Error: " << s << "\033[0m" << std::endl;
	close(*sock);
	exit(err);
}

int check_args(int ac, char **av)
{
	long int n;

	if (ac != 3)
		ft_exit("Wrong number of arguments\nHow to use: ./iretserv port password", 1, NULL);
	n = strtol(av[1], NULL, 10);
	if (n < 0 || n > 65535 || errno == ERANGE)
		ft_exit("Wrong port number, please pick a number between 0 and 65535.", errno == ERANGE ? errno : 1, NULL);
	return n;
}

template <typename T>
void destroy_vector(std::vector <T> v)
{
	
}

void ft_free_exit(std::string s, int err, int *sock)
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
	ft_exit(s, err, sock);
}

int new_client(int id)
{
	struct s_user *tmp = users;
	struct s_user* new_user = new struct s_user();
	if (new_user == nullptr)
		return -1;
	new_user->next = NULL;
	new_user->id = id - 1;
	if (!users)
		users = tmp;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_user;
		new_user->prev = tmp;
	}
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

int main(int ac, char **av)
{
	int server_socket, ret, nfds = 1, current_size, new_sd = -1, len;
	short server_running = TRUE, close_conn, compress_array = FALSE;
	sockaddr_in addr;
	struct pollfd fds[SOMAXCONN];
	char buffer[4096];
	
	int port = check_args(ac, av);
	//Creates socket (fd)
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		ft_exit("Couldn't create socket.", errno, NULL);
	
	addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	//Binds socket to port
	if ((bind(server_socket, (sockaddr *)&addr, sizeof(addr)) == -1))
		ft_exit("Bind failed.", errno, &server_socket);
	
	//Listen for connections on a socket
	if((listen(server_socket, SOMAXCONN)) == -1)
		ft_exit("Listen failed.", errno, &server_socket);
	memset(fds, 0 , sizeof(fds));
	fds[0].fd = server_socket;
	fds[0].events = POLLIN;
	do
	{
		ret = poll(fds, nfds, 3*60*1000);
		if (ret < 0)
    	  ft_exit("  poll() failed", 1, &server_socket);


		if (ret == 0)
			ft_exit("", 0, &server_socket);
		current_size = nfds;
		//POLLIN = data to read
		for (int i = 0; i < current_size; i++)
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
				ft_exit("revents " + fds[i].revents, fds[i].revents, &server_socket);

			if (fds[i].fd == server_socket)
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
				new_sd = accept(server_socket, NULL, NULL);
				if (new_sd < 0)
				{
					if (errno != EWOULDBLOCK)
						ft_free_exit(" accept failed", errno, &server_socket);

				}

				/*****************************************************/
				/* Add the new incoming connection to the            */
				/* pollfd structure                                  */
				/*****************************************************/
				printf("  New incoming connection - %d\n", new_sd);
				fds[nfds].fd = new_sd;
				fds[nfds].events = POLLIN;
				nfds++;
				if (new_client(nfds) == -1)
					ft_free_exit(" client creation failed.", errno, &server_socket);
				

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
				ret = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (ret < 0)
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
				if (ret == 0)
				{
					printf("  Connection closed\n");
					close_conn = TRUE;
					break;
				}

				/*****************************************************/
				/* Data was received                                 */
				/*****************************************************/
				len = ret;
				printf("  %d bytes received\n", len);

				/*****************************************************/
				/* Echo the data back to the client                  */
				/*****************************************************/
				ret = send(fds[i].fd, buffer, len, 0);
				if (ret < 0)
				{
					perror("  send() failed");
					close_conn = TRUE;
					break;
				}

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
					delete_client(i);
				}


		}  /* End of existing connection is readable             */
    }
		
	} while (server_running == TRUE);
	
	// socklen_t addr_len = sizeof(addr);
	// //Accept a connection on a socket
	// if ((accept(server_socket, (sockaddr *)&addr, &addr_len) == -1))
	// 	ft_exit("Listen failed.", errno, &server_socket);
	
	close(server_socket);
	return 0;
}