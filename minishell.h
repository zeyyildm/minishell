/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:15:59 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/04 15:53:38 by hakalkan         ###   ########.fr       */
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
	T_REDİR_IN,
	T_REDİR_OUT,    // <
	T_REDİR_APPEND, // >
	T_REDIR_APPEND, // >>
	T_HEREDOC, // <<
}t_token_type;

typedef struct s_type
{
	char	*value;
	t_token_type type;
	struct s_token *next;
	t_token;
	
}

typedef struct s_token
{
	
}	t_token_;


#endif