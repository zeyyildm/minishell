/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 16:16:16 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/09 16:16:17 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	t_redir	*add_redir(void)
{
	t_redir	*rdr;

	rdr = malloc(sizeof(t_redir));
	if (!rdr)
		return (NULL);
	rdr->file = NULL;
	rdr->type = 0;
	rdr->next = NULL;
	return (rdr);
}

t_command	*add_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->next = NULL;
	cmd->redirs = NULL;
	cmd->i = 0;
	cmd->heredoc_fd = -1;
	return (cmd);
}

void	redir_add_back(t_command *cmd, t_redir *new)
{
	t_redir	*tmp;

	if (!cmd->redirs)
	{
		cmd->redirs = new;
		return ;
	}
	tmp = cmd->redirs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	redir_pars(t_command *cmd, t_token *t)
{
	t_redir	*new;

	if (!t->next || !t->next->value)
		return ;
	new = add_redir();
	new->type = t->type;
	new->file = ft_strdup(t->next->value);
	redir_add_back(cmd, new);
}
