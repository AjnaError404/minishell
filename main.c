/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 13:18:37 by laaubry           #+#    #+#             */
/*   Updated: 2026/09/13 00:58:49 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
	rl_clear_history();
}

static void	init_shell(t_shell *shell, t_rumba **rumba, char **envp)
{
	init_signals();
	shell->env = copy_env(envp);
	shell->status = 0;
	*rumba = NULL;
}

void	process_line(char *line, t_shell *shell, t_rumba **rumba_mk1)
{
	t_tree		*ast;
	t_lexeme	*lex;

	add_history(line);
	if (!check_quotes_closed(line))
	{
		shell->status = 2;
		return ;
	}
	lex = create_lexer(line, rumba_mk1);
	if (check_lex_pip(lex) == 0)
	{
		ft_printf_fd(2, "minishell: syntax error near unexpected token `|'\n");
		shell->status = 2;
		del_all_rumba(rumba_mk1);
		return ;
	}
	ast = create_tree(lex, shell, rumba_mk1);
	if (ast)
	{
		execute_node(ast, shell, rumba_mk1);
		close_tree_fds(ast);
	}
	del_all_rumba(rumba_mk1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	shell;
	t_rumba	*rumba;

	(void)argc;
	(void)argv;
	init_shell(&shell, &rumba, envp);
	while (1)
	{
		line = readline("\033[38;5;45mminishell$ \033[0m");
		if (g_signal)
			shell.status = 128 + g_signal;
		g_signal = 0;
		if (!line)
			break ;
		if (line[0])
			process_line(line, &shell, &rumba);
		free(line);
	}
	ft_printf("exit\n");
	free_env(shell.env);
	return (shell.status);
}
