#include "../minishell.h"

void	echo_helper(t_shell *shell, t_command *cmd)
{
	int	i;
	int	newline;

	if (exec_redir(cmd) != 0)
	{
		free_lists(shell);
		free_env(shell->env);
		exit(1);
	}
	i = 1;
	newline = parse_newline(cmd, &i);
	while (cmd->argv[i])
	{
		printf("%s", cmd->argv[i]);
		if (cmd->argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline == 1)
		printf("\n");
	free_lists(shell);
	free_env(shell->env);
	exit(0);
}

int	parse_newline(t_command *cmd, int *i)
{
	int	j;
	int	newline;

	newline = 1;
	while (cmd->argv[*i] && cmd->argv[*i][0] == '-'
		&& cmd->argv[*i][1] == 'n')
	{
		j = 1;
		while (cmd->argv[*i][j] == 'n')
			j++;
		if (cmd->argv[*i][j] != '\0')
			break ;
		newline = 0;
		*i += 1;
	}
	return (newline);
}

void	pwd_helper(t_shell *shell, t_command *cmd)
{
	char	*path;

	if (exec_redir(cmd) != 0)
		exit(1);
	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("pwd error");
		free_lists(shell);
		free_env(shell->env);
		exit(1);
	}
	printf("%s\n", path);
	free(path);
	free_lists(shell);
	free_env(shell->env);
	exit(0);
}
