/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 00:26:24 by laaubry           #+#    #+#             */
/*   Updated: 2026/09/12 19:11:18 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
void	close_tree_fds(t_tree *tree)
{
	if (!tree)
		return ;
	if (tree->fd_in != -2)
	{
		close(tree->fd_in);
		tree->fd_in = -2;
	}
	if (tree->fd_out != -2)
	{
		close(tree->fd_out);
		tree->fd_out = -2;
	}
	close_tree_fds(tree->l_child);
	close_tree_fds(tree->r_child);
}
void	clean_child_exit(int code, char **envp, t_rumba **rumba_mk1)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		free(envp[i++]);
	free(envp);
	del_all_rumba(rumba_mk1);
	rl_clear_history();
	exit(code);
}

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
		clean_child_exit(127, envp, rumba_mk1);
	}
}
