/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 16:13:01 by ykandous          #+#    #+#             */
/*   Updated: 2026/09/12 18:00:29 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	heredoc_write(int fd_write, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_printf_fd(2, "warning: heredoc delimited by EOF (wanted `%s')\n",
				delimiter);
			break ;
		}
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		ft_printf_fd(fd_write, "%s\n", line);
		free(line);
	}
	close(fd_write);
}

static int	wait_heredoc(pid_t pid)
{
	int	st;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &st, 0);
	init_signals();
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
	{
		g_exit_status = 130;
		write(1, "\n", 1);
		return (-2);
	}
	return (0);
}

int	heredoc_magic(char *delimiter, t_rumba **rumba_mk1)
{
	int		pipefd[2];
	pid_t	pid;

	(void)rumba_mk1;
	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), -1);
	if (pid == 0)
	{
		close(pipefd[0]);
		signal(SIGINT, SIG_DFL);
		heredoc_write(pipefd[1], delimiter);
		exit(0);
	}
	close(pipefd[1]);
	if (wait_heredoc(pid) == -2)
		return (close(pipefd[0]), -2);
	return (pipefd[0]);
}
