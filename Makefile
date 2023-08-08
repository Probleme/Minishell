# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/16 14:44:39 by hed-dyb           #+#    #+#              #
#    Updated: 2023/08/08 13:23:05 by ataouaf          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


LIBRARY = minishell.h exec.h
NAME = minishell
CC = cc 
RM = rm -rf
FLAGS =  #-Wall -Werror -Wextra #-g -fsanitize=address,undefined

OBJECTS = ./obj
EXEC = $(addprefix execution/, check_tokens.c ft_utils.c exec_cmd.c exec.c execute.c utils.c command_init.c builtins.c)
PARSE = $(addprefix parsing/, parse_operator.c command_split.c handle_dollar.c dollar_utils.c delete_quotes.c utils.c parse.c tokenise.c check_path.c)
HEREDOC = $(addprefix hered_sig/, read_heredoc.c heredoc_utils.c handle_signal.c handle_file.c)
BUILTINS  = $(addprefix builtins/, env.c ft_export_utils.c ft_cd.c ft_cd_utils.c ft_echo.c ft_exit.c ft_export_utils2.c ft_unset.c ft_exit_utils.c ft_export.c)
TOKENIZER = $(addprefix tokenizer/, tokenizer_1.c tokenizer_2.c spaces.c)
PARSER = $(addprefix parcer/, parser.c)
TOOLS = $(addprefix tools/, utils_1.c utils_2.c)

FILES = main.c $(TOKENIZER) $(TOOLS) $(PARSER) $(EXPANDER) $(EXEC) $(PARSE) $(HEREDOC) $(BUILTINS) 

OBJ = $(addprefix $(OBJECTS)/, $(FILES:%.c=%.o))

all : $(NAME)

$(OBJECTS)/%.o : %.c $(LIBRARY)
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(CPPFLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $(LDFLAGS) -lreadline $(OBJ)  -o $(NAME)
	
clean :
	$(RM) $(OBJECTS)

fclean : clean
	$(RM) $(NAME)

re : fclean all
