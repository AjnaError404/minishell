/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:41:58 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/08 14:58:02 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_lexeme	*add_lexeme(t_lexeme **lex, char *value, t_rumba **rumba_mk1)
{
	t_lexeme	*lex_node;
	t_lexeme	*tmp_node;

	lex_node = init_lexeme(value, rumba_mk1);
	if (!lex_node)
		return (NULL);
	if (!lex || !*lex)
	{
		*lex = lex_node;
		return (lex_node);
	}
	tmp_node = *lex;
	while (tmp_node->next)
		tmp_node = tmp_node->next;
	lex_node->prev = tmp_node;
	tmp_node->next = lex_node;
	return (lex_node);
}

t_lexeme	*create_lexer(char *args, t_rumba **rumba_mk1)
{
	t_lexeme	*lexer;
	char		**splited;
	int			i;

	lexer = NULL;
	splited = fls_split(args, (char *[]){"|", NULL}, 1, rumba_mk1);
	i = 0;
	while (splited[i])
	{
		if (!add_lexeme(&lexer, splited[i], rumba_mk1))
			return (NULL);
		i++;
	}
	return (lexer);
}

int	count_lexemes(t_lexeme *lex)
{
	int	i;

	i = 0;
	while (lex)
	{
		i++;
		lex = lex->next;
	}
	return (i);
}

char	**fill_str_from_lex(t_lexeme *lexer, t_rumba **rumba_mk1)
{
	int		i;
	char	**args;

	args = malloc_rumba((count_lexemes(lexer) + 1) * sizeof(char *), rumba_mk1);
	if (!args)
		return (NULL);
	i = 0;
	while (lexer)
	{
		args[i++] = lexer->value;
		lexer = lexer->next;
	}
	args[i] = NULL;
	return (args);
}
