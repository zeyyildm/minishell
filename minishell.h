/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:15:59 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/14 20:01:19 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "libft/libft.h"


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
void	print_commands(t_command *cmd);
void	print_tokens(t_token *head);
void	print_env_debug(t_env *env);
void get_env(t_shell *shell);





#endif