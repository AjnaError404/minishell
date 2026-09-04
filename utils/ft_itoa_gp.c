/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:10:23 by ykandous          #+#    #+#             */
/*   Updated: 2025/11/14 20:42:25 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_sizenbr(int n)
{
	size_t	siz;

	siz = 0;
	while (n / 10)
	{
		siz++;
		n = n / 10;
	}
	return (siz + 1);
}

char	*ft_fillnbr(int sign, int i, char *res, long n)
{
	while (i >= sign)
	{
		res[i] = '0' + n % 10;
		n = n / 10;
		i--;
	}
	if (sign == 1)
		res[i] = '-';
	return (res);
}

char	*ft_itoa_gp(int n, t_rumba **rumba_mk1)
{
	char	*res;
	size_t	siz;
	int		sign;
	int		i;
	long	nb;

	sign = 0;
	nb = n;
	if (n < 0)
	{
		sign++;
		nb = -1 * nb;
	}
	siz = ft_sizenbr(nb);
	res = malloc_rumba((siz + sign + 1) * sizeof(char), rumba_mk1);
	if (res == NULL)
		return (NULL);
	i = siz + sign - 1;
	res[siz + sign] = '\0';
	res = ft_fillnbr(sign, i, res, nb);
	return (res);
}
/*
#include <limits.h>
#include <stdio.h>

int	main(void)
{
	char	*s;

	s = ft_itoa(INT_MIN);
	if (!ft_strncmp(s, "-2147483648", 100))
		printf("OK");
}
*/
