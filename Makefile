NAME = minitalk

SERVER = server
CLIENT = client

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

SRCS_SERVER = server.c utils.c
OBJS_SERVER = $(SRCS_SERVER:.c=.o)

SRCS_CLIENT = client.c utils.c
OBJS_CLIENT = $(SRCS_CLIENT:.c=.o)

all: $(SERVER) $(CLIENT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER): $(OBJS_SERVER)
	$(CC) $(CFLAGS) $(OBJS_SERVER) -o $(SERVER)

$(CLIENT): $(OBJS_CLIENT)
	$(CC) $(CFLAGS) $(OBJS_CLIENT) -o $(CLIENT)

clean:
	$(RM) $(OBJS_SERVER) $(OBJS_CLIENT)

fclean: clean
	$(RM) $(SERVER) $(CLIENT)

re: fclean all

.PHONY: all $(NAME) clean fclean re

