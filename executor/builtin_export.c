/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 20:07:38 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:07:39 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	export_print(t_shell *shell)
{
	t_env	*tmp;

	tmp = env_sort(shell->env);
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

static int	export_no_equal(t_shell *shell, t_command *cmd, int i)
{
	if (!is_valid_identifier(cmd->argv[i]))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(cmd->argv[i], 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (get_env_values(shell->env, cmd->argv[i]) == NULL)
		add_env_node(shell, cmd->argv[i], NULL);
	return (0);
}

static int	export_equal(t_shell *shell, t_command *cmd, int i)
{
	char	*equal;
	char	*key;
	char	*value;

	equal = ft_strchr(cmd->argv[i], '=');
	key = ft_substr(cmd->argv[i], 0, equal - cmd->argv[i]);
	value = ft_strdup(equal + 1);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(cmd->argv[i], 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free(key);
		free(value);
		return (1);
	}
	if (get_env_values(shell->env, key))
		update_env(shell->env, key, value);
	else
		add_env_node(shell, key, value);
	free(key);
	free(value);
	return (0);
}

int	built_export(t_shell *shell, t_command *cmd)
{
	int		i;
	int		status;

	i = 1;
	status = 0;
	if (!cmd->argv[1])
	{
		export_print(shell);
		return (0);
	}
	while (cmd->argv[i] != NULL)
	{
		if (!ft_strchr(cmd->argv[i], '='))
			status |= export_no_equal(shell, cmd, i);
		else
			status |= export_equal(shell, cmd, i);
		i++;
	}
	return (status);
}
