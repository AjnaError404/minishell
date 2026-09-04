/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 06:13:57 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/23 20:11:17 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_putchar_fd(int c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_putstr_fd(char *str, int fd)
{
	int	len;

	if (!str)
		return (ft_putstr_fd("(null)", fd));
	len = ft_strlen(str);
	write(fd, str, len);
	return (len);
}

void	ft_put_pos_base_fd(unsigned long nb, char *base, int *cmp, int fd)
{
	int		len;
	char	n;

	len = ft_strlen(base);
	if (nb / len != 0)
	{
		(*cmp)++;
		ft_put_pos_base_fd(nb / len, base, cmp, fd);
	}
	n = base[nb % len];
	write(fd, &n, 1);
}

int	ft_putbase_fd(int nb, char *base, int fd)
{
	int	cmp;

	cmp = 1;
	if (nb == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return (11);
	}
	else
	{
		if (nb < 0)
		{
			write(fd, "-", 1);
			cmp++;
			ft_put_pos_base_fd(-1 * nb, base, &cmp, fd);
		}
		else
			ft_put_pos_base_fd(nb, base, &cmp, fd);
	}
	return (cmp);
}
