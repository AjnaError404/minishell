/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:52:14 by ykandous          #+#    #+#             */
/*   Updated: 2026/09/12 23:39:47 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_cmd_redir(int type)
{
	if (type >= TYPE_REDIR_IN && type <= TYPE_APPEND)
		return (type);
	return (0);
}

int	is_redir_valid(char **cmd)
{
	int	i;

	if (!cmd)
		return (0);
	i = 0;
	while (cmd[i])
	{
		if (is_cmd_redir(ft_str_is_something(cmd[i])))
		{
			if (cmd[i + 1] == NULL)
				return (0);
			if (is_cmd_redir(ft_str_is_something(cmd[i + 1])))
				return (0);
		}
		i++;
	}
	return (1);
}

static char	*add_spaces_redir(char *cmd, t_rumba **rumba_mk1)
{
	char	*res;
	int		i;
	int		j;
	int		s;

	res = malloc_rumba((ft_strlen(cmd) * 3 + 1), rumba_mk1);
	if (!res)
		return (NULL);
	i = -1;
	j = 0;
	s = STATE_OUT_QUOTE;
	while (cmd[++i])
	{
		s = update_fsa_state(cmd[i], s);
		if (s == STATE_OUT_QUOTE && (cmd[i] == '<' || cmd[i] == '>'))
			res[j++] = ' ';
		res[j++] = cmd[i];
		if (s == STATE_OUT_QUOTE && (cmd[i] == '<' || cmd[i] == '>')
			&& cmd[i + 1] == cmd[i])
			res[j++] = cmd[++i];
		if (s == STATE_OUT_QUOTE && (cmd[i] == '<' || cmd[i] == '>'))
			res[j++] = ' ';
	}
	res[j] = '\0';
	return (res);
}

char	**cmd_to_args(char *cmd, t_rumba **rumba_mk1)
{
	char	**splited;
	char	*spaced;
	int		i;
	int		len;

	spaced = add_spaces_redir(cmd, rumba_mk1);
	if (!spaced)
		return (NULL);
	splited = fls_split(spaced, (char *[]){" ", "\t", NULL}, 0, rumba_mk1);
	if (!splited)
		return (NULL);
	i = 0;
	len = 0;
	while (splited[i])
	{
		if (splited[i][0] != '\0')
			splited[len++] = splited[i];
		i++;
	}
	splited[len] = NULL;
	if (len == 0 || !is_redir_valid(splited))
		return (NULL);
	return (splited);
}

int	shift_2left_str_tab(char **strs, int nshift)
{
	int	i;
	int	j;

	if (!strs)
		return (0);
	i = 0;
	while (i < nshift)
	{
		if (!strs[i])
			return (0);
		i++;
	}
	j = 0;
	while (strs[j + nshift])
	{
		strs[j] = strs[j + nshift];
		j++;
	}
	strs[j] = strs[j + nshift];
	return (1);
}
