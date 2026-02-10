/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:32:06 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/10 20:15:15 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir *add_redir(t_redir *rdr)
{
    rdr = malloc(sizeof(t_redir));
    rdr->file = NULL;
    rdr->type = 0;
    rdr->next = NULL;
    
    return rdr;
}

t_command *add_command(t_command *cmd)
{
    t_redir rdr;
    cmd = malloc(sizeof(t_command));
    cmd->redirs = add_redir(&rdr);
    cmd->argv = NULL;
    cmd->next = NULL;
    cmd->i = 0;

    return cmd;
}

int cmd_size(t_token *t)
{
    int i;

    i = 0;
    while (t && t->type != TPIPE)
    {
        if (t->type == T_REDIR_IN || t->type == T_REDIR_OUT ||t->type == T_REDIR_APPEND || t->type == T_HEREDOC)
            t = t->next;
        else if(t->type == TWORD)
            while (t->value[i])
            {
                i++;          
            }
        t = t->next;
    }
    return i;
}

int word_save(t_command *cmd , t_token *t)
{
    t_token *tmp;
    int count;

    count = 0;
    tmp = t;    
    while (tmp && tmp->type != TPIPE)
    {
        if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT ||tmp->type == T_REDIR_APPEND || tmp->type == T_HEREDOC)
            tmp = tmp->next;
        else if(tmp->type == TWORD)
        {
            count++;
            cmd->argv[cmd->i++] = ft_strdup(tmp->value);
        }
        tmp = tmp->next;
    }
    // printf("%s\n",cmd->argv[0]);
    // printf("%s\n",cmd->argv[1]);
    return count;
    
}


t_command *parser(t_token *t, t_command *cmd)
{
    cmd = NULL;
    cmd = add_command(cmd);
    int count;
    
    cmd->argv = malloc(sizeof(char *) * cmd_size(t));

    if(t && t->type == TWORD)
    {
        count = word_save(cmd, t);
        // while (t && count > 0)
        // { 
        //     t = t->next;
        //     count--;
        // }   
    }
    while (t && t->type != TPIPE)
    {
        if(t && (t->type == T_REDIR_IN || t->type == T_REDIR_OUT ||t->type == T_REDIR_APPEND || t->type == T_HEREDOC))
        {
        }
        t = t->next;    
    }
    if(t && t->type == TPIPE)
    {
        t = t->next;
        cmd->i = 0;
        cmd->next = parser(t,cmd);
        cmd = cmd->next;
    }
    return cmd;
}
