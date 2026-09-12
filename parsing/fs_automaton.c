/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fs_automaton.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 20:03:39 by ykandous          #+#    #+#             */
/*   Updated: 2026/09/12 23:08:18 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	fls_countword(char *str, char **set, int add_sep)
{
	size_t	count;
	char	*sep;

	count = 0;
	while (str && *str)
	{
		sep = is_charset(str, set);
		if (!sep)
		{
			count++;
			str += fls_wordsize(str, set, STATE_OUT_QUOTE);
		}
		else
		{
			if (add_sep)
				count++;
			str += ft_strlen(sep);
		}
	}
	return (count);
}

int	fls_wordsize(char *str, char **set, int state)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		state = update_fsa_state(str[i], state);
		if (state == STATE_OUT_QUOTE && is_charset(&str[i], set))
			break ;
		i++;
	}
	return (i);
}

char	*fls_fillword(char *str, char **set, int state, t_rumba **rumba_mk1)
{
	size_t	i;
	size_t	end_word;
	char	*word;

	i = 0;
	end_word = fls_wordsize(str, set, state);
	word = malloc_rumba((end_word + 1) * sizeof(char), rumba_mk1);
	if (!word)
		return (NULL);
	while (*str && i < end_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

int	handle_sep(char *sep, int add_sep, char **splited, int *j)
{
	if (add_sep)
	{
		splited[*j] = sep;
		(*j)++;
	}
	return (ft_strlen(sep));
}

char	**fls_split(char *str, char **set, int add_sep, t_rumba **rumba_mk1)
{
	int		j;
	int		state;
	char	**splited;
	char	*sep;

	splited = malloc_rumba((fls_countword(str, set, add_sep) + 1)
			* sizeof(char *), rumba_mk1);
	if (!splited)
		return (NULL);
	state = STATE_OUT_QUOTE;
	j = 0;
	while (str && *str)
	{
		sep = is_charset(str, set);
		if (!sep)
		{
			splited[j++] = fls_fillword(str, set, state, rumba_mk1);
			str += fls_wordsize(str, set, state);
		}
		else
			str += handle_sep(sep, add_sep, splited, &j);
	}
	splited[j] = NULL;
	return (splited);
}
