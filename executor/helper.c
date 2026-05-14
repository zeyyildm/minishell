#include "../minishell.h"

int	heredoc_break(char *line, char *delimiter)
{
	if (!line)
	{
		printf("warning: here-document delimited by end-of-file\n");
		return (1);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& ft_strlen(line) == ft_strlen(delimiter))
	{
		free(line);
		return (1);
	}
	return (0);
}

void	heredoc_write(t_shell *shell, char **line, int *fd, int expand)
{
	char	*tmp;

	if (expand)
	{
		tmp = expand_arg(shell, *line);
		free(*line);
		*line = tmp;
	}
	write(fd[1], *line, ft_strlen(*line));
	write(fd[1], "\n", 1);
	free(*line);
}

void	handle_heredoc(t_shell *shell, t_redir *redir, int *fd)
{
	char	*line;
	char	*delimiter;
	int		expand;

	delimiter = for_quotes(redir->file);
	if (ft_strchr(redir->file, '\'') || ft_strchr(redir->file, '"'))
		expand = 0;
	else
		expand = 1;
	while (1)
	{
		line = readline("> ");
		if (heredoc_break(line, delimiter))
			break ;
		heredoc_write(shell, &line, fd, expand);
	}
	free(delimiter);
}
