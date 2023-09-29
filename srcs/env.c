
#include "../includes/minishell.h"

char	*ft_strdup_equal_value(const char *src)
{
	char	*cpy;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	while (src[i] != '=')
		i++;
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

	i = 0;
	while (src[i] != '=')
		i++;
	cpy = (char *)malloc(sizeof(char) * i + 1);
	if (!cpy)
		return (NULL);
	i = 0;
	while (*src != '=')
		cpy[i++] = *src++;
	cpy[i] = '\0';
	return (cpy);
}

int	create_add_node_to_list(t_env **head, char *line)
{
	t_env *new = (t_env *)malloc(sizeof(t_env));
	if (!new) {
		ft_printf_fd(1, "Failed to allocate memory for t_env");
		return (0);
	}
	new->key = ft_strdup_equal_key(line);
	new->value = ft_strdup_equal_value(line);
	new->line = line;
	new->next = NULL;

	new->next = *head;
	*head = new;
	return (0);
}

t_env	*init_env(char **envp)
{
	int i = 0;
	t_env *env_list = NULL;

	while(envp[i])
		i++;
	while (--i  >=  0)
		create_add_node_to_list(&env_list, envp[i]);

	return (env_list);
}

void print_env_list(t_env *head, int flag)
{
	t_env *current = head;
	while (current != NULL)
	{
		if (flag)
			ft_printf_fd(1, "%s\n", current->line);
		else
			ft_printf_fd(1, "declare -x %s\n", current->line);
		current = current->next;
	}
}
