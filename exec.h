/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 10:51:58 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/06 18:22:42 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#define NC "\033[0m"
#define CYAN "\033[1;36m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define BOLD "\033[1m"

extern int	g_exit_status;

#define HD_EOF \
"minishell: warning: here-document delimited by end-of-file \
(wanted `%s')\n"
#define E_OPERATOR_PARSE \
"minishell: not a valid identifier\n"

typedef enum e_tokens
{
	CMD_NAME = 1,
	CMD_ARG,
	SIGNAL_INPUT,
	FILE_INPUT,
	SIGNAL_HEREDOC,
	LIMIT_HEREDOC,
	SIGNAL_OUTPUT,
	FILE_OUTPUT,
	SIGNAL_APP,
	FILE_APP,
	PIPE,
	EMPTY,
}			t_tokens;
typedef enum e_sig_state
{
	DEFAULT_SIGNAL = 1,
	HEREDOC_SIGNAL,
	CHILD_SIGNAL,
	PARENT_SIGNAL,
}			t_sig_state;

typedef struct s_exec
{
	char	***cmds;
	char	**command;
	int		**tokens;
	int		*old_token;
	int		count_cmd;
	int		heredoc_fd[16][2];
	int		pipe_fd[2][2];
	int		*pid;
	t_env	**env;
}			t_exec;

//!  Operator parsing

char		*ft_operator_parse(char *command);
char		**ft_split_cmd(char *cmd);
int			*set_type_tokens(char **command);
int			ft_check_input(int *cmd_type);
void		handle_dollar(char **cmd, int *tokens, t_env *env);
char		*ft_replace_exitcode(char *token, int *flag);
char		*ft_replace_wrongname(char *token, int *flag);
void		delete_quotes(char **command);

//? execution

void		start_exec(t_env **env, char *command);
t_exec		*init_execution(t_env *env, char *command);
char		***ft_fill_commands(t_exec *exec, char **command);
int			**count_tokens(t_exec *exec, int flag);
void		begin_builtin_env(char **cmd, int *toks, t_exec *exec);
char		**ft_get_command(char **cmd_arg, int *tokens, t_env *env,
				char *cmd);
void		ft_execute_cmd(char *path, char **args, int nbr_cmd, t_exec *exec);
void		ft_wait_children(int *cpid, int cmds_cnt);
void		init_builtin(char **command, int *tokens, t_exec *exec, int flag);
char		*ft_get_path_of_cmd(char **cmd_arg, int *tokens, t_env *env);
void		ft_get_last_cmd(t_exec *exec, int idx, char *path, char **args);

// TODO builtins

t_env		*ft_new_list(char *str);
t_env		*ft_list_search(t_env *env, char *searched);
void		ft_list_add_back(t_env **env, t_env *new_env);
int			ft_list_size(t_env *env);
void		free_env_list(t_env **head);
void		ft_list_clearone(t_env **root, t_env *todel);
void		ft_cd(char **args, t_env *env, t_exec *exec);
void		ft_unset(t_env **env, char **command);
void		ft_exit(char **args, t_exec *exec);
void		ft_print_env_var(t_env *env);
void		ft_export_concatenate(char *args, t_env **env, char *old_value);
void		ft_export_assign(char *args, t_env **env);
void		ft_export_create_var(char *args, t_env **env);
void		ft_export(t_env **env, char **args);
void		ft_echo(char **array_str);

//** utils

void		ft_putstr_fd(char *s, int fd);
void		*ft_calloc(size_t count, size_t size);
char		*ft_strcpy(char *s1, char *s2);
int			ft_strlcpy(char *dst, char *src, size_t size);
char		*ft_strchr(const char *s, int c);
int			ft_strcmp(char *s1, char *s2);
// int			parse_line(char *line);
int			ft_is_pipe(char c);
int			ft_is_redirection(char c);
int			ft_is_space(char c);
int			ft_negorpos(int nbr);
int			count_cmd(char **command, int *tokens);
int			ft_sizeof_array(char **array_str);
char		**ft_get_next_cmd(char **cmd_split, int *tokens);
int			check_path(t_exec *exec);
int			ft_is_builtin(char **command, int *tokens);
void		ft_check_state(int *state, int car);
int			ft_isalnum(int c);
int64_t		ft_erratoll(const char *str, int *err);
int64_t		ft_atoll(const char *str);
void		free_exec(t_exec *exec, char *path, char **command);
void		ft_free_arr(void **array);
char		**ft_env_name(t_env *env);
void		close_fd(int fd);
void		close_all_fd(int pipe_fd[2][2], int cmd_nb);

// heredoc and signal

int			check_use_heredoc(int fd, t_exec *exec, int nbr_cmd);
int			init_heredoc(int arr_herd[16][2], char ***commands, int **tokens);
int			state_stdinput(void);
void		heredoc_close(int arr_heredoc[16][2]);
void		handle_signal(int sig);
void		handle_redirection(t_exec *exec, int nbr_command);


// main.c
t_env		*ft_get_env(char **envp);

