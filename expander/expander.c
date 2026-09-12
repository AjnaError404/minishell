/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:43:34 by ykandous          #+#    #+#             */
/*   Updated: 2026/09/13 00:41:46 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*expand_var(t_shell *shell, char *tree_args, int *j,
		t_rumba **rumba_mk1)
{
	char	*picked;
	char	*expanded;
	char	*replacement;
	t_tab	*index_tab;

	picked = pick_var_name(&tree_args[*j + 1], rumba_mk1);
	if (!picked)
	{
		(*j)++;
		return (tree_args);
	}
	index_tab = init_tab(2, rumba_mk1);
	if (!index_tab)
		return (NULL);
	index_tab->tab[0] = *j;
	index_tab->tab[1] = *j + ft_strlen(picked) + 1;
	if (ft_strcmp(picked, "?") == 0)
		replacement = ft_itoa_gp(shell->status, rumba_mk1);
	else
		replacement = find_envvar(shell->env, picked);
	expanded = graft_str_gp(tree_args, index_tab, replacement, rumba_mk1);
	*j += ft_strlen(replacement);
	return (expanded);
}

int	treenode_expand_args(t_tree **tree, t_shell *shell, t_rumba **rumba_mk1)
{
	int		i;
	int		j;
	char	*var;

	if (!tree || !*tree)
		return (0);
	if (!(*tree)->args)
		return (1);
	i = 0;
	while ((*tree)->args[i])
	{
		j = 0;
		var = find_var((*tree)->args[i], &j);
		while (var)
		{
			(*tree)->args[i] = expand_var(shell, (*tree)->args[i], &j,
					rumba_mk1);
			var = find_var((*tree)->args[i], &j);
		}
		i++;
	}
	return (1);
}

int	treenode_expand_cmd(t_tree **tree, t_shell *shell, t_rumba **rumba_mk1)
{
	int		j;
	char	*var;

	if (!tree || !*tree || !(*tree)->cmd_name)
		return (0);
	j = 0;
	var = find_var((*tree)->cmd_name, &j);
	while (var)
	{
		(*tree)->cmd_name = expand_var(shell, (*tree)->cmd_name, &j,
				rumba_mk1);
		var = find_var((*tree)->cmd_name, &j);
	}
	return (1);
}

int	tree_expand_all(t_tree **tree, t_shell *shell, t_rumba **rumba_mk1)
{
	if (!tree || !*tree)
		return (0);
	if ((*tree)->type == TYPE_PIPE)
	{
		if (!tree_expand_all(&(*tree)->l_child, shell, rumba_mk1))
			return (0);
		if (!tree_expand_all(&(*tree)->r_child, shell, rumba_mk1))
			return (0);
		return (1);
	}
	if (!treenode_expand_args(tree, shell, rumba_mk1))
		return (0);
	if (!treenode_expand_cmd(tree, shell, rumba_mk1))
		return (0);
	return (1);
}

int	update_fsa_state(char c, int state)
{
	if (c == '\'')
		return (state - (state == STATE_SGL_QUOTE)
			+ (state == STATE_OUT_QUOTE));
	else if (c == '\"')
		return (state - 2 * (state == STATE_DBL_QUOTE) + 2
			* (state == STATE_OUT_QUOTE));
	return (state);
}