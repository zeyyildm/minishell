/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:32:06 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/06 20:11:30 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cmd_size(t_token *t)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = t;
	while (tmp && tmp->type != TPIPE)
	{
		if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT
			|| tmp->type == T_REDIR_APPEND || tmp->type == T_HEREDOC)
			tmp = tmp->next;
		else if (tmp->type == TWORD)
			i++;
		tmp = tmp->next;
	}
	return (i);
}

int	word_save(t_command *cmd, t_token *t)
{
	t_token	*tmp;
	int		count;
	int		i;

	count = 0;
	tmp = t;
	i = 0;
	while (tmp && tmp->type != TPIPE)
	{
		if (tmp->next && (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT
				||tmp->type == T_REDIR_APPEND || tmp->type == T_HEREDOC))
			tmp = tmp->next;
		else if (tmp->type == TWORD)
		{
			cmd->argv[i++] = ft_strdup(tmp->value);
			count++;
		}
		tmp = tmp->next;
	}
	cmd->argv[i] = NULL;
	cmd->i = i;
	return (count);
}

t_command	*parser(t_token *t, t_command *cmd)
{
	cmd = add_command();
	cmd->argv = malloc(sizeof(char *) * (cmd_size(t) + 1));
	word_save(cmd, t);
	while (t && t->type != TPIPE)
	{
		if (t && (t->type == T_REDIR_IN || t->type == T_REDIR_OUT
				||t->type == T_REDIR_APPEND || t->type == T_HEREDOC))
		{
			if (t->next && t->next->type == TWORD)
				redir_pars(cmd, t);
			else
				return (NULL);
		}
		t = t->next;
	}
	if (t && t->type == TPIPE)
	{
		t = t->next;
		if (!t)
			return (NULL);
		cmd->i = 0;
		cmd->next = parser(t, NULL);
	}
	return (cmd);
}
