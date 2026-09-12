/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 15:08:29 by laaubry           #+#    #+#             */
/*   Updated: 2026/09/12 00:35:56 by laaubry          ###   ########.fr       */
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

	i = 1;
	n_flag = 0;
	while (cmd->args[i] && is_valid_n_flag(cmd->args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (cmd->args[i])
	{
		ft_printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!n_flag)
		ft_printf("\n");
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
