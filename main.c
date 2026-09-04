/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 13:18:37 by laaubry           #+#    #+#             */
/*   Updated: 2026/08/30 19:13:19 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

void	process_line(char *line, char ***my_env, t_rumba **rumba_mk1)
{
	t_tree	*ast;

	add_history(line);
	ast = create_tree(create_lexer(line, rumba_mk1), *my_env, rumba_mk1);
	if (ast)
		execute_node(ast, my_env, rumba_mk1);
	del_all_rumba(rumba_mk1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**my_env;
	t_rumba	*rumba_mk1;

	(void)argc;
	(void)argv;
	init_signals();
	my_env = copy_env(envp);
	rumba_mk1 = NULL;
	while (1)
	{
		line = readline("\033[38;5;45mminishell$ \033[0m");
		if (!line)
		{
			ft_printf("exit\n");
			break ;
		}
		if (line[0])
			process_line(line, &my_env, &rumba_mk1);
		free(line);
	}
	return (g_exit_status);
}
