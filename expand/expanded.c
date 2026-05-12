/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanded.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 16:16:38 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/09 16:16:38 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(t_shell *shell, char *key)
{
	t_env	*tmp;

	tmp = shell->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key)) == 0
			&& tmp->key[ft_strlen(key)] == '\0')
		{
			return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	expand_variable(t_shell *shell, char *arg, char **result)
{
	char	*value;
	char	*tmp;

	if (arg[0] == '?')
	{
		value = ft_itoa(shell->last_exit_status);
		if (!value)
			return (0);
		tmp = ft_strjoin(*result, value);
		free(*result);
		*result = tmp;
		free(value);
		return (2);
	}
	if (!arg[0] || (!(ft_isalnum(arg[0]) || arg[0] == '_')))
	{
		tmp = ft_strjoin(*result, "$\0");
		free(*result);
		*result = tmp;
		return (1);
	}
	return (handle_named_var(shell, arg, result));
}

static char	*expand_chars(t_shell *shell, char *arg, char *new_arg)
{
	int		i;
	char	state;

	i = 0;
	state = 0;
	while (arg[i])
	{
		if (arg[i] == '\'' || arg[i] == '"')
			state = update_quote_state(arg[i], state);
		else if (arg[i] == '$' && state != '\'')
		{
			i += handle_dollar(shell, arg, i, &new_arg);
			if (!new_arg)
				return (NULL);
			continue ;
		}
		else
			new_arg = append_non_special(new_arg, arg[i]);
		if (!new_arg)
			return (NULL);
		i++;
	}
	return (new_arg);
}

char	*expand_arg(t_shell *shell, char *arg)
{
	char	*new_arg;
	char	*tmp;
	char	*home;

	new_arg = ft_strdup("");
	if (!new_arg)
		return (NULL);
	if (arg[0] == '~')
	{
		home = get_env_value(shell, "HOME");
		if (home)
		{
			tmp = ft_strjoin(home, arg + 1);
			free(new_arg);
			new_arg = tmp;
			if (!new_arg)
				return (NULL);
			return (new_arg);
		}
	}
	new_arg = expand_chars(shell, arg, new_arg);
	if (!new_arg)
		return (NULL);
	return (new_arg);
}

void	expanded(t_shell *shell)
{
	int			count;
	int			total;
	t_command	*tmp;

	tmp = shell->commands;
	while (tmp)
	{
		expand_argv(shell, tmp->argv);
		total = 0;
		while (tmp->argv[total])
			total++;
		count = count_non_empty(tmp->argv);
		if (count < total)
			tmp->argv = rebuild_argv(tmp->argv, count);
		tmp = tmp->next;
	}
}
