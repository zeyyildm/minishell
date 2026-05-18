/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 20:07:31 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:07:32 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd_get_path(t_shell *shell, t_command *cmd, char **path_target)
{
	if (cmd->argv[1] && cmd->argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (!cmd->argv[1])
		*path_target = get_env_values(shell->env, "HOME");
	else if (is_cmd(cmd->argv[1], "-"))
		*path_target = get_env_values(shell->env, "OLDPWD");
	else
		*path_target = cmd->argv[1];
	if (!*path_target)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	return (0);
}

char	*get_env_values(t_env *env, char *key)
{
	while (env)
	{
		if (is_cmd(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	is_cmd(char *name, char *cmd)
{
	if (!name || !cmd)
		return (0);
	if (ft_strlen(name) != ft_strlen(cmd))
		return (0);
	return (ft_strncmp(name, cmd, ft_strlen(cmd)) == 0);
}

void	update_env(t_env *env, char *key, char *new_value)
{
	while (env)
	{
		if (is_cmd(env->key, key))
		{
			free(env->value);
			env->value = ft_strdup(new_value);
			return ;
		}
		env = env->next;
	}
}

int	built_exit(t_shell *shell, t_command *cmd)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->argv[1] && !is_valid_number(cmd->argv[1]))
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	if (cmd->argv[1] && cmd->argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (cmd->argv[1])
		exit_code = ft_atoi(cmd->argv[1]);
	printf("exit\n");
	rl_clear_history();
	free_lists(shell);
	free_env(shell->env);
	exit(exit_code);
}
