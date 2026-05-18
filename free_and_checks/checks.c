#include "../minishell.h"

void	free_lists(t_shell *shell)
{
	if (!shell)
		return ;
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	free_commands(shell->commands);
	shell->commands = NULL;
}

int	check_redir(t_token *tmp)
{
	if (!is_redir(tmp->type))
		return (0);
	if (!tmp->next)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
			2);
		return (1);
	}
	if (tmp->next->type != TWORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(tmp->next->value, 2);
		ft_putstr_fd("'\n", 2);
		return (1);
	}
	return (0);
}

int	check_pipe_error(t_token *tmp, t_token *head)
{
	if (tmp->type != TPIPE)
		return (0);
	if (tmp == head || !tmp->next || tmp->next->type == TPIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

int	syntax_check(t_token *t)
{
	t_token	*tmp;

	if (!t)
		return (0);

	if (t->type == TPIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}

	tmp = t;
	while (tmp)
	{
		if(check_pipe_error(tmp, t) || check_redir(tmp))
			return(1);
		tmp = tmp->next;
	}
	return (0);
}

int prechecks(t_shell *shell, char *line)
{
	if (!line)
	{
		write(1, "exit\n", 5);
		rl_clear_history();
		free_lists(shell);
		free_env(shell->env);
		exit(0);
	}
	if (line_check_quote(line))
	{
		ft_putstr_fd("minishell: unclosed quote\n", 2);
		shell->last_exit_status = 2;
		free(line);
		return (1);
	}
	if (is_only_spaces(line))
	{
		free(line);
		return (1);
	}
	return (0);
}
