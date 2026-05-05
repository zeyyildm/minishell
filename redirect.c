/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 15:10:10 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/05 15:10:10 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_redir_in(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(filename);
		free(filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	exec_redir_out(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(filename);
		free(filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	exec_redir_append(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(filename);
		free(filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

void	fd_check(t_command *cmd)
{
	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
}

int	exec_redir(t_command *cmd)
{
	t_redir	*tmp;
	char	*filename;

	tmp = cmd->redirs;
	fd_check(cmd);
	while (tmp)
	{
		// Heredoc'u burada handle etme (exec_heredoc yapacak)
		if (tmp->type == T_HEREDOC)
		{
			tmp = tmp->next;
			continue ;
		}
		filename = for_quotes(tmp->file); // Quote'ları temizle
		if (tmp->type == T_REDIR_IN)
			exec_redir_in(filename);
		else if (tmp->type == T_REDIR_OUT)
			exec_redir_out(filename);
		else if (tmp->type == T_REDIR_APPEND)
			exec_redir_append(filename);
		free(filename);
		tmp = tmp->next;
	}
	return (0);
}
