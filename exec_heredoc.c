/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:43:08 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/16 13:47:18 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_heredoc(t_command *cmd)
{
    t_command *tmp;

    tmp = cmd;
    while (tmp)
    {
        if(tmp->redirs->type )

        tmp = tmp->next;
    }
    
}