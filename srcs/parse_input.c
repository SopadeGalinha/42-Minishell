/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:23:53 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/05 15:23:53 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_cmd(t_token **tokens, char *path_env)
{
	t_token	*current;
	int		i;
	char	*path;
	char	**split_path;
	current = *tokens;

	i = -1;
	split_path = ft_split(path_env, ':');
	while (current != NULL)
	{
		i = -1;
		if (current->type == WHITESPACE)
		{
			current = current->next;
			continue ;
		}
		while (split_path[++i])
		{
			char	*cur_path;
			cur_path = ft_strjoin(split_path[i], "/");
			path = ft_strjoin(cur_path, current->data);
			free(cur_path);
			if (access(path, F_OK) == 0)
				if (access(path, X_OK) == 0)
				{
					current->type = CMD;
					free(path);
					break ;
				}
			free(path);
		}
		current = current->next;
	}
	ft_free_array(split_path);
}

char	*ft_getenv(char **env, char *var_name)
{
	int		i;
	char	*tmp;

	i = -1;
	while (env[++i] != NULL)
		if (ft_strncmp(env[i], var_name, ft_strlen(var_name)) == 0)
			return (ft_substr(env[i], ft_strlen(var_name) + 1, ft_strlen(env[i])));
	return (NULL);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0 && ft_strlen(current->key) == ft_strlen(key))
			return (current->value);
		current = current->next;
	}
	return (ft_strdup(""));
}

void	expand_env(t_env *env, t_token *node)
{
	int		i;
	int		start;
	char	*key;
	char	*tmp;
	char	*value;

	i = -1;
	tmp = NULL;
	key = NULL;
	value = NULL;
	if (node->quote == SINGLE || (node->prev != NULL && node->prev->type == HEREDOC))
		return ;
	while (node->data[++i])
	{
		if (node->data[i] && node->data[i] == '$')
		{
			start = i;
			while (ft_isalnum(node->data[++i]) || node->data[i] == '_')
				;
			key = ft_substr(node->data, start + 1, i - start - 1);
			value = get_env_value(env, key);
			free(key);
			key = ft_substr(node->data, 0, start);
			tmp = ft_strjoin(key, value);
			free(key);
			key = ft_substr(node->data, i, ft_strlen(node->data) - i);
			free(node->data);
			node->data = ft_strjoin(tmp, key);
			free(key);
			i = start + ft_strlen(value) - 1;
			free(node->data);
			node->data = tmp;
		}
	}
}

void	build_cmd(t_token **tokens)
{
	t_token	*current;
	char	*cmd;

	if ((*tokens)->type != WORD)
		return ;
}

bool	parse_input(char *path_env, t_shell *shell)
{
	t_token	*current;

	shell->error = lexical(shell->input, &shell->tokens);
	if (shell->error != NO_ERROR)
		return (false);
	current = shell->tokens;
	while (current)
	{
		current->type = define_token(current->data);
		if (current->type == WORD || current->type == CMD || current->type == ENV)
			expand_env(shell->env, current);
		current = current->next;
	}
	build_cmd(&shell->tokens);
	get_cmd(&shell->tokens, path_env);
	print_tokens(shell->tokens);
	return (true);
}
