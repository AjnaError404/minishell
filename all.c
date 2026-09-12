// Dans le dossier builtins

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 15:08:29 by laaubry           #+#    #+#             */
/*   Updated: 2026/08/30 07:43:42 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd(void)
{
	char	buffer[4096];
	char	*result;

	result = getcwd(buffer, 1024);
	if (result == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_printf("%s\n", buffer);
	return (0);
}

int	cd(t_tree *cmd)
{
	int	result;

	if (cmd->args[1] == NULL)
	{
		ft_printf_fd(2, "minishell: cd: need an argument\n");
		return (1);
	}
	result = chdir(cmd->args[1]);
	if (result == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	echo(t_tree *cmd)
{
	int	i;
	int	n_flag;
	int	fd_out;

	i = 1;
	n_flag = 0;
	fd_out = 1;
	if (cmd->fd_out != -2)
		fd_out = cmd->fd_out;
	if (cmd->args[1] != NULL && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		n_flag = 1;
		i = 2;
	}
	while (cmd->args[i] != NULL)
	{
		ft_printf_fd(fd_out, "%s", cmd->args[i]);
		if (cmd->args[i + 1] != NULL)
			ft_printf_fd(fd_out, " ");
		i++;
	}
	if (n_flag == 0)
		ft_printf_fd(fd_out, "\n");
	return (0);
}

int	env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		ft_printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 17:34:00 by laaubry           #+#    #+#             */
/*   Updated: 2026/08/30 12:07:41 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exit(t_tree *cmd)
{
	int	status;

	ft_printf("exit\n");
	if (cmd->args[1] != NULL)
	{
		if (is_numeric(cmd->args[1]) == 0)
		{
			ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n",
				cmd->args[1]);
			exit(2);
		}
		if (cmd->args[2] != NULL)
		{
			ft_printf_fd(2, "minishell: exit:too many arguments\n");
			return (1);
		}
		status = ft_atoi(cmd->args[1]);
		exit(status);
	}
	else
		exit(0);
}

int	ft_unset(char **env, t_tree *cmd)
{
	int	i;
	int	len;

	if (cmd->args[1] == NULL)
		return (0);
	i = 0;
	len = ft_strlen(cmd->args[1]);
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], cmd->args[1], len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			while (env[i] != NULL)
			{
				env[i] = env[i + 1];
				i++;
			}
			return (0);
		}
		i++;
	}
	return (0);
}

int	update_env_var(char **env, char *arg, int len)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], arg, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			env[i] = ft_strdup(arg);
			return (1);
		}
		i++;
	}
	return (0);
}

char	**add_env_var(char **env, char *arg)
{
	int		i;
	int		count;
	char	**new_env;

	count = count_variables(env);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[count] = ft_strdup(arg);
	new_env[count + 1] = NULL;
	free(env);
	return (new_env);
}

char	**ft_export(char **env, t_tree *cmd)
{
	int	len;

	if (cmd->args[1] == NULL)
		return (env);
	len = 0;
	while (cmd->args[1][len] != '=' && cmd->args[1][len] != '\0')
		len++;
	if (update_env_var(env, cmd->args[1], len) == 1)
		return (env);
	return (add_env_var(env, cmd->args[1]));
}

// Dans le dossier exec_dir

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

// Dans le dossier expander

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 19:27:47 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 19:13:50 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isvarname_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && str[i] != '_')
			return (i);
		i++;
	}
	return (i);
}

char	*graft_str_gp(char *dest, t_tab *index_tab, char *grafted,
		t_rumba **rumba_mk1)
{
	int		len_dest;
	char	*result;

	len_dest = ft_strlen(dest);
	if (index_tab->tab[0] > len_dest || index_tab->tab[1] > len_dest
		|| index_tab->tab[0] > index_tab->tab[1])
		return (NULL);
	result = ft_strjoin_gp(ft_strjoin_gp(ft_substr_gp(dest, 0,
					index_tab->tab[0], rumba_mk1), grafted, rumba_mk1),
			ft_substr_gp(dest, index_tab->tab[1], len_dest - index_tab->tab[1],
				rumba_mk1), rumba_mk1);
	return (result);
}

char	*find_var(char *arg, int *i)
{
	int	state;

	state = STATE_OUT_QUOTE;
	while (arg[*i])
	{
		state = update_fsa_state(arg[*i], state);
		if (arg[*i] == '$' && (state == STATE_OUT_QUOTE
				|| state == STATE_DBL_QUOTE))
			return (&arg[*i]);
		(*i)++;
	}
	return (NULL);
}

char	*pick_var_name(char *str, t_rumba **rumba_mk1)
{
	char	*var;
	int		len_var;

	var = NULL;
	if (str[0] == ' ' || str[0] == '\0' || str[0] == '\"')
		return (NULL);
	else if (str[0] == '?')
	{
		var = malloc_rumba(2 * sizeof(char), rumba_mk1);
		var[0] = '?';
		var[1] = 0;
		return (var);
	}
	len_var = ft_isvarname_char(str);
	if (len_var)
		var = ft_substr_gp(str, 0, len_var, rumba_mk1);
	return (var);
}

char	*find_envvar(char **envp, char *var_name)
{
	int	i;
	int	var_len;

	var_len = ft_strlen(var_name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var_name, var_len) && envp[i][var_len] == '=')
			return (&envp[i][var_len + 1]);
		i++;
	}
	return ("");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:43:34 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 19:14:01 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_var(char **envp, char *tree_args, char *var,
		t_rumba **rumba_mk1)
{
	char	*picked;
	char	*expanded;
	char	*replacement;
	int		var_index;
	t_tab	*index_tab;

	var_index = var - tree_args;
	picked = pick_var_name(var + 1, rumba_mk1);
	if (!picked)
		return (tree_args);
	index_tab = init_tab(2, rumba_mk1);
	if (!index_tab)
		return (NULL);
	index_tab->tab[0] = var_index;
	index_tab->tab[1] = var_index + ft_strlen(picked) + 1;
	if (ft_strcmp(picked, "?") == 0)
		replacement = ft_itoa_gp(g_exit_status, rumba_mk1);
	else
		replacement = find_envvar(envp, picked);
	expanded = graft_str_gp(tree_args, index_tab, replacement, rumba_mk1);
	return (expanded);
}

int	treenode_expand_args(t_tree **tree, char **envp, t_rumba **rumba_mk1)
{
	int		i;
	int		j;
	char	*var;

	if (!tree || !*tree)
		return (0);
	if (!(*tree)->args)
		return (1);
	i = 0;
	while ((*tree)->args[i])
	{
		j = 0;
		var = find_var((*tree)->args[i], &j);
		while (var)
		{
			(*tree)->args[i] = expand_var(envp, (*tree)->args[i], var,
					rumba_mk1);
			j++;
			var = find_var((*tree)->args[i], &j);
		}
		i++;
	}
	return (1);
}

int	treenode_expand_cmd(t_tree **tree, char **envp, t_rumba **rumba_mk1)
{
	int		j;
	char	*var;

	if (!tree || !*tree || !(*tree)->cmd_name)
		return (0);
	j = 0;
	var = find_var((*tree)->cmd_name, &j);
	while (var)
	{
		(*tree)->cmd_name = expand_var(envp, (*tree)->cmd_name, var, rumba_mk1);
		j++;
		var = find_var((*tree)->cmd_name, &j);
	}
	return (1);
}

int	tree_expand_all(t_tree **tree, char **envp, t_rumba **rumba_mk1)
{
	if ((*tree)->type == TYPE_PIPE)
	{
		if (!tree_expand_all(&(*tree)->l_child, envp, rumba_mk1))
			return (0);
		if (!tree_expand_all(&(*tree)->r_child, envp, rumba_mk1))
			return (0);
		return (1);
	}
	if (!treenode_expand_args(tree, envp, rumba_mk1))
		return (0);
	if (!treenode_expand_cmd(tree, envp, rumba_mk1))
		return (0);
	return (1);
}

int	update_fsa_state(char c, int state)
{
	if (c == '\'')
		return (state - (state == STATE_SGL_QUOTE)
			+ (state == STATE_OUT_QUOTE));
	else if (c == '\"')
		return (state - 2 * (state == STATE_DBL_QUOTE) + 2
			* (state == STATE_OUT_QUOTE));
	return (state);
}

// Dans le dossier parsing

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_type.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 10:46:39 by ykandous          #+#    #+#             */
/*   Updated: 2026/05/29 11:02:15 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalpha(char c)
{
	if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
		return (1);
	return (0);
}

int	ft_isall_alpha(char *str)
{
	while (*str)
	{
		if (!ft_isalpha(*str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_isdigit(char c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

int	ft_isall_word(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str) && !ft_isalpha(*str) && *str != '-' && *str != ' '
			&& *str != '\'' && *str != '\"' && *str != '_')
		{
			return (0);
		}
		str++;
	}
	return (1);
}

int	ft_char_is_something(char c)
{
	if (ft_isalpha(c) || ft_isdigit(c) || c == '-')
		return (TYPE_WORD);
	if (c == 39)
		return (TYPE_QUOTE);
	if (c == -1)
		return (TYPE_EOF);
	if (c == '\"')
		return (TYPE_DBLQUOTE);
	if (c == '<')
		return (TYPE_REDIR_IN);
	if (c == '>')
		return (TYPE_REDIR_OUT);
	if (c == '|')
		return (TYPE_PIPE);
	return (TYPE_UNDEFINE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 20:57:22 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/23 19:47:43 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_lex_pip(t_lexeme *lex)
{
	if (!lex)
		return (-1);
	if (lex->type == TYPE_PIPE)
		return (0);
	while (lex)
	{
		if (lex->type == TYPE_PIPE && (lex->next == NULL
				|| lex->next->type == TYPE_PIPE))
			return (0);
		lex = lex->next;
	}
	return (1);
}

int	str_is_quote_closed(char *cmd)
{
	int	i;
	int	f_closed_quote;

	f_closed_quote = 1;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\"')
		{
			if (f_closed_quote)
				f_closed_quote = 0;
			else
				f_closed_quote = 1;
		}
		i++;
	}
	return (f_closed_quote);
}

int	lex_cmd_is_valid(t_lexeme *lex)
{
	if (!lex)
		return (-1);
	while (lex)
	{
		if (lex->type == TYPE_WORD && !str_is_quote_closed(lex->value))
			return (0);
		lex = lex->next;
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_args_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:53:57 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 12:32:28 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_str_redir(char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "<") == 0 || ft_strcmp(str, ">") == 0 || ft_strcmp(str,
			"<<") == 0 || ft_strcmp(str, ">>") == 0)
		return (1);
	return (0);
}

int	switch_redir(char *name, char *symbol, t_rumba **rumba_mk1)
{
	int	fd_opened;

	fd_opened = -1;
	if (ft_strcmp(symbol, "<<") == 0)
		fd_opened = heredoc_magic(name, rumba_mk1);
	else if (ft_strcmp(symbol, "<") == 0)
		fd_opened = open(name, O_RDONLY);
	else if (ft_strcmp(symbol, ">") == 0)
		fd_opened = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strcmp(symbol, ">>") == 0)
		fd_opened = open(name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_opened == -1)
	{
		ft_printf_fd(2, "minishell: %s: ", name);
		perror("");
	}
	return (fd_opened);
}

int	treenode_add_fd(t_tree **tree_node, t_rumba **rumba_mk1, int i)
{
	int		fd;
	char	*sym;

	sym = (*tree_node)->args[i];
	if (!is_str_redir(sym))
		return (i + 1);
	fd = switch_redir((*tree_node)->args[i + 1], sym, rumba_mk1);
	if (fd == -1 || fd == -2)
		return (fd);
	if (sym[0] == '<')
	{
		if ((*tree_node)->fd_in != -2)
			close((*tree_node)->fd_in);
		(*tree_node)->fd_in = fd;
	}
	else
	{
		if ((*tree_node)->fd_out != -2)
			close((*tree_node)->fd_out);
		(*tree_node)->fd_out = fd;
	}
	shift_2left_str_tab(&(*tree_node)->args[i], 2);
	return (i);
}

int	treenode_add_args_n_fd(t_tree **tree_node, t_rumba **rumba_mk1)
{
	int	i;

	(*tree_node)->args = cmd_to_args((*tree_node)->cmd_name, rumba_mk1);
	if (!(*tree_node)->args)
		return (0);
	i = 0;
	while ((*tree_node)->args[i])
	{
		i = treenode_add_fd(tree_node, rumba_mk1, i);
		if (i == -1 || i == -2)
		{
			(*tree_node)->args[0] = NULL;
			return (0);
		}
	}
	return (1);
}

int	tree_update_args_n_fd(t_tree **tree, t_rumba **rumba_mk1)
{
	if ((*tree)->type == TYPE_PIPE)
	{
		if (!tree_update_args_n_fd(&(*tree)->l_child, rumba_mk1))
			return (0);
		if (!tree_update_args_n_fd(&(*tree)->r_child, rumba_mk1))
			return (0);
		return (1);
	}
	if (!treenode_add_args_n_fd(tree, rumba_mk1))
		return (0);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fs_automaton_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 18:52:27 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 06:50:44 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*is_charset(char *c, char **set)
{
	size_t	i;

	i = 0;
	while (set && set[i])
	{
		if (!ft_strncmp(c, set[i], ft_strlen(set[i])))
			return (set[i]);
		i++;
	}
	return (NULL);
}

int	increment_count_or_not(char c, int state)
{
	if ((state == STATE_SGL_QUOTE && c == '\'') || (state == STATE_DBL_QUOTE
			&& c == '\"'))
		return (1);
	return (0);
}

int	update_state_n_count(char c, int state, ssize_t *count)
{
	*count += increment_count_or_not(c, state);
	return (update_fsa_state(c, state));
}

void	fls_countword_initvat(size_t *i, ssize_t *count, size_t *word,
		int *state)
{
	*i = 0;
	*count = 0;
	*word = 0;
	*state = STATE_OUT_QUOTE;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fs_automaton.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 20:03:39 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 12:18:22 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	fls_countword(char *str, char **set)
{
	size_t	i;
	ssize_t	count;
	size_t	word;
	int		state;
	char	*sep;

	fls_countword_initvat(&i, &count, &word, &state);
	while (str[i])
	{
		state = update_state_n_count(str[i], state, &count);
		if (state == STATE_OUT_QUOTE)
		{
			sep = is_charset(&str[i], set);
			if (!sep && word == 0)
				word = (count++ > -1);
			else if (sep && word == 1)
			{
				word = !(count++ > -1);
				i += ft_strlen(sep) - 1;
			}
		}
		i++;
	}
	return (count);
}

int	fls_wordsize(char *str, char **set, int state)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		state = update_fsa_state(str[i], state);
		if (state == STATE_OUT_QUOTE && is_charset(&str[i], set))
			break ;
		i++;
	}
	return (i);
}

char	*fls_fillword(char *str, char **set, int state, t_rumba **rumba_mk1)
{
	size_t	i;
	size_t	end_word;
	char	*word;

	i = 0;
	end_word = fls_wordsize(str, set, state);
	word = malloc_rumba((end_word + 1) * sizeof(char), rumba_mk1);
	if (!word)
		return (NULL);
	while (*str && i < end_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

int	handle_sep(char *sep, int add_sep, char **splited, int *j)
{
	if (add_sep)
	{
		splited[*j] = sep;
		(*j)++;
	}
	return (ft_strlen(sep));
}

char	**fls_split(char *str, char **set, int add_sep, t_rumba **rumba_mk1)
{
	int		j;
	int		state;
	char	**splited;
	char	*sep;

	splited = malloc_rumba((fls_countword(str, set) + 1) * sizeof(char *),
			rumba_mk1);
	if (!splited)
		return (NULL);
	state = STATE_OUT_QUOTE;
	j = 0;
	while (str && *str)
	{
		sep = is_charset(str, set);
		if (!sep)
		{
			splited[j++] = fls_fillword(str, set, state, rumba_mk1);
			str += fls_wordsize(str, set, state);
		}
		else
			str += handle_sep(sep, add_sep, splited, &j);
	}
	splited[j] = NULL;
	return (splited);
}

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 13:03:48 by ykandous          #+#    #+#             */
/*   Updated: 2026/05/29 11:19:00 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_charcmd	*init_charcmd(char value, t_rumba **rumba_mk1)
{
	t_charcmd	*cc_node;

	cc_node = malloc_rumba(sizeof(t_charcmd), rumba_mk1);
	if (!cc_node)
		return (NULL);
	cc_node->type = ft_char_is_something(value);
	cc_node->value = value;
	return (cc_node);
}

t_lexeme	*init_lexeme(char *value, t_rumba **rumba_mk1)
{
	t_lexeme	*lx_node;

	lx_node = malloc_rumba(sizeof(t_lexeme), rumba_mk1);
	if (!lx_node)
		return (NULL);
	lx_node->type = ft_str_is_something(value);
	lx_node->value = value;
	lx_node->next = NULL;
	lx_node->prev = NULL;
	return (lx_node);
}

t_tree	*init_tree(char *value, int type, t_rumba **rumba_mk1)
{
	t_tree	*tree_node;

	tree_node = malloc_rumba(sizeof(t_tree), rumba_mk1);
	if (!tree_node)
		return (NULL);
	tree_node->cmd_name = value;
	tree_node->type = type;
	tree_node->fd_in = -2;
	tree_node->fd_out = -2;
	tree_node->r_child = NULL;
	tree_node->l_child = NULL;
	return (tree_node);
}

t_tab	*init_tab(int size, t_rumba **rumba_mk1)
{
	t_tab	*tab;

	tab = malloc_rumba(sizeof(t_tab), rumba_mk1);
	if (!tab)
		return (NULL);
	tab->tab = malloc_rumba(size * sizeof(int), rumba_mk1);
	if (!tab->tab)
	{
		del_one_rumba(rumba_mk1, tab);
		return (NULL);
	}
	tab->size = size;
	return (tab);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:41:58 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/08 14:58:02 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_lexeme	*add_lexeme(t_lexeme **lex, char *value, t_rumba **rumba_mk1)
{
	t_lexeme	*lex_node;
	t_lexeme	*tmp_node;

	lex_node = init_lexeme(value, rumba_mk1);
	if (!lex_node)
		return (NULL);
	if (!lex || !*lex)
	{
		*lex = lex_node;
		return (lex_node);
	}
	tmp_node = *lex;
	while (tmp_node->next)
		tmp_node = tmp_node->next;
	lex_node->prev = tmp_node;
	tmp_node->next = lex_node;
	return (lex_node);
}

t_lexeme	*create_lexer(char *args, t_rumba **rumba_mk1)
{
	t_lexeme	*lexer;
	char		**splited;
	int			i;

	lexer = NULL;
	splited = fls_split(args, (char *[]){"|", NULL}, 1, rumba_mk1);
	i = 0;
	while (splited[i])
	{
		if (!add_lexeme(&lexer, splited[i], rumba_mk1))
			return (NULL);
		i++;
	}
	return (lexer);
}

int	count_lexemes(t_lexeme *lex)
{
	int	i;

	i = 0;
	while (lex)
	{
		i++;
		lex = lex->next;
	}
	return (i);
}

char	**fill_str_from_lex(t_lexeme *lexer, t_rumba **rumba_mk1)
{
	int		i;
	char	**args;

	args = malloc_rumba((count_lexemes(lexer) + 1) * sizeof(char *), rumba_mk1);
	if (!args)
		return (NULL);
	i = 0;
	while (lexer)
	{
		args[i++] = lexer->value;
		lexer = lexer->next;
	}
	args[i] = NULL;
	return (args);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:52:14 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 12:18:48 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_cmd_redir(int type)
{
	if (type >= TYPE_REDIR_IN && type <= TYPE_APPEND)
		return (type);
	return (0);
}

int	is_redir_valid(char **cmd)
{
	int	i;

	if (!cmd)
		return (0);
	i = 0;
	while (cmd[i])
	{
		if (is_cmd_redir(ft_str_is_something(cmd[i])))
		{
			if (cmd[i + 1] == NULL)
				return (0);
			if (is_cmd_redir(ft_str_is_something(cmd[i + 1])))
				return (0);
		}
		i++;
	}
	return (1);
}

int	switch_open(char *pathname, int redir_type)
{
	int	fd;

	fd = -1;
	if (redir_type == TYPE_REDIR_IN)
		fd = open(pathname, O_RDONLY);
	else if (redir_type == TYPE_REDIR_OUT)
		fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir_type == TYPE_APPEND)
		fd = open(pathname, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

char	**cmd_to_args(char *cmd, t_rumba **rumba_mk1)
{
	char	**splited;
	int		i;
	int		len;

	splited = fls_split(cmd, (char *[]){" ", "\t", NULL}, 0, rumba_mk1);
	if (!splited)
		return (NULL);
	i = 0;
	len = 0;
	while (splited[i])
	{
		if (splited[i][0] != '\0')
		{
			splited[len] = splited[i];
			len++;
		}
		i++;
	}
	splited[len] = NULL;
	if (len == 0 || !is_redir_valid(splited))
		return (NULL);
	return (splited);
}

int	shift_2left_str_tab(char **strs, int nshift)
{
	int	i;
	int	j;

	if (!strs)
		return (0);
	i = 0;
	while (i < nshift)
	{
		if (!strs[i])
			return (0);
		i++;
	}
	j = 0;
	while (strs[j + nshift])
	{
		strs[j] = strs[j + nshift];
		j++;
	}
	strs[j] = strs[j + nshift];
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 18:03:38 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 12:07:21 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	fix_state_strlen(char *str)
{
	int	state;
	int	newstate;
	int	i;
	int	len;

	state = STATE_OUT_QUOTE;
	i = 0;
	len = 0;
	while (str[i])
	{
		newstate = update_fsa_state(str[i], state);
		if (state == newstate)
			len++;
		state = newstate;
		i++;
	}
	return (len);
}

char	*str_rm_quote(char *str, t_rumba **rumba_mk1)
{
	int		i;
	int		j;
	int		state;
	int		newstate;
	char	*unquoted;

	unquoted = malloc_rumba((fix_state_strlen(str) + 1) * sizeof(char),
			rumba_mk1);
	if (!unquoted)
		return (NULL);
	state = STATE_OUT_QUOTE;
	j = 0;
	i = 0;
	while (str[j])
	{
		newstate = update_fsa_state(str[j], state);
		if (state == newstate)
			unquoted[i++] = str[j++];
		else
			j++;
		state = newstate;
	}
	unquoted[i] = 0;
	return (unquoted);
}

int	args_rm_quote(char **args, t_rumba **rumba_mk1)
{
	int	i;

	if (!args)
		return (0);
	i = 0;
	while (args[i])
	{
		args[i] = str_rm_quote(args[i], rumba_mk1);
		if (!args[i])
			return (0);
		i++;
	}
	return (1);
}

int	remove_quote(t_tree **tree, t_rumba **rumba_mk1)
{
	if (!tree)
		return (0);
	if (is_cmd_opprt((*tree)->type))
	{
		if (!remove_quote(&((*tree)->l_child), rumba_mk1))
			return (0);
		if (!remove_quote(&((*tree)->r_child), rumba_mk1))
			return (0);
	}
	else
	{
		if (!args_rm_quote((*tree)->args, rumba_mk1))
			return (0);
		(*tree)->cmd_name = str_rm_quote((*tree)->cmd_name, rumba_mk1);
		if (!(*tree)->cmd_name)
			return (0);
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 22:28:53 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 07:25:45 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_lexeme	*find_oprt(t_lexeme *lexer)
{
	while (lexer)
	{
		if (is_cmd_opprt(lexer->type))
			return (lexer);
		lexer = lexer->next;
	}
	return (NULL);
}

int	find_lex_childs(t_lexeme **root, t_lexeme **l_child, t_lexeme **r_child)
{
	*root = find_oprt(*l_child);
	if (!*root)
		return (0);
	if ((*root)->prev)
		(*root)->prev->next = NULL;
	else
		*l_child = NULL;
	if ((*root)->next)
	{
		*r_child = (*root)->next;
		(*r_child)->prev = NULL;
	}
	return (1);
}

t_tree	*lex_to_tree(t_lexeme *lexer, t_rumba **rumba_mk1)
{
	t_lexeme	*root;
	t_lexeme	*r_child;
	t_tree		*tree;

	if (!lexer)
		return (NULL);
	if (!find_lex_childs(&root, &lexer, &r_child))
	{
		tree = init_tree(lexer->value, lexer->type, rumba_mk1);
		if (!tree)
			return (NULL);
		tree->args = fill_str_from_lex(lexer, rumba_mk1);
		if (!tree->args)
			return (NULL);
	}
	else
	{
		tree = init_tree(root->value, root->type, rumba_mk1);
		if (!tree)
			return (NULL);
		tree->l_child = lex_to_tree(lexer, rumba_mk1);
		tree->r_child = lex_to_tree(r_child, rumba_mk1);
	}
	return (tree);
}

t_tree	*find_child(t_lexeme *r_child, t_rumba **rumba_mk1, t_tree *tree)
{
	t_lexeme	*root;
	t_lexeme	*l_child;

	if (find_lex_childs(&root, &l_child, &r_child))
	{
		tree = init_tree(root->value, root->type, rumba_mk1);
		if (!tree)
			return (NULL);
		tree->l_child = init_tree(l_child->value, l_child->type, rumba_mk1);
		if (!(tree->l_child))
			return (NULL);
		tree->r_child = find_child(r_child, rumba_mk1, tree);
	}
	else
	{
		tree->r_child = init_tree(root->value, root->type, rumba_mk1);
		if (!(tree->r_child))
			return (NULL);
	}
	return (tree->r_child);
}

t_tree	*create_tree(t_lexeme *lexer, char **envp, t_rumba **rumba_mk1)
{
	t_tree	*tree;

	tree = NULL;
	tree = lex_to_tree(lexer, rumba_mk1);
	tree_update_args_n_fd(&tree, rumba_mk1);
	tree_expand_all(&tree, envp, rumba_mk1);
	remove_quote(&tree, rumba_mk1);
	return (tree);
}

// Dans le dossier rumba

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rumba_del.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:09:03 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 19:09:22 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	del_one_rumba(t_rumba **rumba_mk1, void *wilfreed)
{
	t_rumba	*tmp;

	if (!rumba_mk1 || !*rumba_mk1 || !wilfreed)
		return (-1);
	tmp = *rumba_mk1;
	while (tmp)
	{
		if (tmp->wilfreed == wilfreed)
		{
			if (tmp == *rumba_mk1)
			{
				if (tmp->next)
					tmp->next->prev = NULL;
				*rumba_mk1 = tmp->next;
			}
			else
				extract_mid_or_end_node(&tmp);
			free(tmp->wilfreed);
			free(tmp);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	del_first_rumba(t_rumba **rumba_mk1, void *wilfreed)
{
	t_rumba	*tmp;

	if (!rumba_mk1 || !*rumba_mk1 || !wilfreed)
		return (-1);
	tmp = *rumba_mk1;
	if (tmp->wilfreed == wilfreed)
	{
		if (tmp->next != NULL)
			tmp->next->prev = NULL;
		*rumba_mk1 = tmp->next;
		free(tmp->wilfreed);
		free(tmp);
		return (1);
	}
	return (0);
}

int	del_all_rumba(t_rumba **rumba_mk1)
{
	t_rumba	*tmp_node;

	if (!rumba_mk1 || !*rumba_mk1)
		return (-1);
	tmp_node = *rumba_mk1;
	while (tmp_node)
	{
		*rumba_mk1 = tmp_node;
		tmp_node = tmp_node->next;
		if (*rumba_mk1 != NULL)
		{
			if ((*rumba_mk1)->wilfreed != NULL)
				free((*rumba_mk1)->wilfreed);
			free(*rumba_mk1);
		}
	}
	*rumba_mk1 = NULL;
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rumba_mk1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 01:58:15 by ykandous          #+#    #+#             */
/*   Updated: 2026/05/29 16:17:45 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_rumba	*add_rumba(t_rumba **rumba_mk1, void *wilfreed)
{
	t_rumba	*node;

	node = init_rumba_mk1(wilfreed);
	if (!node)
		return (NULL);
	if (!*rumba_mk1)
	{
		*rumba_mk1 = node;
		return (node);
	}
	(*rumba_mk1)->prev = node;
	node->next = *rumba_mk1;
	*rumba_mk1 = node;
	return (node);
}
/*
int	cmpr_data(void *data_one, void *data_two, int n)
{
	int	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n - 1
		&& ((unsigned char *)data_one)[i] == ((unsigned char *)data_two)[i])
		i++;
	return (((unsigned char *)data_one)[i] - ((unsigned char *)data_two)[i]);
}
*/

