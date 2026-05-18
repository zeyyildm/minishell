/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanded_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 20:07:23 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/18 20:07:24 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*append_char(char *str, char c)
{
	char	*new_str;
	int		size;
	int		i;

	if (!str)
		return (NULL);
	size = ft_strlen(str);
	new_str = malloc(size + 2);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	free(str);
	return (new_str);
}

char	update_quote_state(char c, char state)
{
	if (c == '\'' && state != '"')
	{
		if (state == '\'')
			return (0);
		return ('\'');
	}
	if (c == '"' && state != '\'')
	{
		if (state == '"')
			return (0);
		return ('"');
	}
	return (state);
}

char	*append_non_special(char *new_arg, char arg_i)
{
	char	*tmp;

	tmp = append_char(new_arg, arg_i);
	if (!tmp)
		return (NULL);
	return (tmp);
}

int	count_non_empty(char **argv)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (argv[i])
	{
		if (argv[i][0] != '\0')
			count++;
		i++;
	}
	return (count);
}

char	**rebuild_argv(char **argv, int count)
{
	char	**new_argv;
	int		i;
	int		j;

	new_argv = malloc(sizeof(char *) * (count + 1));
	if (!new_argv)
		return (NULL);
	i = 0;
	j = 0;
	while (argv[i])
	{
		if (argv[i][0] != '\0')
			new_argv[j++] = argv[i];
		else
			free(argv[i]);
		i++;
	}
	new_argv[j] = NULL;
	free(argv);
	return (new_argv);
}
