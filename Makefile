# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/03 10:51:35 by laaubry           #+#    #+#              #
#    Updated: 2026/09/12 00:29:02 by laaubry          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I.

SRCS    =   builtins/builtins1.c \
            builtins/builtins2.c \
            exec_dir/exec_core.c \
            exec_dir/exec_redir.c \
			exec_dir/exec_tools.c \
            expander/expander_utils.c \
            expander/expander.c \
            parsing/char_type.c \
            parsing/check_lexer.c \
            parsing/fill_args_fd.c \
            parsing/fs_automaton_utils.c \
            parsing/fs_automaton.c \
            parsing/heredoc.c \
            parsing/init.c \
            parsing/lexer.c \
            parsing/redir.c \
            parsing/remove_quote.c \
            parsing/tree.c \
            rumba/rumba_del.c \
            rumba/rumba_mk1.c \
            utils/ft_itoa_gp.c \
            utils/ft_printf_utils.c \
            utils/ft_printf.c \
            utils/ft_str_utils.c \
            utils/is_something.c \
            utils/utils.c \
            utils/utils2.c \
            main.c \
            signals.c
            


OBJECTS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f vgcore.*

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
