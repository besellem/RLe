# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: besellem <besellem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/04 15:00:35 by besellem          #+#    #+#              #
#    Updated: 2022/01/05 01:22:31 by besellem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	RLe

## Main directories
BUILD_DIR	:=	.build
SRCS_DIR	:=	srcs
INCS		:=	
SUB_DIR		:=	srcs
OBJ_DIR 	:=	$(BUILD_DIR)/obj
DIRS		:=	$(OBJ_DIR) $(addprefix $(OBJ_DIR)/, $(SUB_DIR))


## Main srcs
SRCS		:=	RLe_main.c \
				RLe.c \
				RLe_RunLength.c \
				RLe_RunLengthEscape.c \
				RLe_BurrowsWheelerTransform.c


## Objects
OBJS 		:=	$(SRCS:%.c=$(OBJ_DIR)/%.o)


## Commands
CC			:=	clang
CFLAGS		:=	-Wall -Wextra -Werror #-g3 -fsanitize=address
RM			:=	rm -f


# Colors
CLR_COLOR	:= \033[0m
RED			:= \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m


## Compilation
$(NAME):	$(OBJS)
			@echo "Creating $(RED_COLOR)$@ $(CLR_COLOR)..."
			@$(CC) $(CFLAGS) $(INCS) $^ -o $@
			@echo "$(GREEN)Compilation $(YELLOW)of $(RED)$@ $(BLUE)done$(CLR_COLOR)"

all:		$(NAME)

clean:
			@ echo "Deleting $(CYAN)$(NAME) $(CLR_COLOR)objs ..."
			@ $(RM) $(OBJS)
			@ rm -rf $(BUILD_DIR)

fclean:		clean
			@ echo "Deleting $(CYAN)$(NAME) $(CLR_COLOR)binary ..."
			@ $(RM) $(NAME)

re:			fclean all

$(BUILD_DIR):
			@ echo "Creating $(RED)$@ $(CLR_COLOR)..."
			@ mkdir $@ $(DIRS)

$(OBJ_DIR)/%.o:$(SRCS_DIR)/%.c | $(BUILD_DIR)
			@ echo "Compiling $(YELLOW)$< $(CLR_COLOR)..."
			@ $(CC) $(CFLAGS) -c $< -o $@ $(INCS)

.PHONY:		all clean fclean re
