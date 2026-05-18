/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:43:08 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:08:12 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	create_pipe(int fd[2], pid_t *pid)
{
	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	*pid = fork();
	if (*pid < 0)
		return (perror("fork"), 1);
	return (0);
}

static void	heredoc_child(t_shell *shell, t_redir *redir, int fd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd[0]);
	handle_heredoc(shell, redir, fd);
	close(fd[1]);
	exit(0);
}

static void	heredoc_parent(t_command *cmd_tmp, int fd[2], pid_t pid)
{
	close(fd[1]);
	waitpid(pid, NULL, 0);
	if (cmd_tmp->heredoc_fd != -1)
		close(cmd_tmp->heredoc_fd);
	cmd_tmp->heredoc_fd = fd[0];
}

int	exec_heredoc(t_shell *shell, t_command *cmd)
{
	t_command	*cmd_tmp;
	t_redir		*redir;
	int			fd[2];
	pid_t		pid;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		redir = cmd_tmp->redirs;
		while (redir)
		{
			if (redir->type == T_HEREDOC)
			{
				if (create_pipe(fd, &pid))
					return (1);
				if (pid == 0)
					heredoc_child(shell, redir, fd);
				else
					heredoc_parent(cmd_tmp, fd, pid);
			}
			redir = redir->next;
		}
		cmd_tmp = cmd_tmp->next;
	}
	return (0);
}
