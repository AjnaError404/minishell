/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 18:44:57 by laaubry           #+#    #+#             */
/*   Updated: 2026/08/30 12:42:37 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <sys/wait.h> // pour les macros WIFEXITED
#include <unistd.h>   // pour STDIN-FILENO, STDOUT_FILENO, dup2 and close

void	execute_node(t_tree *node, char ***envp, t_rumba **rumba_mk1)
{
	if (node == NULL)
		return ;
	if (node->type == TYPE_PIPE)
		execute_pipe(node, *envp, rumba_mk1);
	else
		execute_command(node, envp, rumba_mk1);
}

void	setup_redir(t_tree *tree)
{
	if (tree->fd_in != -2)
	{
		dup2(tree->fd_in, 0);
		close(tree->fd_in);
	}
	if (tree->fd_out != -2)
	{
		dup2(tree->fd_out, 1);
		close(tree->fd_out);
	}
}

void	exec_left_child(t_tree *node, char **envp, int pipefd[2],
		t_rumba **rumba_mk1)
{
	dup2(pipefd[1], 1);
	close(pipefd[0]);
	close(pipefd[1]);
	execute_node(node->l_child, &envp, rumba_mk1);
	exit(0);
}

void	exec_right_child(t_tree *node, char **envp, int pipefd[2],
		t_rumba **rumba_mk1)
{
	dup2(pipefd[0], 0);
	close(pipefd[0]);
	close(pipefd[1]);
	execute_node(node->r_child, &envp, rumba_mk1);
	exit(0);
}

void	execute_pipe(t_tree *node, char **envp, t_rumba **rumba_mk1)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(pipefd) == -1)
		return ;
	pid_left = fork();
	if (pid_left == 0)
		exec_left_child(node, envp, pipefd, rumba_mk1);
	pid_right = fork();
	if (pid_right == 0)
		exec_right_child(node, envp, pipefd, rumba_mk1);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}
