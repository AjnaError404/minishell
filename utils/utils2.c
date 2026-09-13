/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 14:37:28 by ykandous          #+#    #+#             */
/*   Updated: 2026/09/13 22:06:54 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n - 1 && (s1[i] == s2[i]) && (s1[i] != 0 && s2[i] != 0))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] == s2[i]) && (s1[i] != 0 && s2[i] != 0))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strjoin_gp(char *s1, char *s2, t_rumba **rumba_mk1)
{
	char	*res;
	size_t	i;
	size_t	len_s1;

	len_s1 = ft_strlen(s1);
	res = malloc_rumba((len_s1 + ft_strlen(s2) + 1) * sizeof(char), rumba_mk1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != 0)
	{
		res[i] = s1[i];
		i++;
	}
	while (s2[i - len_s1] != 0)
	{
		res[i] = s2[i - len_s1];
		i++;
	}
	res[i] = '\0';
	return (res);
}

int	is_valid_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
