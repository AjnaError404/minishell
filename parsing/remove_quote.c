/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 18:03:38 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 12:07:21 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	fix_state_strlen(char *str)
{
	int	state;
	int	newstate;
	int	i;
	int	len;

	state = STATE_OUT_QUOTE;
	i = 0;
	len = 0;
	while (str[i])
	{
		newstate = update_fsa_state(str[i], state);
		if (state == newstate)
			len++;
		state = newstate;
		i++;
	}
	return (len);
}

char	*str_rm_quote(char *str, t_rumba **rumba_mk1)
{
	int		i;
	int		j;
	int		state;
	int		newstate;
	char	*unquoted;

	unquoted = malloc_rumba((fix_state_strlen(str) + 1) * sizeof(char),
			rumba_mk1);
	if (!unquoted)
		return (NULL);
	state = STATE_OUT_QUOTE;
	j = 0;
	i = 0;
	while (str[j])
	{
		newstate = update_fsa_state(str[j], state);
		if (state == newstate)
			unquoted[i++] = str[j++];
		else
			j++;
		state = newstate;
	}
	unquoted[i] = 0;
	return (unquoted);
}

int	args_rm_quote(char **args, t_rumba **rumba_mk1)
{
	int	i;

	if (!args)
		return (0);
	i = 0;
	while (args[i])
	{
		args[i] = str_rm_quote(args[i], rumba_mk1);
		if (!args[i])
			return (0);
		i++;
	}
	return (1);
}

int	remove_quote(t_tree **tree, t_rumba **rumba_mk1)
{
	if (!tree)
		return (0);
	if (is_cmd_opprt((*tree)->type))
	{
		if (!remove_quote(&((*tree)->l_child), rumba_mk1))
			return (0);
		if (!remove_quote(&((*tree)->r_child), rumba_mk1))
			return (0);
	}
	else
	{
		if (!args_rm_quote((*tree)->args, rumba_mk1))
			return (0);
		(*tree)->cmd_name = str_rm_quote((*tree)->cmd_name, rumba_mk1);
		if (!(*tree)->cmd_name)
			return (0);
	}
	return (1);
}
