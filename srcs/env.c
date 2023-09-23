
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

char	**get_key(char **envp)
{
	int i = 0;

	while(envp[i])
		i++;
	char **key_array = malloc(sizeof(char *) * i + 1);
	i = 0;
	while (envp[i])
	{
		key_array[i] = ft_strdup_equal_key(envp[i]);
		i++;
	}
	key_array[i] = NULL;
	return (key_array);
}

char	**get_value(char **envp)
{
	int i = 0;

	while(envp[i])
		i++;
	char **value_array = malloc(sizeof(char *) * i + 1);
	i = 0;
	while (envp[i])
	{
		value_array[i] = ft_strdup_equal_value(envp[i]);
		i++;
	}
	value_array[i] = NULL;
	return (value_array);
}


char	**get_full(char **envp)
{
	int i = 0;

	while(envp[i])
		i++;
	char **full_array = malloc(sizeof(char *) * i + 1);
	i = 0;
	while (envp[i])
	{
		full_array[i] = ft_strdup(envp[i]);
		i++;
	}
	full_array[i] = NULL;
	return (full_array);
}

t_env_arr	*init_env(char **envp)
{
	int i = 0;
	char	**key_array;
	char	**value_array;
	char	**full_array;
	t_env_arr	*env;

	env = (t_env_arr *)malloc(sizeof(t_env_arr));
	env->key_array = get_key(envp);
	env->value_array = get_value(envp);
	env->full_array = get_full(envp);
	return (env);
}