/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcueille <jcueille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 13:42:50 by jcueille          #+#    #+#             */
/*   Updated: 2022/05/09 16:18:24 by jcueille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

user *users = NULL;
channel *channels = NULL;

void ft_exit(std::string s, int err, int *sock)
{
	std::cerr << "\033[1;31m" << "Error: " << s << "\033[0m" << std::endl;
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



int main(int ac, char **av)
{
	int server_socket, ret, nfds = 1;
	short server_running = TRUE;
	sockaddr_in addr;
	struct pollfd fds[SOMAXCONN];
	
	int port = check_args(ac, av);
	//Creates socket (fd)
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		ft_exit("Couldn't create socket.", errno, NULL);
	
	addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	//Binds socket to IP and port
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
		
	} while (server_running == TRUE);
	
	socklen_t addr_len = sizeof(addr);
	//Accept a connection on a socket
	if ((accept(server_socket, (sockaddr *)&addr, &addr_len) == -1))
		ft_exit("Listen failed.", errno, &server_socket);
	
	close(server_socket);
	return 0;
}