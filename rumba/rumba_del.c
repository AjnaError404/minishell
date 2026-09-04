/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rumba_del.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:09:03 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 19:09:22 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	del_one_rumba(t_rumba **rumba_mk1, void *wilfreed)
{
	t_rumba	*tmp;

	if (!rumba_mk1 || !*rumba_mk1 || !wilfreed)
		return (-1);
	tmp = *rumba_mk1;
	while (tmp)
	{
		if (tmp->wilfreed == wilfreed)
		{
			if (tmp == *rumba_mk1)
			{
				if (tmp->next)
					tmp->next->prev = NULL;
				*rumba_mk1 = tmp->next;
			}
			else
				extract_mid_or_end_node(&tmp);
			free(tmp->wilfreed);
			free(tmp);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	del_first_rumba(t_rumba **rumba_mk1, void *wilfreed)
{
	t_rumba	*tmp;

	if (!rumba_mk1 || !*rumba_mk1 || !wilfreed)
		return (-1);
	tmp = *rumba_mk1;
	if (tmp->wilfreed == wilfreed)
	{
		if (tmp->next != NULL)
			tmp->next->prev = NULL;
		*rumba_mk1 = tmp->next;
		free(tmp->wilfreed);
		free(tmp);
		return (1);
	}
	return (0);
}

int	del_all_rumba(t_rumba **rumba_mk1)
{
	t_rumba	*tmp_node;

	if (!rumba_mk1 || !*rumba_mk1)
		return (-1);
	tmp_node = *rumba_mk1;
	while (tmp_node)
	{
		*rumba_mk1 = tmp_node;
		tmp_node = tmp_node->next;
		if (*rumba_mk1 != NULL)
		{
			if ((*rumba_mk1)->wilfreed != NULL)
				free((*rumba_mk1)->wilfreed);
			free(*rumba_mk1);
		}
	}
	*rumba_mk1 = NULL;
	return (1);
}
