NAME = philos
SRC = create.c loop.c main.c philo_eat.c philo_tsd.c utils.c
OBJ = $(SRC:.c=.o)
FLAGS = -Wall -Werror -Wextra -g
LIBS = -lpthread

all: $(NAME)

$(NAME): $(OBJ) Makefile 
	@cc $(OBJ) -o $(NAME) $(LIBS)
	@echo "Philos compiled!"

%.o: %.c Makefile philos.h
	cc $(FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ)
	@echo "Deleted .o files!"

fclean: clean
	@rm -rf $(NAME)
	@echo "Deleted executable"

re: fclean all

.PHONY: all clean fclean re
