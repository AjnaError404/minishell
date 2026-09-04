/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 18:44:43 by laaubry           #+#    #+#             */
/*   Updated: 2026/08/30 19:14:33 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

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

static void	exec_child_process(t_tree **cmd, char **envp, t_rumba **rumba_mk1)
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

void	execute_simple_command(t_tree **cmd, char **envp, t_rumba **rumba_mk1)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		exec_child_process(cmd, envp, rumba_mk1);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
}

void	execute_command(t_tree *cmd, char ***envp, t_rumba **rumba_mk1)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		cd(cmd);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		pwd();
	else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		echo(cmd);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		*envp = ft_export(*envp, cmd);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		ft_unset(*envp, cmd);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		env(*envp);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		ft_exit(cmd);
	else
		execute_simple_command(&cmd, *envp, rumba_mk1);
}
