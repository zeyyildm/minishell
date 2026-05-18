/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   for_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 20:08:35 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:08:36 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	copy_quotes(char *s, char *line, int *i, int *j)
{
	char	k;

	k = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != k)
		line[(*j)++] = s[(*i)++];
	if (s[*i] == k)
		(*i)++;
}

char	*for_quotes(char *s)
{
	char	*line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	line = malloc(ft_strlen(s) + 1);
	if (!line)
		return (NULL);
	while (s[i])
	{
		if ((s[i] == '"' || s[i] == '\''))
		{
			copy_quotes(s, line, &i, &j);
			continue ;
		}
		line[j++] = s[i++];
	}
	line[j] = '\0';
	return (line);
}

static void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

static char	*join_exec_path(char *path, char *cmd_name)
{
	char	*full_path;
	char	*tmp;

	full_path = ft_strjoin(path, "/");
	tmp = full_path;
	full_path = ft_strjoin(full_path, cmd_name);
	free(tmp);
	return (full_path);
}

char	*find_ex_path(t_shell *shell, char *cmd_name)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd_name, '/'))
		return (ft_strdup(cmd_name));
	path_env = find_path(shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = join_exec_path(paths[i], cmd_name);
		if (access(full_path, X_OK) == 0)
			return (free_paths(paths), full_path);
		free(full_path);
		i++;
	}
	free_paths(paths);
	return (NULL);
}
