/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:11:46 by hakalkan          #+#    #+#             */
/*   Updated: 2025/06/20 14:24:25 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_findchar(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;
	char	*s2;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	while (s1[i] && ft_findchar(s1[i], set))
		i++;
	len = ft_strlen(s1);
	if (len == 0)
		return (ft_strdup(""));
	len--;
	while (len > i && ft_findchar(s1[len], set))
		len--;
	if (len < i)
		return (ft_strdup(""));
	s2 = ft_substr(s1, i, len - i + 1);
	return (s2);
}
