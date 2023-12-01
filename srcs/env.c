/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:08:16 by jhogonca          #+#    #+#             */
/*   Updated: 2023/12/01 12:49:13 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strdup_equal_value(const char *src)
{
	char	*cpy;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	while (src[i] != '=' && src[i])
		i++;
	if (src[i] == '\0')
		return (NULL);
	k = ++i;
	while (src[i++] != '\0')
		j++;
	cpy = (char *)malloc(sizeof(char) * (j + 1));
	if (!cpy)
		return (NULL);
	j = 0;
	while (src[k] != '\0')
		cpy[j++] = src[k++];
	cpy[j] = '\0';
	return (cpy);
}

char	*ft_strdup_equal_key(const char *src)
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

int	create_add_node_to_list(t_env **head, char *line)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
	{
		ft_printf_fd(STDOUT_FILENO, "Failed to allocate memory for t_env");
		return (0);
	}
	new->key = ft_strdup_equal_key(line);
	new->value = ft_strdup_equal_value(line);
	new->line = ft_strdup(line);
	if (!head)
		new->next = NULL;
	else
		new->next = *head;
	*head = new;
	return (0);
}

t_env	*init_env(char **envp)
{
	int		i;
	t_env	*env_list;

	i = 0;
	env_list = NULL;
	while (envp[i])
		i++;
	while (--i >= 0)
		create_add_node_to_list(&env_list, envp[i]);
	return (env_list);
}

void	print_list(t_shell *shell, int flag, t_pipes *pipes)
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