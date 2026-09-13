/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 17:34:00 by laaubry           #+#    #+#             */
/*   Updated: 2026/09/13 22:03:29 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exit(t_tree *cmd, t_shell *shell, t_rumba **rumba_mk1)
{
	int	status;

	ft_printf("exit\n");
	if (cmd->args[1] != NULL)
	{
		if (is_numeric(cmd->args[1]) == 0)
		{
			ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n",
				cmd->args[1]);
			clean_child_exit(2, shell->env, rumba_mk1);
		}
		if (cmd->args[2] != NULL)
		{
			ft_printf_fd(2, "minishell: exit: too many arguments\n");
			return (1);
		}
		status = ft_atoi(cmd->args[1]);
		clean_child_exit(status, shell->env, rumba_mk1);
	}
	clean_child_exit(shell->status, shell->env, rumba_mk1);
	return (0);
}

int	ft_unset(char **env, t_tree *cmd)
{
	int	i;
	int	j;
	int	len;

	j = 0;
	while (cmd->args[++j])
	{
		i = -1;
		len = ft_strlen(cmd->args[j]);
		while (env[++i])
		{
			if (!ft_strncmp(env[i], cmd->args[j], len) && env[i][len] == '=')
			{
				free(env[i]);
				while (env[i])
				{
					env[i] = env[i + 1];
					i++;
				}
				break ;
			}
		}
	}
	return (0);
}

char	**ft_export(char **envp, t_tree *cmd)
{
	int	len;
	int	i;

	if (cmd->args[1] == NULL)
	{
		env(envp);
		return (envp);
	}
	i = 1;
	while (cmd->args[i])
	{
		len = 0;
		while (cmd->args[i][len] && cmd->args[i][len] != '=')
			len++;
		if (cmd->args[i][len] == '=')
		{
			if (!update_env_var(envp, cmd->args[i], len))
				envp = add_env_var(envp, cmd->args[i]);
		}
		i++;
	}
	return (envp);
}

int	update_env_var(char **env, char *arg, int len)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], arg, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			env[i] = ft_strdup(arg);
			return (1);
		}
		i++;
	}
	return (0);
}

char	**add_env_var(char **env, char *arg)
{
	int		i;
	int		count;
	char	**new_env;

	count = count_variables(env);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[count] = ft_strdup(arg);
	new_env[count + 1] = NULL;
	free(env);
	return (new_env);
}
