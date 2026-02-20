#include <stdio.h>
#include "minishell.h"

void	print_token_type(t_token_type type)
{
	if (type == TWORD)
		printf("TWORD");
	else if (type == TPIPE)
		printf("TPIPE");
	else if (type == T_REDIR_IN)
		printf("T_REDIR_IN");
	else if (type == T_REDIR_OUT)
		printf("T_REDIR_OUT");
	else if (type == T_REDIR_APPEND)
		printf("T_REDIR_APPEND");
	else if (type == T_HEREDOC)
		printf("T_HEREDOC");
	else
		printf("UNKNOWN");
}

void	print_tokens(t_token *head)
{
	while (head)
	{
		printf("Token:\n");
		printf("  value: %s\n", head->value);
		printf("  type: ");
		print_token_type(head->type);
		printf("\n");
		printf("----------------------\n");
		head = head->next;
	}
}
void	print_redirs(t_redir *redir)
{
	while (redir)
	{
		printf("  Redirection:\n");
		printf("    type: ");
		print_token_type(redir->type);
		printf("\n");
		printf("    file: %s\n", redir->file);
		printf("  ------------------\n");
		redir = redir->next;
	}
}
void	print_commands(t_command *cmd)
{
	int	i;

	while (cmd)
	{
		printf("=================================\n");
		printf("Command:\n");

		printf("  argv:\n");
		i = 0;
		if (cmd->argv)
		{
			while (cmd->argv[i])
			{
				printf("    [%d]: %s\n", i, cmd->argv[i]);
				i++;
			}
		}
		else
			printf("    (null)\n");

		printf("  redirections:\n");
		if (cmd->redirs)
			print_redirs(cmd->redirs);
		else
			printf("    (none)\n");

		printf("=================================\n\n");

		cmd = cmd->next;
	}
}
void	print_env_debug(t_env *env)
{
	while (env)
	{
		printf("KEY   : [%s]\n", env->key);
		printf("VALUE : [%s]\n", env->value ? env->value : "NULL");
		printf("--------\n");
		env = env->next;
	}
}
