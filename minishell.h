/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:15:59 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/06 20:11:30 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "libft/libft.h"
#include <readline/readline.h>
#include <signal.h>
#include <readline/history.h>
#include <errno.h>
#include <sys/stat.h>

typedef enum e_token_type
{
	TWORD,
	TPIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_command
{
	char				**argv;
	t_redir				*redirs;
	int					i;
	int					heredoc_fd;
	struct s_command	*next;
}	t_command;

typedef struct s_shell
{
	t_token		*tokens;
	t_command	*commands;
	t_env		*env;
	char		**envp;
	int			last_exit_status;
}	t_shell;

t_token		*tokenizer(char *s);
t_command	*parser(t_token *t, t_command *cmd);
t_command	*add_command(void);
t_token		*new_token(t_token_type type, char *value);
void		get_env(t_shell *shell);
void		free_lists(t_shell *shell);
void		free_env(t_env *env);
int			init_builtin_ex(t_shell *shell, t_command *cmd);
int			is_parent_builtin(char *cmd);
int			built_echo(t_shell *shell, t_command *cmd);
int			built_cd(t_shell *shell, t_command *cmd);
int			built_pwd(t_shell *shell, t_command *cmd);
void		add_env_node(t_shell *shell, char *key, char *value);
int			built_export(t_shell *shell, t_command *cmd);
int			built_unset(t_shell *shell, t_command *cmd);
int			built_env(t_shell *shell, t_command *cmd);
int			built_exit(t_shell *shell, t_command *cmd);
void		execute_pipe(t_shell *shell, t_command *cmd, int prev_fd);
void		execute_basic(t_shell *shell, t_command *cmd);
int			exec_heredoc(t_shell *shell, t_command *cmd);
int			is_cmd(char *name, char *cmd);
void		expanded(t_shell *shell);
char		*for_quotes(char *s);
int			exec_redir(t_command *cmd);
char		*expand_arg(t_shell *shell, char *arg);
char		*find_ex_path(t_shell *shell, char *cmd_name);
void		exec_external_no_fork(t_shell *shell, t_command *cmd);
void		t_add_back(t_token **list, t_token *new);
void		skip_spaces(char *s, int *i);
int			quote_size(char *str, char c);
void		redir_pars(t_command *cmd, t_token *t);
int			handle_named_var(t_shell *shell, char *arg, char **result);
int			handle_dollar(t_shell *shell, char *arg, int i, char **new_arg);
void		expand_argv(t_shell *shell, char **argv);
char		*append_char(char *str, char c);
char		update_quote_state(char c, char state);
char		*append_non_special(char *new_arg, char arg_i);
int			count_non_empty(char **argv);
char		**rebuild_argv(char **argv, int count);
int			expand_variable(t_shell *shell, char *arg, char **result);
char		*get_env_value(t_shell *shell, char *key);

#endif