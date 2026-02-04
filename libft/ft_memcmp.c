/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:25:55 by hakalkan          #+#    #+#             */
/*   Updated: 2025/06/21 19:36:19 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*d;
	unsigned char	*a;
	size_t			i;

	d = (unsigned char *)s1;
	a = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (d[i] != a[i])
			return (d[i] - a[i]);
		i++;
	}
	return (0);
}
