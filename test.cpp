#include <iostream>
#include <fstream>

int main()
{
	std::ifstream file("conf");

	std::string a, b;
	file >> a >> b;
	std::cout << a << " " << b << std::endl;
	return 0;
}

/*
USER MODES :
- a : away
- i : invisible
- w : wallops
- r : restricted
- o : operator

CHANNEL MODES :
for users :
    - O : channel creator
    - o : channel operator
    - v : voice
for channels :
    - i : invite only
    - m : moderated
    - n : outside privmsg disabled
    - p : private
    - t : topic locked
    - k : key locked
    - l : user limit


CLIENT COMMANDS:

PASS OK 
NICK OK 
USER OK 
OPER OK 
MODE OK
QUIT OK 

CHANNEL COMMANDS:
JOIN OK 
PART
MODE
TOPIC
NAMES 
LIST 
INVITE 
KICK 

SENDING MESSAGES:
PRIVMSG 
NOTICE
*/