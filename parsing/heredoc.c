/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 16:13:01 by ykandous          #+#    #+#             */
/*   Updated: 2026/09/13 21:36:09 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	heredoc_sig(int sig)
{
	(void)sig;
	exit(130);
}

static void	heredoc_child(char *delimiter, int write_fd, t_rumba **rumba_mk1)
{
	char	*line;

	signal(SIGINT, heredoc_sig);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		ft_printf_fd(write_fd, "%s\n", line);
		free(line);
	}
	if (!line)
		ft_printf_fd(2, "warning: heredoc delimited by EOF (wanted `%s')\n",
			delimiter);
	free(line);
	close(write_fd);
	del_all_rumba(rumba_mk1);
	exit(0);
}

static int	wait_heredoc(pid_t pid)
{
	int	st;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &st, 0);
	init_signals();
	if ((WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
		|| (WIFEXITED(st) && WEXITSTATUS(st) == 130))
	{
		g_signal = SIGINT;
		write(1, "\n", 1);
		return (-2);
	}
	return (0);
}

int	heredoc_magic(char *delimiter, t_rumba **rumba_mk1)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), -1);
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child(delimiter, pipefd[1], rumba_mk1);
	}
	close(pipefd[1]);
	if (wait_heredoc(pid) == -2)
		return (close(pipefd[0]), -2);
	return (pipefd[0]);
}
