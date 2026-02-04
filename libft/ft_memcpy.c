/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:32:14 by hakalkan          #+#    #+#             */
/*   Updated: 2025/06/21 20:35:22 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*de;
	char	*se;
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	de = dest;
	se = (char *)src;
	while (i < n)
	{
		de[i] = se[i];
		i++;
	}
	return (dest);
}
