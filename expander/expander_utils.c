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
