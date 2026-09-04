/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_nco.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 16:13:01 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/15 16:44:58 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_fd_created_named_file(char *name, t_rumba **rumba_mk1)
{
	static int	index;
	char		*file_name;

	file_name = ft_strjoin_gp(name, ft_itoa_gp(index, rumba_mk1), rumba_mk1);
	index++;
	return (file_name);
}

int	heredoc_write_tmp(char *file_name, char *delimiter)
{
	int		fd_writen;
	char	*line;

	fd_writen = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_writen == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_printf_fd(2, "Warning: heredoc delimited by EOF (wanted `%s')\n",
				delimiter);
			break ;
		}
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		ft_printf_fd(fd_writen, "%s", line);
		ft_printf_fd(fd_writen, "\n");
		free(line);
	}
	return (close(fd_writen), 1);
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
	int		fd_read;
	char	*file_name;
	pid_t	pid;

	file_name = get_fd_created_named_file(".heredoc_tmp_file", rumba_mk1);
	if (!file_name)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		heredoc_write_tmp(file_name, delimiter);
		exit(0);
	}
	if (wait_heredoc(pid) == -2)
		return (-2);
	fd_read = open(file_name, O_RDONLY);
	unlink(file_name);
	return (fd_read);
}
