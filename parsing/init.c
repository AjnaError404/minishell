/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 13:03:48 by ykandous          #+#    #+#             */
/*   Updated: 2026/05/29 11:19:00 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_charcmd	*init_charcmd(char value, t_rumba **rumba_mk1)
{
	t_charcmd	*cc_node;

	cc_node = malloc_rumba(sizeof(t_charcmd), rumba_mk1);
	if (!cc_node)
		return (NULL);
	cc_node->type = ft_char_is_something(value);
	cc_node->value = value;
	return (cc_node);
}

t_lexeme	*init_lexeme(char *value, t_rumba **rumba_mk1)
{
	t_lexeme	*lx_node;

	lx_node = malloc_rumba(sizeof(t_lexeme), rumba_mk1);
	if (!lx_node)
		return (NULL);
	lx_node->type = ft_str_is_something(value);
	lx_node->value = value;
	lx_node->next = NULL;
	lx_node->prev = NULL;
	return (lx_node);
}

t_tree	*init_tree(char *value, int type, t_rumba **rumba_mk1)
{
	t_tree	*tree_node;

	tree_node = malloc_rumba(sizeof(t_tree), rumba_mk1);
	if (!tree_node)
		return (NULL);
	tree_node->cmd_name = value;
	tree_node->type = type;
	tree_node->fd_in = -2;
	tree_node->fd_out = -2;
	tree_node->r_child = NULL;
	tree_node->l_child = NULL;
	return (tree_node);
}

t_tab	*init_tab(int size, t_rumba **rumba_mk1)
{
	t_tab	*tab;

	tab = malloc_rumba(sizeof(t_tab), rumba_mk1);
	if (!tab)
		return (NULL);
	tab->tab = malloc_rumba(size * sizeof(int), rumba_mk1);
	if (!tab->tab)
	{
		del_one_rumba(rumba_mk1, tab);
		return (NULL);
	}
	tab->size = size;
	return (tab);
}
