/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 22:28:53 by ykandous          #+#    #+#             */
/*   Updated: 2026/09/12 01:03:04 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_lexeme	*find_oprt(t_lexeme *lexer)
{
	while (lexer)
	{
		if (is_cmd_opprt(lexer->type))
			return (lexer);
		lexer = lexer->next;
	}
	return (NULL);
}

int	find_lex_childs(t_lexeme **root, t_lexeme **l_child, t_lexeme **r_child)
{
	*root = find_oprt(*l_child);
	if (!*root)
		return (0);
	if ((*root)->prev)
		(*root)->prev->next = NULL;
	else
		*l_child = NULL;
	if ((*root)->next)
	{
		*r_child = (*root)->next;
		(*r_child)->prev = NULL;
	}
	else
		*r_child = NULL;
	return (1);
}

t_tree	*lex_to_tree(t_lexeme *lexer, t_rumba **rumba_mk1)
{
	t_lexeme	*root;
	t_lexeme	*r_child;
	t_tree		*tree;

	if (!lexer)
		return (NULL);
	if (!find_lex_childs(&root, &lexer, &r_child))
	{
		tree = init_tree(lexer->value, lexer->type, rumba_mk1);
		if (!tree)
			return (NULL);
		tree->args = fill_str_from_lex(lexer, rumba_mk1);
		if (!tree->args)
			return (NULL);
	}
	else
	{
		tree = init_tree(root->value, root->type, rumba_mk1);
		if (!tree)
			return (NULL);
		tree->l_child = lex_to_tree(lexer, rumba_mk1);
		tree->r_child = lex_to_tree(r_child, rumba_mk1);
	}
	return (tree);
}


t_tree	*find_child(t_lexeme *r_child, t_rumba **rumba_mk1, t_tree *tree)
{
	t_lexeme	*root;
	t_lexeme	*l_child;

	if (find_lex_childs(&root, &l_child, &r_child))
	{
		tree = init_tree(root->value, root->type, rumba_mk1);
		if (!tree)
			return (NULL);
		tree->l_child = init_tree(l_child->value, l_child->type, rumba_mk1);
		if (!(tree->l_child))
			return (NULL);
		tree->r_child = find_child(r_child, rumba_mk1, tree);
	}
	else
	{
		tree->r_child = init_tree(root->value, root->type, rumba_mk1);
		if (!(tree->r_child))
			return (NULL);
	}
	return (tree->r_child);
}

t_tree	*create_tree(t_lexeme *lexer, char **envp, t_rumba **rumba_mk1)
{
	t_tree	*tree;

	if (!lexer)
		return (NULL);
	tree = lex_to_tree(lexer, rumba_mk1);
	if (!tree)
		return (NULL);
	if (!tree_update_args_n_fd(&tree, rumba_mk1))
		return (NULL);
	if (!tree_expand_all(&tree, envp, rumba_mk1))
		return (NULL);
	if (!remove_quote(&tree, rumba_mk1))
		return (NULL);
	return (tree);
}
