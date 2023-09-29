
#include "../includes/minishell.h"

void insert_sorted(t_env **export_list, t_env *env)
{
	t_env *current = *export_list;
	
	if (*export_list == NULL || ft_strncmp(env->key, (*export_list)->key, strlen(env->key) + 1) < 0)
	{
		env->next = *export_list;
		*export_list = env;
	}
	else
	{
		current = *export_list;
		while (current->next != NULL && ft_strncmp(env->key, current->next->key, strlen(env->key) + 1) > 0)
			current = current->next;
		env->next = current->next;
		current->next = env;
	}
}

t_env *create_node(char *key, char *value, char *line)
{
    t_env *new_node = (t_env *)malloc(sizeof(t_env));
    if (!new_node)
    {
        perror("Failed to allocate memory for t_env");
        exit(EXIT_FAILURE);
    }
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->line = strdup(line);
    new_node->next = NULL;
    return new_node;
}

t_env *init_export(t_env *env)
{
	t_env	*export_list = NULL;
	t_env	*current = env;
	while (current != NULL)
	{
		t_env *copy_node = create_node(current->key, current->value, current->line);
		insert_sorted(&export_list, copy_node);
		current = current->next;
	}
	return (export_list);
}