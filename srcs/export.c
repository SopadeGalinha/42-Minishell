
#include "../includes/minishell.h"

	/*if ((ft_strncmp(input, "export A=asb", ft_strlen(input)) == 0))
		{
			// Essa parte faz incremento na lista do export caso a variavel seja passada sem '='
			int length = ft_strlen(input);
    		char *output = (char *)malloc(length);
			if (output == NULL) {
				perror("Failed to allocate memory");
				exit(EXIT_FAILURE);
			}
			int output_index = 0;
			for (int i = length - 1; i >= 0 && input[i] != ' '; i--) {
				output[output_index++] = input[i];
			}
			int start = 0;
			int end = output_index - 1;
			while (start < end) {
				char temp = output[start];
				output[start] = output[end];
				output[end] = temp;
				start++;
				end--;
			}
			printf("%s", output);
			//update_lists(output);
		}*/

void insert_sorted(t_env **export_list, t_env *env)
{
	t_env *current = *export_list;
	
	if (*export_list == NULL || ft_strncmp(env->key, (*export_list)->key, strlen(env->key)) < 0)
	{
		env->next = *export_list;
		*export_list = env;
	}
	else
	{
		current = *export_list;
		while (current->next != NULL && ft_strncmp(env->key, current->next->key, strlen(env->key)) > 0)
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
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->line = ft_strdup(line);
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