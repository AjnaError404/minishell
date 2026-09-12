/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 20:57:22 by ykandous          #+#    #+#             */
/*   Updated: 2026/09/12 23:08:55 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

int	check_lex_pip(t_lexeme *lex)
{
	t_lexeme	*tmp;

	if (!lex)
		return (-1);
	if (lex->type == TYPE_PIPE)
		return (0);
	while (lex)
	{
		if (lex->type == TYPE_PIPE)
		{
			tmp = lex->next;
			while (tmp && is_only_spaces(tmp->value))
				tmp = tmp->next;
			if (!tmp || tmp->type == TYPE_PIPE)
				return (0);
		}
		lex = lex->next;
	}
	return (1);
}

int	check_quotes_closed(char *line)
{
	int	state;
	int	i;

	state = STATE_OUT_QUOTE;
	i = 0;
	while (line && line[i])
	{
		state = update_fsa_state(line[i], state);
		i++;
	}
	if (state != STATE_OUT_QUOTE)
	{
		ft_printf_fd(2, "minishell: syntax error: unclosed quote\n");
		g_exit_status = 2;
		return (0);
	}
	return (1);
}

int	lex_cmd_is_valid(t_lexeme *lex)
{
	if (!lex)
		return (-1);
	while (lex)
	{
		if (lex->type == TYPE_WORD && !check_quotes_closed(lex->value))
			return (0);
		lex = lex->next;
	}
	return (1);
}
