/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:30:45 by hakalkan          #+#    #+#             */
/*   Updated: 2025/06/21 20:35:36 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_word(char const *s, char c)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			in_word = 0;
		else
		{
			if (in_word == 0)
			{
				count++;
				in_word = 1;
			}
		}
		i++;
	}
	return (count);
}

int	head_word(char const *s, char c, int start)
{
	while (s[start] && s[start] == c)
		start++;
	if (s[start] != '\0')
		return (start);
	return (-1);
}

int	last_word(char const *s, char c, int start)
{
	while (s[start] && s[start] != c)
		start++;
	return (start);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		start;
	int		end;
	char	**res;
	int		w;

	i = 0;
	w = 0;
	if (!s)
		return (NULL);
	res = malloc(sizeof(char *) * (count_word(s, c) + 1));
	if (!res)
		return (NULL);
	while (w < count_word(s, c))
	{
		start = head_word(s, c, i);
		if (start == -1)
			break ;
		end = last_word(s, c, start);
		res[w++] = ft_substr(s, start, end - start);
		i = end;
	}
	res[w] = NULL;
	return (res);
}
