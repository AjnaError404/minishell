/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:52:14 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 12:18:48 by laaubry          ###   ########.fr       */
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

int	switch_open(char *pathname, int redir_type)
{
	int	fd;

	fd = -1;
	if (redir_type == TYPE_REDIR_IN)
		fd = open(pathname, O_RDONLY);
	else if (redir_type == TYPE_REDIR_OUT)
		fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir_type == TYPE_APPEND)
		fd = open(pathname, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

char	**cmd_to_args(char *cmd, t_rumba **rumba_mk1)
{
	char	**splited;
	int		i;
	int		len;

	splited = fls_split(cmd, (char *[]){" ", "\t", NULL}, 0, rumba_mk1);
	if (!splited)
		return (NULL);
	i = 0;
	len = 0;
	while (splited[i])
	{
		if (splited[i][0] != '\0')
		{
			splited[len] = splited[i];
			len++;
		}
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