int	extract_mid_or_end_node(t_rumba **rumba_mk1)
{
	t_rumba	*tmp_node;

	if (rumba_mk1 || *rumba_mk1)
	{
		tmp_node = (*rumba_mk1)->next;
		if ((*rumba_mk1)->prev)
			(*rumba_mk1)->prev->next = tmp_node;
		if (tmp_node)
			tmp_node->prev = (*rumba_mk1)->prev;
		return (1);
	}
	return (0);
}

void	*malloc_rumba(size_t size, t_rumba **rumba_mk1)
{
	void	*wilfreed;

	wilfreed = malloc(size);
	if (!wilfreed)
		return (NULL);
	add_rumba(rumba_mk1, wilfreed);
	return (wilfreed);
}

t_rumba	*init_rumba_mk1(void *wilfreed)
{
	t_rumba	*gc_node;

	gc_node = malloc(sizeof(t_rumba));
	if (!gc_node)
		return (NULL);
	gc_node->wilfreed = wilfreed;
	gc_node->next = NULL;
	gc_node->prev = NULL;
	return (gc_node);
}

// Dans le dossier utils

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:10:23 by ykandous          #+#    #+#             */
/*   Updated: 2025/11/14 20:42:25 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_sizenbr(int n)
{
	size_t	siz;

	siz = 0;
	while (n / 10)
	{
		siz++;
		n = n / 10;
	}
	return (siz + 1);
}

char	*ft_fillnbr(int sign, int i, char *res, long n)
{
	while (i >= sign)
	{
		res[i] = '0' + n % 10;
		n = n / 10;
		i--;
	}
	if (sign == 1)
		res[i] = '-';
	return (res);
}

char	*ft_itoa_gp(int n, t_rumba **rumba_mk1)
{
	char	*res;
	size_t	siz;
	int		sign;
	int		i;
	long	nb;

	sign = 0;
	nb = n;
	if (n < 0)
	{
		sign++;
		nb = -1 * nb;
	}
	siz = ft_sizenbr(nb);
	res = malloc_rumba((siz + sign + 1) * sizeof(char), rumba_mk1);
	if (res == NULL)
		return (NULL);
	i = siz + sign - 1;
	res[siz + sign] = '\0';
	res = ft_fillnbr(sign, i, res, nb);
	return (res);
}
/*
#include <limits.h>
#include <stdio.h>

int	main(void)
{
	char	*s;

	s = ft_itoa(INT_MIN);
	if (!ft_strncmp(s, "-2147483648", 100))
		printf("OK");
}
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 06:13:57 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/23 20:11:17 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_putchar_fd(int c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_putstr_fd(char *str, int fd)
{
	int	len;

	if (!str)
		return (ft_putstr_fd("(null)", fd));
	len = ft_strlen(str);
	write(fd, str, len);
	return (len);
}

void	ft_put_pos_base_fd(unsigned long nb, char *base, int *cmp, int fd)
{
	int		len;
	char	n;

	len = ft_strlen(base);
	if (nb / len != 0)
	{
		(*cmp)++;
		ft_put_pos_base_fd(nb / len, base, cmp, fd);
	}
	n = base[nb % len];
	write(fd, &n, 1);
}

int	ft_putbase_fd(int nb, char *base, int fd)
{
	int	cmp;

	cmp = 1;
	if (nb == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return (11);
	}
	else
	{
		if (nb < 0)
		{
			write(fd, "-", 1);
			cmp++;
			ft_put_pos_base_fd(-1 * nb, base, &cmp, fd);
		}
		else
			ft_put_pos_base_fd(nb, base, &cmp, fd);
	}
	return (cmp);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 06:35:14 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/15 16:44:50 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_put_unsigned_nbr_base_fd(unsigned int nb, char *base, int fd)
{
	int	cmp;

	cmp = 1;
	ft_put_pos_base_fd(nb, base, &cmp, fd);
	return (cmp);
}

int	ft_put_ptr_fd(void *ptr, int fd)
{
	int	cmp;

	if (!ptr)
	{
		ft_putstr_fd("(nil)", fd);
		return (5);
	}
	cmp = ft_putstr_fd("0x", fd) + 1;
	ft_put_pos_base_fd((unsigned long)ptr, "0123456789abcdef", &cmp, fd);
	return (cmp);
}

void	ft_switch_case_fd(char convert, va_list params, int *cmp, int fd)
{
	if (convert == 'c')
		*cmp += ft_putchar_fd(va_arg(params, int), fd);
	else if (convert == 's')
		*cmp += ft_putstr_fd(va_arg(params, char *), fd);
	else if (convert == 'p')
		*cmp += ft_put_ptr_fd(va_arg(params, void *), fd);
	else if (convert == 'd' || convert == 'i')
		*cmp += ft_putbase_fd(va_arg(params, int), "0123456789", fd);
	else if (convert == 'u')
		*cmp += ft_put_unsigned_nbr_base_fd(va_arg(params, unsigned int),
				"0123456789", fd);
	else if (convert == 'x')
		*cmp += ft_put_unsigned_nbr_base_fd(va_arg(params, unsigned int),
				"0123456789abcdef", fd);
	else if (convert == 'X')
		*cmp += ft_put_unsigned_nbr_base_fd(va_arg(params, unsigned int),
				"0123456789ABCDEF", fd);
	else
		*cmp += ft_putchar_fd(convert, fd);
}

int	ft_printf_fd(int fd, const char *format, ...)
{
	va_list	params;
	int		i;
	int		cmp;

	i = 0;
	cmp = 0;
	if (!format)
		return (0);
	va_start(params, format);
	while (format[i])
	{
		if (format[i] == '%')
			ft_switch_case_fd(format[++i], params, &cmp, fd);
		else
		{
			ft_putchar_fd(format[i], fd);
			cmp++;
		}
		i++;
	}
	va_end(params);
	return (cmp);
}

int	ft_printf(const char *format, ...)
{
	va_list	params;
	int		i;
	int		cmp;

	i = 0;
	cmp = 0;
	if (!format)
		return (0);
	va_start(params, format);
	while (format[i])
	{
		if (format[i] == '%')
			ft_switch_case_fd(format[++i], params, &cmp, STDOUT_FILENO);
		else
		{
			ft_putchar_fd(format[i], STDOUT_FILENO);
			cmp++;
		}
		i++;
	}
	va_end(params);
	return (cmp);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 18:06:21 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/17 18:06:46 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup_gp(char *s, t_rumba **rumba_mk1)
{
	size_t	sizs;
	size_t	i;
	char	*dup;

	sizs = ft_strlen(s) + 1;
	dup = malloc_rumba(sizs * sizeof(char), rumba_mk1);
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < sizs - 1)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_substr_gp(char *s, unsigned int start, size_t len,
		t_rumba **rumba_mk1)
{
	char	*sub;
	size_t	i;
	size_t	len_s;

	len_s = ft_strlen(s);
	if (len == 0 || start > len_s)
		return (ft_strdup_gp("", rumba_mk1));
	if (len >= len_s - start)
		sub = malloc_rumba((len_s - (size_t)start + 1) * sizeof(char),
				rumba_mk1);
	else
		sub = malloc_rumba((len + 1) * sizeof(char), rumba_mk1);
	if (sub == NULL)
		return (NULL);
	i = 0;
	while (i < len && i <= len_s)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

static int	ft_min(size_t a, size_t b)
{
	if (a < b)
		return (a);
	return (b);
}

size_t	ft_strlcat(char *dst, char *src, size_t siz)
{
	size_t	i;
	size_t	len_dst;
	size_t	len_src;

	len_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	i = len_dst;
	while (i + 1 < siz && i < len_src + len_dst)
	{
		dst[i] = src[i - len_dst];
		i++;
	}
	return (len_src + ft_min(siz, len_dst));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_something.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykandous <ykandous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 20:14:41 by ykandous          #+#    #+#             */
