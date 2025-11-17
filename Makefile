NAME = minitalk

SERVER = server
CLIENT = client

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

SRCS_SERVER = server.c utils.c
OBJS_SERVER = 	$(SRCS_SERVER: .o=.c)

SRCS_CLIENT = client.c
OBJS_CLIENT = $(SRCS_CLIENT: .o=.c)


all: $(SERVER) $(CLIENT)

$(SERVER): $(OBJS_SERVER) minitalk.h
	$(CC) $(CFLAGS) $(OBJS_SERVER) -o $(SERVER)

$(CLIENT): $(OBJS_CLIENT) minitalk.h
	$(CC) $(CFLAGS) $(OBJS_CLIENT) -o $(CLIENT)

clean:
	$(RM) $(SERVER) \
	$(RM) $(CLIENT)

fclean: clean

re: fclean all

.PHONY: all $(NAME) clean fclean re

