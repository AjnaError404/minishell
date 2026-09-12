/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 18:44:57 by laaubry           #+#    #+#             */
/*   Updated: 2026/09/13 01:17:25 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_node(t_tree *node, t_shell *shell, t_rumba **rumba_mk1)
{
	if (node == NULL)
		return ;
	if (node->type == TYPE_PIPE)
		execute_pipe(node, shell, rumba_mk1);
	else
		execute_command(node, shell, rumba_mk1);
}

void	setup_redir(t_tree *tree)
{
	if (tree->fd_in != -2)
	{
		dup2(tree->fd_in, 0);
		close(tree->fd_in);
		tree->fd_in = -2;
	}
	if (tree->fd_out != -2)
	{
		dup2(tree->fd_out, 1);
		close(tree->fd_out);
		tree->fd_out = -2;
	}
}

void	exec_left_child(t_tree *node, t_shell *shell, int pipefd[2],
		t_rumba **rumba_mk1)
{
	dup2(pipefd[1], 1);
	close(pipefd[0]);
	close(pipefd[1]);
	close_tree_fds(node->r_child);
	execute_node(node->l_child, shell, rumba_mk1);
	close_tree_fds(node->l_child);
	clean_child_exit(shell->status, shell->env, rumba_mk1);
}

void	exec_right_child(t_tree *node, t_shell *shell, int pipefd[2],
		t_rumba **rumba_mk1)
{
	dup2(pipefd[0], 0);
	close(pipefd[0]);
	close(pipefd[1]);
	close_tree_fds(node->l_child);
	execute_node(node->r_child, shell, rumba_mk1);
	close_tree_fds(node->r_child);
	clean_child_exit(shell->status, shell->env, rumba_mk1);
}

void	execute_pipe(t_tree *node, t_shell *shell, t_rumba **rumba_mk1)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(pipefd) == -1)
		return ;
	pid_left = fork();
	if (pid_left == 0)
		exec_left_child(node, shell, pipefd, rumba_mk1);
	pid_right = fork();
	if (pid_right == 0)
		exec_right_child(node, shell, pipefd, rumba_mk1);
	close(pipefd[0]);
	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	init_signals();
	if (WIFEXITED(status))
		shell->status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->status = 128 + WTERMSIG(status);
}
