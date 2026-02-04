/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zeyildir <zeyildir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:15:59 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/04 17:56:08 by zeyildir         ###   ########.fr       */
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

typedef struct s_command //parserın  ana çıktısı (çalıştırılacak olan komut)
{
	char	**argv; //execveye gidecek olan
	t_redir			*redirs; //input output ayarları
	struct s_command	*next;	 //pipeline için
}t_command;

typedef struct s_shell //shell state. komutlar arası kaybolmaması gereken bilgileri tutar
{
	t_token		*tokens;
	t_command	*commands;
	char 		**envp;
	int			last_exit_status;
}t_shell;


#endif