/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 13:15:04 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:07:54 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	u_error_arg(const char *str)
{
	ft_printf_fd(STDERR_FILENO, MS_ERR "export: ");
	ft_printf_fd(STDERR_FILENO, RESET"'%s': not a valid identifier\n", str);
	g_exit_status = 1;
	return (-1);
}

static void	update_node_values(t_env *current, char *line)
{
	char	*tmp;

	if (!current)
		return ;
	tmp = u_ft_strdup_equal_value(line);
	if (!tmp)
		return ;
	free(current->key);
	free(current->value);
	free(current->line);
	current->key = u_ft_strdup_equal_key(line);
	current->value = u_ft_strdup_equal_value(line);
	current->line = ft_strdup(line);
	free(tmp);
}

void	u_update_node(t_env *lst, char *key, char *line)
{
	t_env	*current;

	current = lst;
	while (current && ft_strcmp(current->key, key) != 0)
		current = current->next;
	update_node_values(current, line);
}

void	u_print_list(t_shell *shell, int flag, t_pipes *pipes)
{
	t_env	*current;

	(void)pipes;
	if (flag)
		current = shell->env;
	else
		current = shell->exp;
	while (current != NULL)
	{
		if (flag)
			ft_printf_fd(STDOUT_FILENO, "%s\n", current->line);
		else
		{
			ft_printf_fd(STDOUT_FILENO, "declare -x %s", current->key);
			if (current->value)
			{
				ft_printf_fd(STDOUT_FILENO, "=");
				ft_printf_fd(STDOUT_FILENO, "\"%s\"\n", current->value);
			}
			else
				ft_printf_fd(2, "\n");
		}
		current = current->next;
	}
}

char	*u_ft_strdup_equal_key(const char *src)
{
	char	*cpy;
	size_t	i;
	size_t	j;

	i = 0;
	while ((src[i] != '=') && src[i])
		i++;
	cpy = (char *)malloc(sizeof(char) * (i + 1));
	if (!cpy)
		return (NULL);
	i = 0;
	j = 0;
	while (src[j] != '=' && src[j])
		cpy[i++] = src[j++];
	cpy[i] = '\0';
	return (cpy);
}
