/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 06:35:14 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/15 16:44:50 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_put_unsigned_nbr_base_fd(unsigned int nb, char *base, int fd)
{
	int	cmp;

	cmp = 1;
	ft_put_pos_base_fd(nb, base, &cmp, fd);
	return (cmp);
}

int	ft_put_ptr_fd(void *ptr, int fd)
{
	int	cmp;

	if (!ptr)
	{
		ft_putstr_fd("(nil)", fd);
		return (5);
	}
	cmp = ft_putstr_fd("0x", fd) + 1;
	ft_put_pos_base_fd((unsigned long)ptr, "0123456789abcdef", &cmp, fd);
	return (cmp);
}

void	ft_switch_case_fd(char convert, va_list params, int *cmp, int fd)
{
	if (convert == 'c')
		*cmp += ft_putchar_fd(va_arg(params, int), fd);
	else if (convert == 's')
		*cmp += ft_putstr_fd(va_arg(params, char *), fd);
	else if (convert == 'p')
		*cmp += ft_put_ptr_fd(va_arg(params, void *), fd);
	else if (convert == 'd' || convert == 'i')
		*cmp += ft_putbase_fd(va_arg(params, int), "0123456789", fd);
	else if (convert == 'u')
		*cmp += ft_put_unsigned_nbr_base_fd(va_arg(params, unsigned int),
				"0123456789", fd);
	else if (convert == 'x')
		*cmp += ft_put_unsigned_nbr_base_fd(va_arg(params, unsigned int),
				"0123456789abcdef", fd);
	else if (convert == 'X')
		*cmp += ft_put_unsigned_nbr_base_fd(va_arg(params, unsigned int),
				"0123456789ABCDEF", fd);
	else
		*cmp += ft_putchar_fd(convert, fd);
}

int	ft_printf_fd(int fd, const char *format, ...)
{
	va_list	params;
	int		i;
	int		cmp;

	i = 0;
	cmp = 0;
	if (!format)
		return (0);
	va_start(params, format);
	while (format[i])
	{
		if (format[i] == '%')
			ft_switch_case_fd(format[++i], params, &cmp, fd);
		else
		{
			ft_putchar_fd(format[i], fd);
			cmp++;
		}
		i++;
	}
	va_end(params);
	return (cmp);
}

int	ft_printf(const char *format, ...)
{
	va_list	params;
	int		i;
	int		cmp;

	i = 0;
	cmp = 0;
	if (!format)
		return (0);
	va_start(params, format);
	while (format[i])
	{
		if (format[i] == '%')
			ft_switch_case_fd(format[++i], params, &cmp, STDOUT_FILENO);
		else
		{
			ft_putchar_fd(format[i], STDOUT_FILENO);
			cmp++;
		}
		i++;
	}
	va_end(params);
	return (cmp);
}
