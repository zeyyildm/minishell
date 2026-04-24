/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:15:59 by hakalkan          #+#    #+#             */
/*   Updated: 2026/04/22 21:07:27 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
#define _POSIX_C_SOURCE 200809L

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "libft/libft.h"
#include <readline/readline.h>
#include <signal.h>
#include <readline/history.h>
#include <errno.h>
#include <sys/stat.h>

typedef enum e_token_type //tokenizerın etiketleri için
{
	TWORD,
	TPIPE,
	T_REDIR_IN,		// <
	T_REDIR_OUT,    // >
	T_REDIR_APPEND, // >>
	T_HEREDOC, // <<
}t_token_type;


typedef struct s_token //tokenizerın çıktısı için, parser bu listeyi okur. parserın bir sıraya ihtiyacı var ondan enum yeterli değil
{
	char 	*value;
	t_token_type	type; //bu token ne
	struct s_token	*next;
	
}	t_token; // quot bilgisi yok

typedef struct s_redir //parserın redirectionlarını için
{
	t_token_type	type;
	char			*file; // out.txt
	struct s_redir	*next;
}	t_redir;

typedef struct s_env //envp için
{
	char	*key;
	char	*value;
	struct s_env	*next;
}	t_env;

typedef struct s_command //parserın  ana çıktısı (çalıştırılacak olan komut)
{
	char	**argv; //execveye gidecek olan
	t_redir			*redirs; //input output ayarları
	int 			i;
	int             heredoc_fd;
	struct s_command	*next;	 //pipeline için
}t_command;

typedef struct s_shell //shell state. komutlar arası kaybolmaması gereken bilgileri tutar
{
	t_token		*tokens;
	t_command	*commands;
	t_env		*env;
	char 		**envp;
	int			last_exit_status;
}t_shell;

t_token *tokenizer(char *s);
t_command *parser(t_token *t, t_command *cmd);
void	get_env(t_shell *shell);
void	free_lists(t_shell *shell);
void	free_env(t_env *env);
int		init_builtin_ex(t_shell *shell, t_command *cmd);
int		is_parent_builtin(char *cmd);
int		built_echo(t_shell *shell, t_command *cmd);
int		built_cd(t_shell *shell,t_command *cmd);
int		built_pwd(t_shell *shell, t_command *cmd);
void	add_env_node(t_shell *shell, char *key, char *value);
int		built_export(t_shell *shell, t_command *cmd);
int		built_unset(t_shell *shell, t_command *cmd);
int		built_env(t_shell *shell, t_command *cmd);
int		built_exit(t_shell *shell, t_command *cmd);
void	execute_pipe(t_shell *shell, t_command *cmd, int prev_fd);
void	execute_basic(t_shell *shell, t_command *cmd);
int		exec_heredoc(t_shell *shell, t_command *cmd);
int		is_cmd(char *name, char *cmd);
void	expanded(t_shell *shell);
char	*for_quotes(char *s);
int		exec_redir(t_command *cmd);
char	*expand_arg(t_shell *shell, char *arg);
char	*find_ex_path(t_shell *shell, char *cmd_name);
void exec_external_no_fork(t_shell *shell, t_command *cmd);


#endif