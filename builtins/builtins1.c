/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 15:08:29 by laaubry           #+#    #+#             */
/*   Updated: 2026/08/30 07:43:42 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd(void)
{
	char	buffer[4096];
	char	*result;

	result = getcwd(buffer, 1024);
	if (result == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_printf("%s\n", buffer);
	return (0);
}

int	cd(t_tree *cmd)
{
	int	result;

	if (cmd->args[1] == NULL)
	{
		ft_printf_fd(2, "minishell: cd: need an argument\n");
		return (1);
	}
	result = chdir(cmd->args[1]);
	if (result == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	echo(t_tree *cmd)
{
	int	i;
	int	n_flag;
	int	fd_out;

	i = 1;
	n_flag = 0;
	fd_out = 1;
	if (cmd->fd_out != -2)
		fd_out = cmd->fd_out;
	if (cmd->args[1] != NULL && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		n_flag = 1;
		i = 2;
	}
	while (cmd->args[i] != NULL)
	{
		ft_printf_fd(fd_out, "%s", cmd->args[i]);
		if (cmd->args[i + 1] != NULL)
			ft_printf_fd(fd_out, " ");
		i++;
	}
	if (n_flag == 0)
		ft_printf_fd(fd_out, "\n");
	return (0);
}

int	env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		ft_printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
