/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 20:07:50 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:07:51 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	unset_node(t_shell *shell, t_command *cmd, int i)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = shell->env;
	prev = NULL;
	while (tmp)
	{
		if (is_cmd(tmp->key, cmd->argv[i]))
		{
			if (prev == NULL)
				shell->env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	built_unset(t_shell *shell, t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->argv[i])
	{
		unset_node(shell, cmd, i);
		i++;
	}
	return (0);
}
