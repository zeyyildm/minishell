/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:32:06 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/16 13:41:28 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir *add_redir(void)
{
    t_redir *rdr;

    rdr = malloc(sizeof(t_redir));
    if (!rdr)
        return NULL;
    rdr->file = NULL;
    rdr->type = 0;
    rdr->next = NULL;
    return rdr;
}


t_command *add_command(void)
{
    t_command *cmd;
    
    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->argv = NULL;
    cmd->next = NULL;
    cmd->redirs = NULL;
    cmd->i = 0;

    return cmd;
}

int cmd_size(t_token *t)
{
    int i;
    t_token *tmp;

    i = 0;
    tmp = t;
    while (tmp && tmp->type != TPIPE)
    {
        if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT ||tmp->type == T_REDIR_APPEND || tmp->type == T_HEREDOC)
            tmp = tmp->next;
        else if(tmp->type == TWORD)
            i++;
        tmp = tmp->next;
    }
    return i;
}

int word_save(t_command *cmd , t_token *t)
{
    t_token *tmp;
    int count;

    count = 0;
    tmp = t;    
    while (tmp && tmp->type != TPIPE) //pipe gelene kadar tüm tokenları gez
    {
        if (tmp->next && (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT ||tmp->type == T_REDIR_APPEND || tmp->type == T_HEREDOC))
        {
            tmp = tmp->next; //redirleri atla argvye koyma
        }
        else if(tmp->type == TWORD)
        {
            count++; //word saydır
            cmd->argv[cmd->i++] = ft_strdup(tmp->value); //argvye komutu ekle
        }
        tmp = tmp->next;
    }
    cmd->argv[count] = NULL;
    return count;
}

void redir_pars(t_command *cmd, t_token *t)
{
    t_redir *new;

    if (!t->next || !t->next->value)
        return;
    new = add_redir();
    new->type = t->type;
    new->file = ft_strdup(t->next->value);
    new->next = cmd->redirs;
    cmd->redirs = new;
}

t_command *parser(t_token *t, t_command *cmd)
{
    cmd = add_command();
    cmd->argv = malloc(sizeof(char *) * (cmd_size(t) + 1));
    word_save(cmd, t);
    while (t && t->type != TPIPE)
    {
        if(t && (t->type == T_REDIR_IN || t->type == T_REDIR_OUT ||t->type == T_REDIR_APPEND || t->type == T_HEREDOC))
        {
            if (t->next && t->next->type == TWORD)
                redir_pars(cmd,t);
            else
                return NULL;
        }
        t = t->next;
    }
    if(t && t->type == TPIPE)
    {
        t = t->next;
        if(!t)
        {
            return NULL;//buralar şimdilik böyle free falan yapılacak
        }
        cmd->i = 0;
        cmd->next = parser(t,NULL);
    }
    return cmd;
}
