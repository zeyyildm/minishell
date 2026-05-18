/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_ex_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 20:08:25 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:08:26 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_exec_error(t_shell *shell, char *full_path, int err)
{
	free(full_path);
	free_lists(shell);
	free_env(shell->env);
	if (err == ENOENT)
		exit(127);
	if (err == EACCES)
		exit(126);
	exit(127);
}

void	handle_is_directory(t_shell *shell, t_command *cmd,
		char *full_path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(": Is a directory\n", 2);
	free(full_path);
	free_lists(shell);
	free_env(shell->env);
	exit(126);
}

void	handle_exec_error(t_shell *shell, t_command *cmd,
		char *full_path)
{
	int			err;
	struct stat	path_stat;

	err = errno;
	stat(cmd->argv[0], &path_stat);
	if (err == EACCES && S_ISDIR(path_stat.st_mode))
		handle_is_directory(shell, cmd, full_path);
	errno = err;
	perror(cmd->argv[0]);
	exit_exec_error(shell, full_path, err);
}

char	*prepare_external_exec(t_shell *shell, t_command *cmd)
{
	char	*full_path;

	if (!cmd->argv || !cmd->argv[0])
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		free_lists(shell);
		free_env(shell->env);
		exit(127);
	}
	full_path = find_ex_path(shell, cmd->argv[0]);
	if (!full_path)
	{
		printf("command not found\n");
		free_lists(shell);
		free_env(shell->env);
		exit(127);
	}
	return (full_path);
}

void	exec_external_no_fork(t_shell *shell, t_command *cmd)
{
	char	*full_path;

	full_path = prepare_external_exec(shell, cmd);
	if (exec_redir(cmd))
	{
		free(full_path);
		free_lists(shell);
		free_env(shell->env);
		exit(1);
	}
	execve(full_path, cmd->argv, shell->envp);
	handle_exec_error(shell, cmd, full_path);
}
