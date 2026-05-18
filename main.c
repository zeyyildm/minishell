/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zeyildir <zeyildir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:31:21 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:00:33 by zeyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

int	is_redir(int type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_REDIR_APPEND || type == T_HEREDOC);
}

static int	handle_only_redirections(t_shell *shell,
	t_command *cmdHead, char *line)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (1);
	if (exec_redir(cmdHead) == 0)
		shell->last_exit_status = 0;
	else
		shell->last_exit_status = 1;
	free_lists(shell);
	free(line);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_shell		shell;
	t_command	*cmdHead;
	int			saved_stdin;
	int			saved_stdout;
	int			ret;

	shell.envp = envp;
	shell.env = NULL;
	shell.last_exit_status = 0;
	init_env(&shell);
	init_signals();
	shell.tokens = NULL;
	shell.commands = NULL;
	cmdHead = NULL;
	(void)ac;
	(void)av;
	(void)envp;
	while (1)
	{
		line = read_lines();
		if (prechecks(&shell, line))
			continue ;
		shell.tokens = tokenizer(line);
		if (syntax_check(shell.tokens))
		{
			shell.last_exit_status = 2;
			free_lists(&shell);
			free(line);
			continue ;
		}
		cmdHead = parser(shell.tokens, shell.commands);
		shell.commands = cmdHead;
		if (cmdHead)
			expanded(&shell);
		if (cmdHead)
		{
			if (cmdHead->argv[0] == NULL && cmdHead->redirs && !cmdHead->next)
			{
				handle_only_redirections(&shell, cmdHead, line);
				continue ;
			}
			if (heredoc_search(cmdHead))
			{
				if (exec_heredoc(&shell, cmdHead) != 0)
					return (1);
			}
			if (cmdHead->next)
			{
				if (cmdHead->argv[0] && is_parent_builtin(cmdHead->argv[0]))
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(cmdHead->argv[0], 2);
					ft_putstr_fd(": command cannot be used with pipes\n", 2);
					shell.last_exit_status = 1;
					free_lists(&shell);
					free(line);
					continue ;
				}
				execute_pipe(&shell, cmdHead, -1);
			}
			else
			{
				if (cmdHead->argv[0] && is_parent_builtin(cmdHead->argv[0]))
				{
					saved_stdin = dup(STDIN_FILENO);
					saved_stdout = dup(STDOUT_FILENO);
					if (exec_redir(cmdHead) == 0)
					{
						ret = init_builtin_ex(&shell, cmdHead);
						shell.last_exit_status = ret;
					}
					else
						shell.last_exit_status = 1;
					dup2(saved_stdin, STDIN_FILENO);
					dup2(saved_stdout, STDOUT_FILENO);
					close(saved_stdin);
					close(saved_stdout);
				}
				else
				{
					ret = init_builtin_ex(&shell, cmdHead);
					if (ret == -1)
					{
						if (cmdHead->argv && cmdHead->argv[0])
							execute_basic(&shell, cmdHead);
						else
						{
							ft_putstr_fd("minishell: syntax error\n", 2);
							shell.last_exit_status = 127;
						}
					}
					else
						shell.last_exit_status = ret;
				}
			}
		}
		free_lists(&shell);
		free(line);
	}
	return (0);
}
