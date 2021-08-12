.SUFFIXES:

NAME	=	philo
LIST	=	philosophers \

HEADER	=	-I includes/
CFLAGS	=	-Wall -Wextra -Werror
CC		=	clang

S_DIR	=	srcs/
I_DIR	=	includes/
O_DIR	=	objs/
D_DIR	=	deps/

C_SUFF	=	.c
O_SUFF	=	.o
D_SUFF	=	.d

DIR_O	=	@mkdir -p objs/
DIR_D	=	@mkdir -p deps/

RM_FILE	= rm -f
RM_DIR	= rm -rf

LIST_C	= $(addprefix $(S_DIR), $(LIST))
LIST_O	= $(addprefix $(O_DIR), $(LIST))
LIST_D	= $(addprefix $(D_DIR), $(LIST))

SRCS	= $(addsuffix $(C_SUFF), $(LIST_C))
OBJS	= $(addsuffix $(O_SUFF), $(LIST_O))
DEPS	= $(addsuffix $(D_SUFF), $(LIST_D))

$(O_DIR)%.o	:	$(S_DIR)%.c
				$(DIR_O)
				$(CC) $(HEADER) -c $< -o $@

$(D_DIR)%.d	:	$(O_DIR)%.o
				$(DIR_D)
				$(CC) $(HEADER) $< -MMD $@

$(NAME)	:		$(OBJS)
				$(CC) $(LIST_O) -o $(NAME)

all	:			$(NAME)

clean	:		
				$(RM_DIR) $(O_DIR) $(D_DIR)

fclean	:		clean
				$(RM_FILE) $(NAME)

re	:			fclean all

.PHONY	:		all clean fclean re

-include	$(DEP)

