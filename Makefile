NAME = Minishell
#.SILENT:
SRCS =	main.c \
		error.c \
		environment.c \
		initlst.c \
		parsing.c \
		split_pipes.c \
		split_args.c \
		parse_redirections.c \
		builtins.c \
		display_utils.c \
		str_utils.c \
		str_utils2.c \
		str_utils3.c \
		ft_split.c \
		lst_utils.c \
		dollar.c \
		norm_alloc.c \
		exec.c \
		signox.c \
		parse_redirection_two.c \
		exec_part_two.c

SRCS_DIR	= srcs

OBJS_DIR	= objs

OBJS = $(SRCS:%.c=%.o)

OBJS_P = $(addprefix $(OBJS_DIR)/, $(OBJS))

INC_DIR = include
RDL =  -L/Users/$(USER)/.brew/opt/readline/lib -lreadline
#RDL = -L/usr/lib/ -lreadline

INC = $(INC_DIR)/minishell.h

FS = -fsanitize=address -g3

#FS  =  -Wall

CFLAGS = -Wall -Wextra -Werror -I $(INC_DIR)


CC = gcc 
#$%.o : 	$(SRCS_DIR)/%.c $(INC)
#	$(CC) $(FS) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(INC)
			gcc $(CFLAGS) -c $< -o $@ -I /Users/$(USER)/.brew/opt/readline/include

all:		create_obj_dir $(NAME)

$(NAME) :		$(OBJS_P)	
			$(CC) $(FS) $(CFLAGS) $(OBJS_P) -o $(NAME) $(RDL)

create_obj_dir :
				rm -f objs || true
				mkdir -p objs

clean:
				rm -f $(OBJS)
fclean:		
				rm -f $(OBJS)
				rm -f $(NAME)
				rm -rf objs

re:				fclean all

.PHONY:		all clean fclean re
