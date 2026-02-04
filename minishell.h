/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:15:59 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/04 16:33:28 by hakalkan         ###   ########.fr       */
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

typedef enum e_token_type
{
	TWORD,
	TPİPE,
	T_REDİR_IN,		// <
	T_REDİR_OUT,    // >
	T_REDIR_APPEND, // >>
	T_HEREDOC, // <<
}t_token_type;


typedef struct s_token
{
	char 	*value;
	t_token_type	type;
	struct s_token	*next;
	
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char	**argv;
	t_redir			*redirs;
	int				has_pipe;
	struct s_command	*next;	
}t_command;

typedef struct s_shell
{
	t_token		*tokens;
	t_command	*commands;
	char 		**envp;
	int			last_exit_status;
}t_shell;


#endif