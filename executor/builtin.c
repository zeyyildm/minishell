/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 16:16:22 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:07:54 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_builtin_ex(t_shell *shell, t_command *cmd)
{
	char	*name;

	name = cmd->argv[0];
	if (!name)
		return (0);
	if (is_cmd(name, "echo"))
		return (built_echo(shell, cmd));
	else if (is_cmd(name, "cd"))
		return (built_cd(shell, cmd));
	else if (is_cmd(name, "pwd"))
		return (built_pwd(shell, cmd));
	else if (is_cmd(name, "export"))
		return (built_export(shell, cmd));
	else if (is_cmd(name, "unset"))
		return (built_unset(shell, cmd));
	else if (is_cmd(name, "env"))
		return (built_env(shell, cmd));
	else if (is_cmd(name, "exit"))
		return (built_exit(shell, cmd));
	return (-1);
}

int	built_echo(t_shell *shell, t_command *cmd)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		echo_helper(shell, cmd);
	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
			break ;
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	built_cd(t_shell *shell, t_command *cmd)
{
	char	*path_target;
	char	*old_pwd;
	char	*new_pwd;

	if (cd_get_path(shell, cmd, &path_target) != 0)
		return (1);
	if (chdir(path_target) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	old_pwd = get_env_values(shell->env, "PWD");
	new_pwd = getcwd(NULL, 0);
	if (!old_pwd || !new_pwd)
	{
		perror("minishell: pwd error");
		return (1);
	}
	update_env(shell->env, "OLDPWD", old_pwd);
	update_env(shell->env, "PWD", new_pwd);
	free(new_pwd);
	return (0);
}

int	built_pwd(t_shell *shell, t_command *cmd)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		pwd_helper(shell, cmd);
	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
			break ;
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	built_env(t_shell *shell, t_command *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		env_helper(shell, cmd);
	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
			break ;
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
