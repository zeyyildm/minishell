/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_ex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 16:16:28 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/09 16:16:29 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

void	exec_external_no_fork(t_shell *shell, t_command *cmd)
{
	char			*full_path;
	int				err;
	struct stat		path_stat;

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
	if (exec_redir(cmd))
	{
		free(full_path);
		free_lists(shell);
		free_env(shell->env);
		exit(1);
	}
	execve(full_path, cmd->argv, shell->envp);
	err = errno;
	stat(cmd->argv[0], &path_stat);
	if (err == EACCES && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(full_path);
		free_lists(shell);
		free_env(shell->env);
		exit(126);
	}
	else
	{
		errno = err;
		perror(cmd->argv[0]);
	}
	free(full_path);
	free_lists(shell);
	free_env(shell->env);
	if (err == ENOENT)
		exit(127);
	if (err == EACCES)
		exit(126);
	exit(127);
}

void	execute_pipe(t_shell *shell, t_command *cmd, int prev_fd)
{
	int		fd[2];
	pid_t	pid;
	int		status;
	int		ret;

	if (cmd->next == NULL)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork hatasi 1");
			return ;
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (exec_redir(cmd) != 0)
			{
				free_lists(shell);
				free_env(shell->env);
				exit(1);
			}
			ret = init_builtin_ex(shell, cmd);
			if (ret == -1)
				exec_external_no_fork(shell, cmd);
			free_lists(shell);
			free_env(shell->env);
			exit(ret);
		}
		if (prev_fd != -1)
			close(prev_fd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
		else
			shell->last_exit_status = 1;
		return ;
	}
	else
	{
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
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			if (exec_redir(cmd) != 0)
			{
				free_lists(shell);
				free_env(shell->env);
				exit(1);
			}
			ret = init_builtin_ex(shell, cmd);
			if (ret == -1)
				exec_external_no_fork(shell, cmd);
			free_lists(shell);
			free_env(shell->env);
			exit(ret);
		}
		if (prev_fd != -1)
			close(prev_fd);
		close(fd[1]);
		execute_pipe(shell, cmd->next, fd[0]);
		close(fd[0]);
		while (waitpid(pid, &status, 0) == -1)
		{
			if (errno != EINTR)
				break ;
		}
	}
}
