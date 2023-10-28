/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:43:34 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/28 19:43:34 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}

void	init_shell(t_shell *shell, char **env)
{
	ft_handle_signals();
	shell->error = NO_ERROR;
	shell->env = init_env(env);
	shell->exp = init_export(shell->env);
}

bool	print_error(char *error, int exit_code)
{
	ft_printf_fd(2, "%s\n", error);
	g_exit_status = exit_code;
	return (false);
}

void	get_input(t_shell *shell)
{
	shell->input = readline(MINISHELL);
	if (shell->input && !ft_isspace_str(shell->input))
		add_history(shell->input);
	if (shell->input)
		shell->input = ft_strtrim(shell->input, " \t");
}
