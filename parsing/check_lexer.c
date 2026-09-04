/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 20:57:22 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/23 19:47:43 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_lex_pip(t_lexeme *lex)
{
	if (!lex)
		return (-1);
	if (lex->type == TYPE_PIPE)
		return (0);
	while (lex)
	{
		if (lex->type == TYPE_PIPE && (lex->next == NULL
				|| lex->next->type == TYPE_PIPE))
			return (0);
		lex = lex->next;
	}
	return (1);
}

int	str_is_quote_closed(char *cmd)
{
	int	i;
	int	f_closed_quote;

	f_closed_quote = 1;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\"')
		{
			if (f_closed_quote)
				f_closed_quote = 0;
			else
				f_closed_quote = 1;
		}
		i++;
	}
	return (f_closed_quote);
}

int	lex_cmd_is_valid(t_lexeme *lex)
{
	if (!lex)
		return (-1);
	while (lex)
	{
		if (lex->type == TYPE_WORD && !str_is_quote_closed(lex->value))
			return (0);
		lex = lex->next;
	}
	return (1);
}
