/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 11:52:06 by hed-dyb           #+#    #+#             */
/*   Updated: 2023/08/08 11:15:31 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_type
{
	_white_space,
	_pipe,
	_append_output_re,
	_here_document,
	_output_re,
	_input_re,
	_double_quote,
	_single_quote,
	_word,
}	t_type;

typedef struct s_token
{
	char			*token;
	t_type			type;
	struct s_token	*next;
	struct s_token	*previous;
}	t_token;

typedef struct s_env
{
	char			*var_name;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

t_token	*ft_create_node(char *tok, int type_, t_token *t);
void	ft_add_back(t_token **t, t_token *node);
int		ft_space_or_pipe(t_token **t, char *command, int i);
int		ft_redirections(t_token **t, char *command, int i);
int		ft_end_of_word(char *command, int i);
int		ft_is_a_word(t_token **t, char *command, int i);
int		ft_char_indice(char *command, int i, char c);
int		ft_double_qoute(t_token **t, char *command, int i);
int		ft_single_quote(t_token **t, char *command, int i);
void	ft_create_tokens(t_token **t, char *command);
int		ft_special_char(t_token *node);
void	ft_unnecessary_spaces(t_token **t);
int		ft_is_wrd_qot(t_token *temp);
void	ft_error_msg(char *msg, int fd);
int		ft_is_a_redirection(t_token *node);
int		ft_check_close(t_token *t);
int		ft_parsing(t_token *t);
int		ft_is_a_white_space(char c);
int		ft_strlen(char *s);
int		ft_isalpha(char c);
int		ft_islnum(char c);
int		ft_find_char(char *str, char c);
char	*ft_substr(char *s, int start, int len);
char	*ft_strjoin(char *s1, char *s2);
int		ft_count_words(char *str, char c);
char	**ft_split(char *str, char c);
char	*ft_strdup(char *str);
int		parse_line(char *command, t_token **token_lst);
void	ft_protection(char *to_check, void *optional, t_token **t);
void	ft_free_token(t_token *t);

#endif