/*   Updated: 2026/05/29 11:08:52 by ykandous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_str_is_something(char *str)
{
	if (!str)
		return (TYPE_ERROR);
	if (ft_isall_word(str))
		return (TYPE_WORD);
	if (ft_strlen(str) == 1 && ft_char_is_something(str[0]) == TYPE_EOF)
		return (TYPE_EOF);
	if (ft_strlen(str) == 1 && ft_char_is_something(str[0]) == TYPE_PIPE)
		return (TYPE_PIPE);
	if (ft_strlen(str) == 1 && ft_char_is_something(str[0]) == TYPE_REDIR_IN)
		return (TYPE_REDIR_OUT);
	if (ft_strlen(str) == 1 && ft_char_is_something(str[0]) == TYPE_REDIR_OUT)
		return (TYPE_REDIR_IN);
	if (ft_strlen(str) == 2 && ft_char_is_something(str[0]) == TYPE_REDIR_OUT
		&& ft_char_is_something(str[1]) == TYPE_REDIR_OUT)
		return (TYPE_APPEND);
	if (ft_strlen(str) == 2 && ft_char_is_something(str[0]) == TYPE_REDIR_IN
		&& ft_char_is_something(str[1]) == TYPE_REDIR_IN)
		return (TYPE_HEREDOC);
	return (TYPE_UNDEFINE);
}

int	ft_check_valid(char c)
{
	if (ft_char_is_something(c) == TYPE_UNDEFINE)
		return (0);
	return (1);
}

void	print_type(int type)
{
	if (type < TYPE_WORD || type > TYPE_UNDEFINE)
		return ;
	ft_printf("\"");
	if (type == TYPE_WORD)
		ft_printf("WORD");
	else if (type == TYPE_REDIR_IN)
		ft_printf("R-IN");
	else if (type == TYPE_REDIR_OUT)
		ft_printf("R-OUT");
	else if (type == TYPE_HEREDOC)
		ft_printf("HDOC");
	else if (type == TYPE_APPEND)
		ft_printf("APPD");
	else if (type == TYPE_PIPE)
		ft_printf("PIPE");
	else if (type == TYPE_UNDEFINE)
		ft_printf("UNDEF");
	ft_printf("\"");
}

int	is_cmd_opprt(int type)
{
	if (type >= TYPE_REDIR_IN && type <= TYPE_PIPE)
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 17:34:50 by laaubry           #+#    #+#             */
/*   Updated: 2026/08/30 19:09:44 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(const char *s1)
{
	size_t	i;
	size_t	len;
	char	*dest;

	len = ft_strlen(s1);
	i = 0;
	dest = malloc(len + 1);
	if (!dest)
		return (NULL);
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	count_variables(char **envp)
{
	int	count;

	count = 0;
	while (envp[count] != NULL)
		count++;
	return (count);
}

char	**copy_env(char **envp)
{
	int		i;
	int		count;
	char	**new_env;

	i = 0;
	count = count_variables(envp);
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	while (envp[i] != NULL)
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 14:37:28 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/23 19:05:04 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n - 1 && (s1[i] == s2[i]) && (s1[i] != 0 && s2[i] != 0))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] == s2[i]) && (s1[i] != 0 && s2[i] != 0))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strjoin_gp(char *s1, char *s2, t_rumba **rumba_mk1)
{
	char	*res;
	size_t	i;
	size_t	len_s1;

	len_s1 = ft_strlen(s1);
	res = malloc_rumba((len_s1 + ft_strlen(s2) + 1) * sizeof(char), rumba_mk1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != 0)
	{
		res[i] = s1[i];
		i++;
	}
	while (s2[i - len_s1] != 0)
	{
		res[i] = s2[i - len_s1];
		i++;
	}
	res[i] = '\0';
	return (res);
}

// MAIN

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

// SIGNALS.C

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 15:30:12 by laaubry           #+#    #+#             */
/*   Updated: 2026/08/22 17:51:22 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

void	init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
