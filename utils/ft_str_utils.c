/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 18:06:21 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/17 18:06:46 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup_gp(char *s, t_rumba **rumba_mk1)
{
	size_t	sizs;
	size_t	i;
	char	*dup;

	sizs = ft_strlen(s) + 1;
	dup = malloc_rumba(sizs * sizeof(char), rumba_mk1);
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < sizs - 1)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_substr_gp(char *s, unsigned int start, size_t len,
		t_rumba **rumba_mk1)
{
	char	*sub;
	size_t	i;
	size_t	len_s;

	len_s = ft_strlen(s);
	if (len == 0 || start > len_s)
		return (ft_strdup_gp("", rumba_mk1));
	if (len >= len_s - start)
		sub = malloc_rumba((len_s - (size_t)start + 1) * sizeof(char),
				rumba_mk1);
	else
		sub = malloc_rumba((len + 1) * sizeof(char), rumba_mk1);
	if (sub == NULL)
		return (NULL);
	i = 0;
	while (i < len && i <= len_s)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

static int	ft_min(size_t a, size_t b)
{
	if (a < b)
		return (a);
	return (b);
}

size_t	ft_strlcat(char *dst, char *src, size_t siz)
{
	size_t	i;
	size_t	len_dst;
	size_t	len_src;

	len_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	i = len_dst;
	while (i + 1 < siz && i < len_src + len_dst)
	{
		dst[i] = src[i - len_dst];
		i++;
	}
	return (len_src + ft_min(siz, len_dst));
}
