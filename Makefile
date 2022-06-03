NAME = ircserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

HEADER_PATH = ./includes
SRCS_PATH = ./srcs

SRCS_FILES = \
	main.cpp \
	$(SRCS_PATH)/client_commands.cpp \
	$(SRCS_PATH)/server_commands.cpp \
	$(SRCS_PATH)/utils.cpp \
	$(SRCS_PATH)/exit.cpp \
	$(SRCS_PATH)/debug.cpp \

HEADER_FILES = $(HEADER_PATH)/includes.hpp $(HEADER_PATH)/replies.hpp
OBJS_PATH = ./obj/
OBJS = ${SRCS_FILES:.c=.o}

all: ${NAME}

$(NAME): ${OBJS_PATH} ${OBJS} ${HEADER_FILES}
	${CC} ${CFLAGS} -o ${NAME} ${OBJS}

${OBJS_PATH}:
	@mkdir -p $@

${OBJS_PATH}%.o: %.cpp
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${OBJS_PATH}

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re