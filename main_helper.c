/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 20:06:50 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:06:51 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_only_spaces(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

char	*read_lines(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (!line)
		return (NULL);
	if (*line && !is_only_spaces(line))
		add_history(line);
	return (line);
}

int	line_check_quote(char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
	}
	if (quote != 0)
		return (1);
	return (0);
}

void	init_env(t_shell *shell)
{
	shell->env = NULL;
	get_env(shell);
}

int	heredoc_search(t_command *cmd)
{
	t_command	*cmd_tmp;
	t_redir		*tmp;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		tmp = cmd_tmp->redirs;
		while (tmp)
		{
			if (tmp->type == T_HEREDOC)
			{
				return (1);
			}
			tmp = tmp->next;
		}
		cmd_tmp = cmd_tmp->next;
	}
	return (0);
}
