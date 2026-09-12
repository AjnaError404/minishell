/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 18:44:43 by laaubry           #+#    #+#             */
/*   Updated: 2026/09/12 18:30:16 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>


static void	run_builtin(t_tree *cmd, char ***envp)
{
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		g_exit_status = cd(cmd);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		g_exit_status = pwd();
	else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		g_exit_status = echo(cmd);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		*envp = ft_export(*envp, cmd);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		g_exit_status = ft_unset(*envp, cmd);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		g_exit_status = env(*envp);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		ft_exit(cmd);
}

static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd, "pwd", 4)
		|| !ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "exit", 5))
		return (1);
	return (0);
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
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_printf_fd(2, "Quit (core dumped)\n");
		g_exit_status = 128 + WTERMSIG(status);
	}
}

void	execute_command(t_tree *cmd, char ***envp, t_rumba **rumba_mk1)
{
	int	saved_stdout;
	int	saved_stdin;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (is_builtin(cmd->args[0]))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		setup_redir(cmd);
		run_builtin(cmd, envp);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}
	else
		execute_simple_command(&cmd, *envp, rumba_mk1);
}
