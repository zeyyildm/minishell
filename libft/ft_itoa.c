/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:42:53 by hakalkan          #+#    #+#             */
/*   Updated: 2025/06/18 18:18:43 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	digit_count(long *num)
{
	int		count;
	long	copy;

	copy = *num;
	count = 0;
	if (copy <= 0)
	{
		count++;
		*num = -*num;
		copy = -copy;
	}
	while (copy > 0)
	{
		count++;
		copy /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		count;
	long	long_num;

	long_num = n;
	count = 0;
	count = digit_count(&long_num);
	if (n < 0)
		n = -1;
	res = (char *)malloc(sizeof(char) * (count + 1));
	if (!res)
		return (NULL);
	res[count--] = '\0';
	while (count >= 0)
	{
		res[count--] = long_num % 10 + '0';
		long_num /= 10;
	}
	if (n == -1)
		res[0] = '-';
	return (res);
}
