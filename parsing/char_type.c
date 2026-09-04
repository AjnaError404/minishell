/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_type.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 10:46:39 by ykandous          #+#    #+#             */
/*   Updated: 2026/05/29 11:02:15 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalpha(char c)
{
	if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
		return (1);
	return (0);
}

int	ft_isall_alpha(char *str)
{
	while (*str)
	{
		if (!ft_isalpha(*str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_isdigit(char c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

int	ft_isall_word(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str) && !ft_isalpha(*str) && *str != '-' && *str != ' '
			&& *str != '\'' && *str != '\"' && *str != '_')
		{
			return (0);
		}
		str++;
	}
	return (1);
}

int	ft_char_is_something(char c)
{
	if (ft_isalpha(c) || ft_isdigit(c) || c == '-')
		return (TYPE_WORD);
	if (c == 39)
		return (TYPE_QUOTE);
	if (c == -1)
		return (TYPE_EOF);
	if (c == '\"')
		return (TYPE_DBLQUOTE);
	if (c == '<')
		return (TYPE_REDIR_IN);
	if (c == '>')
		return (TYPE_REDIR_OUT);
	if (c == '|')
		return (TYPE_PIPE);
	return (TYPE_UNDEFINE);
}
