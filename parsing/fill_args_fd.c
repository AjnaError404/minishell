/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_args_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:53:57 by ykandous          #+#    #+#             */
/*   Updated: 2026/09/11 20:25:16 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_str_redir(char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "<") == 0 || ft_strcmp(str, ">") == 0 || ft_strcmp(str,
			"<<") == 0 || ft_strcmp(str, ">>") == 0)
		return (1);
	return (0);
}

int	switch_redir(char *name, char *symbol, t_rumba **rumba_mk1)
{
	int	fd_opened;

	fd_opened = -1;
	if (ft_strcmp(symbol, "<<") == 0)
		fd_opened = heredoc_magic(name, rumba_mk1);
	else if (ft_strcmp(symbol, "<") == 0)
		fd_opened = open(name, O_RDONLY);
	else if (ft_strcmp(symbol, ">") == 0)
		fd_opened = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strcmp(symbol, ">>") == 0)
		fd_opened = open(name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_opened == -1)
	{
		ft_printf_fd(2, "minishell: %s: ", name);
		perror("");
	}
	return (fd_opened);
}

int	treenode_add_fd(t_tree **tree_node, t_rumba **rumba_mk1, int i)
{
	int		fd;
	char	*sym;

	sym = (*tree_node)->args[i];
	if (!is_str_redir(sym))
		return (i + 1);
	fd = switch_redir((*tree_node)->args[i + 1], sym, rumba_mk1);
	if (fd == -1 || fd == -2)
		return (fd);
	if (sym[0] == '<')
	{
		if ((*tree_node)->fd_in != -2)
			close((*tree_node)->fd_in);
		(*tree_node)->fd_in = fd;
	}
	else
	{
		if ((*tree_node)->fd_out != -2)
			close((*tree_node)->fd_out);
		(*tree_node)->fd_out = fd;
	}
	shift_2left_str_tab(&(*tree_node)->args[i], 2);
	return (i);
}

int	treenode_add_args_n_fd(t_tree **tree_node, t_rumba **rumba_mk1)
{
	int	i;

	(*tree_node)->args = cmd_to_args((*tree_node)->cmd_name, rumba_mk1);
	if (!(*tree_node)->args)
		return (0);
	i = 0;
	while ((*tree_node)->args[i])
	{
		i = treenode_add_fd(tree_node, rumba_mk1, i);
		if (i == -1 || i == -2)
		{
			(*tree_node)->args[0] = NULL;
			return (0);
		}
	}
	return (1);
}

int	tree_update_args_n_fd(t_tree **tree, t_rumba **rumba_mk1)
{
	if (!tree || !*tree)
		return (0);
	if ((*tree)->type == TYPE_PIPE)
	{
		if (!tree_update_args_n_fd(&(*tree)->l_child, rumba_mk1))
			return (0);
		if (!tree_update_args_n_fd(&(*tree)->r_child, rumba_mk1))
			return (0);
		return (1);
	}
	if (!treenode_add_args_n_fd(tree, rumba_mk1))
		return (0);
	return (1);
}
