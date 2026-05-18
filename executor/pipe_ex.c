/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_ex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zeyildir <zeyildir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 16:16:28 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 18:22:19 by zeyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	cleanup_and_exit(t_shell *shell, int ret)
{
	free_lists(shell);
	free_env(shell->env);
	exit(ret);
}

static void	execute_child(t_shell *shell, t_command *cmd, int prev_fd,
		int *fd)
{
	int	ret;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (fd)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	if (exec_redir(cmd) != 0)
		cleanup_and_exit(shell, 1);
	ret = init_builtin_ex(shell, cmd);
	if (ret == -1)
		exec_external_no_fork(shell, cmd);
	cleanup_and_exit(shell, ret);
}

static void	execute_last_command(t_shell *shell, t_command *cmd, int prev_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork hatasi 1");
		return ;
	}
	if (pid == 0)
		execute_child(shell, cmd, prev_fd, NULL);
	if (prev_fd != -1)
		close(prev_fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else
		shell->last_exit_status = 1;
}

static void	execute_middle_command(t_shell *shell, t_command *cmd, int prev_fd)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
	{
		perror("pipe fd hatasi");
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork hatasi 2");
		return ;
	}
	if (pid == 0)
		execute_child(shell, cmd, prev_fd, fd);
	if (prev_fd != -1)
		close(prev_fd);
	close(fd[1]);
	execute_pipe(shell, cmd->next, fd[0]);
	close(fd[0]);
	while (waitpid(pid, &status, 0) == -1)
		if (errno != EINTR)
			break ;
}

void	execute_pipe(t_shell *shell, t_command *cmd, int prev_fd)
{
	if (cmd->next == NULL)
		execute_last_command(shell, cmd, prev_fd);
	else
		execute_middle_command(shell, cmd, prev_fd);
}
