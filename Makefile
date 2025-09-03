NAME = philos
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
FLAGS = -Wall -Werror -Wextra -g
LIBS = -lpthread

all: $(NAME)

$(NAME): $(OBJ)
	@cc $(addprefix obj/, $(OBJ)) -o $(NAME) $(LIBS)
	@echo "Philos compiled!"

%.o: %.c
	@cc $(FLAGS) -c $< -o $@
	@mkdir -p obj
	@mv $@ obj/

clean:
	@rm -rf $(addprefix obj/, $(OBJ))
	@rm -rf obj
	@echo "Deleted .o files!"

fclean: clean
	@rm -rf $(NAME)
	@echo "Deleted executable"

re: fclean all

.PHONY: all clean fclean re
