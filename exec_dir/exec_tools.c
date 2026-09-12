/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 00:26:24 by laaubry           #+#    #+#             */
/*   Updated: 2026/09/12 14:00:02 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_child_process(t_tree **cmd, char **envp, t_rumba **rumba_mk1)
{
	int	exec;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_redir(*cmd);
	update_args0_path(cmd, envp, rumba_mk1);
	exec = execve((*cmd)->args[0], (*cmd)->args, envp);
	if (exec == -1)
	{
		ft_printf_fd(2, "minishell: %s: command not found\n", (*cmd)->args[0]);
		exit(127);
	}
}

char	*pathifie(t_tree **cmd, char **envp, t_rumba **rumba_mk1)
{
	char	**paths;
	char	*absolute;
	int		i;
	int		j;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			paths = fls_split(&envp[i][5], (char *[]){":", NULL}, 0, rumba_mk1);
			j = 0;
			while (paths && paths[j])
			{
				absolute = ft_strjoin_gp(ft_strjoin_gp(paths[j++], "/",
							rumba_mk1), (*cmd)->args[0], rumba_mk1);
				if (!absolute)
					return (NULL);
				if (!access(absolute, X_OK))
					return (absolute);
			}
		}
		i++;
	}
	return (NULL);
}

int	update_args0_path(t_tree **cmd, char **envp, t_rumba **rumba_mk1)
{
	char	*abs_path;

	abs_path = pathifie(cmd, envp, rumba_mk1);
	if (abs_path)
	{
		(*cmd)->args[0] = abs_path;
		return (1);
	}
	return (0);
}
