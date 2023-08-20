/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 10:51:58 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 18:22:11 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <paths.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <unistd.h>
# define NC "\033[0m"
# define CYAN "\033[1;36m"
# define GREEN "\033[1;32m"
# define RED "\033[1;31m"
# define BOLD "\033[1m"

int			g_exit_status;

typedef struct s_env
{
	char			*var_name;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

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
}					t_tokens;
typedef enum e_sig_state
{
	DEFAULT_SIGNAL = 1,
	HEREDOC_SIGNAL,
	CHILD_SIGNAL,
	PARENT_SIGNAL,
}					t_sig_state;

typedef struct s_exec
{
	char			***cmds;
	char			**command;
	char			**herd_cmd;
	int				**tokens;
	int				*old_token;
	int				count_cmd;
	int				is_quote;
	int				flag;
	int				heredoc_fd[16][2];
	int				pipe_fd[2][2];
	int				*pid;
	t_env			**env;
}					t_exec;

char				*ft_operator_parse(char *command);
char				**ft_split_cmd(char *cmd, int flag);
int					*set_type_tokens(char **command);
int					ft_check_input(int *cmd_type);
void				handle_dollar(char **cmd, int *tokens, t_env *env);
char				*ft_replace_exitcode(char *token, int *flag);
char				*ft_replace_wrongname(char *token, int *flag);
void				delete_quotes(char **command, t_exec *exec);

void				start_exec(t_env **env, char *command);
t_exec				*init_execution(t_env *env, char *command);
char				***ft_fill_commands(t_exec *exec, char **command);
int					**count_tokens(t_exec *exec, int flag);
void				begin_builtin_env(char **cmd, int *toks, t_exec *exec);
char				**ft_get_command(char **cmd_arg, int *tokens, t_env *env,
						char *cmd);
void				ft_execute_cmd(char *path, char **args, int nbr_cmd,
						t_exec *exec);
void				ft_wait_children(int *cpid, int cmds_cnt);
void				init_builtin(char **command, int *tokens, t_exec *exec,
						int flag);
char				*ft_get_path_of_cmd(char **cmd_arg, int *tokens,
						t_env *env);
void				ft_get_last_cmd(t_exec *exec, int idx, char *path,
						char **args);

void				ft_pwd(t_env *env);
void				set_pwd(t_env *env, char *pwd);
void				ft_cd(char **str, t_env *env, char *old_pwd);
void				ft_unset(t_env **env, char **command);
void				ft_exit(char **args, t_exec *exec);
void				ft_print_env_var(t_env *env);
void				ft_export_concatenate(char *args, t_env **env,
						char *old_value);
void				ft_export_assign(char *args, t_env **env);
void				ft_export_create_var(char *args, t_env **env);
void				ft_export(t_env **env, char **args);
void				ft_echo(char **array_str);

int					ft_dprintf(int fd, const char *str, ...);
void				*ft_calloc(size_t count, size_t size);
int					ft_strlcpy(char *dst, char *src, size_t size);
char				*ft_strchr(const char *s, int c);
int					ft_strcmp(char *s1, char *s2);
void				close_unused_hd(int heredoc_fd[16][2], int cmd_nb);
int					ft_is_pipe(char c);
int					ft_is_redirection(char c);
int					ft_is_space(char c);
int					ft_negorpos(int nbr);
int					count_cmd(char **command, int *tokens);
int					ft_sizeof_array(char **array_str);
char				**ft_get_next_cmd(char **cmd_split, int *tokens);
int					check_path(t_exec *exec);
int					ft_is_builtin(char **command, int *tokens);
void				ft_check_state(int *state, int car);
int					ft_isalnum(int c);
int64_t				ft_erratoll(const char *str, int *err);
int64_t				ft_atoll(const char *str);
void				free_exec(t_exec *exec, char *path, char **command);
void				ft_free_arr(void **array);
char				**ft_env_name(t_env *env);
void				close_fd(int fd);
void				close_all_fd(int pipe_fd[2][2], int cmd_nb);
int					ft_check_path(t_exec *exec, t_env *env);
char				*ft_var_replace(char *token, int *i, t_env *env);
char				*ft_itoa(int n);
void				ft_error_cd(char *args, char *old_pwd);
void				ft_print_err_env(void);

int					check_use_heredoc(int fd, t_exec *exec, int nbr_cmd);
int					init_heredoc(int arr_herd[16][2], char ***commands,
						int **tokens, t_exec *exec);
int					state_stdinput(void);
void				heredoc_close(int arr_heredoc[16][2]);
void				ft_expand(char **command, t_env *env, t_exec *exec);
void				handle_signal(int sig);
void				handle_redirection(t_exec *exec, int nbr_command);
void				handle_out_file(int fd[], t_exec *exec, int nbr_command,
						int flag);
void				handle_in_file(int fd[], t_exec *exec, int nbr_command,
						int flag);

t_env				*ft_get_env(char **envp);
t_env				*ft_new_list(char *str);
t_env				*ft_list_search(t_env *env, char *searched);
void				ft_list_add_back(t_env **env, t_env *new_env);
int					ft_list_size(t_env *env);
void				free_env_list(t_env **head);
void				ft_list_clearone(t_env **root, t_env *todel);

#endif