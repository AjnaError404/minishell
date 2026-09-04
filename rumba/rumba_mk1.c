/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rumba_mk1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 01:58:15 by ykandous          #+#    #+#             */
/*   Updated: 2026/05/29 16:17:45 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_rumba	*add_rumba(t_rumba **rumba_mk1, void *wilfreed)
{
	t_rumba	*node;

	node = init_rumba_mk1(wilfreed);
	if (!node)
		return (NULL);
	if (!*rumba_mk1)
	{
		*rumba_mk1 = node;
		return (node);
	}
	(*rumba_mk1)->prev = node;
	node->next = *rumba_mk1;
	*rumba_mk1 = node;
	return (node);
}
/*
int	cmpr_data(void *data_one, void *data_two, int n)
{
	int	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n - 1
		&& ((unsigned char *)data_one)[i] == ((unsigned char *)data_two)[i])
		i++;
	return (((unsigned char *)data_one)[i] - ((unsigned char *)data_two)[i]);
}
*/

int	extract_mid_or_end_node(t_rumba **rumba_mk1)
{
	t_rumba	*tmp_node;

	if (rumba_mk1 || *rumba_mk1)
	{
		tmp_node = (*rumba_mk1)->next;
		if ((*rumba_mk1)->prev)
			(*rumba_mk1)->prev->next = tmp_node;
		if (tmp_node)
			tmp_node->prev = (*rumba_mk1)->prev;
		return (1);
	}
	return (0);
}

void	*malloc_rumba(size_t size, t_rumba **rumba_mk1)
{
	void	*wilfreed;

	wilfreed = malloc(size);
	if (!wilfreed)
		return (NULL);
	add_rumba(rumba_mk1, wilfreed);
	return (wilfreed);
}

t_rumba	*init_rumba_mk1(void *wilfreed)
{
	t_rumba	*gc_node;

	gc_node = malloc(sizeof(t_rumba));
	if (!gc_node)
		return (NULL);
	gc_node->wilfreed = wilfreed;
	gc_node->next = NULL;
	gc_node->prev = NULL;
	return (gc_node);
}
