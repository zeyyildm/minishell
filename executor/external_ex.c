/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_ex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 16:16:24 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:08:09 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_exec_status(t_shell *shell, int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			shell->last_exit_status = 130;
		else if (WTERMSIG(status) == SIGQUIT)
		{
			write(1, "Quit (core dumped)\n", 20);
			shell->last_exit_status = 131;
		}
	}
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else
		shell->last_exit_status = 1;
}

void	handle_execve_error(t_shell *shell, t_command *cmd,
	char **child_envp, char *full_path)
{
	int			err;
	struct stat	path_stat;

	err = errno;
	stat(cmd->argv[0], &path_stat);
	if (err == EACCES && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free_tmp_envp(child_envp);
		free(full_path);
		free_lists(shell);
		free_env(shell->env);
		exit(126);
	}
	errno = err;
	perror(cmd->argv[0]);
	free_tmp_envp(child_envp);
	free(full_path);
	free_lists(shell);
	free_env(shell->env);
	if (err == ENOENT)
		exit(127);
	exit(126);
}

void	exec_child_process(t_shell *shell,
	t_command *cmd, char *full_path)
{
	char	**child_envp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (exec_redir(cmd))
		exit(1);
	child_envp = build_envp(shell->env);
	if (!child_envp)
		exit(1);
	execve(full_path, cmd->argv, child_envp);
	handle_execve_error(shell, cmd, child_envp, full_path);
}

void	exec_parent_process(t_shell *shell,
	pid_t pid, int *status)
{
	while (waitpid(pid, status, 0) == -1)
	{
		if (errno != EINTR)
			break ;
	}
	handle_exec_status(shell, *status);
}

void	execute_basic(t_shell *shell, t_command *cmd)
{
	pid_t	pid;
	int		status;
	char	*full_path;

	status = 0;
	if (!cmd->argv || !cmd->argv[0])
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		shell->last_exit_status = 127;
		return ;
	}
	full_path = find_ex_path(shell, cmd->argv[0]);
	if (!full_path)
		return ((void)command_not_found(shell, cmd->argv[0]));
	pid = fork();
	if (pid < 0)
	{
		shell->last_exit_status = 1;
		return (perror("fork"), free(full_path));
	}
	if (pid == 0)
		exec_child_process(shell, cmd, full_path);
	else
		exec_parent_process(shell, pid, &status);
	free(full_path);
}
