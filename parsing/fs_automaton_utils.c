/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fs_automaton_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 18:52:27 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 06:50:44 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*is_charset(char *c, char **set)
{
	size_t	i;

	i = 0;
	while (set && set[i])
	{
		if (!ft_strncmp(c, set[i], ft_strlen(set[i])))
			return (set[i]);
		i++;
	}
	return (NULL);
}

int	increment_count_or_not(char c, int state)
{
	if ((state == STATE_SGL_QUOTE && c == '\'') || (state == STATE_DBL_QUOTE
			&& c == '\"'))
		return (1);
	return (0);
}

int	update_state_n_count(char c, int state, ssize_t *count)
{
	*count += increment_count_or_not(c, state);
	return (update_fsa_state(c, state));
}

void	fls_countword_initvat(size_t *i, ssize_t *count, size_t *word,
		int *state)
{
	*i = 0;
	*count = 0;
	*word = 0;
	*state = STATE_OUT_QUOTE;
}
