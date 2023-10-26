/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:18:41 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/22 12:25:51 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", ft_strlen("echo")) == 0)
		return (true);
	if (ft_strncmp(cmd, "cd", ft_strlen("cd")) == 0)
		return (true);
	if (ft_strncmp(cmd, "pwd", ft_strlen("pwd")) == 0)
		return (true);
	if (ft_strncmp(cmd, "export", ft_strlen("export")) == 0)
		return (true);
	if (ft_strncmp(cmd, "unset", ft_strlen("unset")) == 0)
		return (true);
	if (ft_strncmp(cmd, "env", ft_strlen("env")) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit", ft_strlen("exit")) == 0
		|| ft_strncmp(cmd, "quit", ft_strlen("quit")) == 0)
		return (true);
	return (false);
}

void	ft_echo(t_token *tokens)
{
	while (tokens)
	{
		ft_printf_fd(1, "%s", tokens->data);
		if (tokens->next)
			ft_printf_fd(1, " ");
		tokens = tokens->next;
	}
	ft_printf_fd(1, "\n");
}

void	ft_cd(t_token *tokens, t_env *env)
{
	char	*path;

	if (tokens == NULL)
	{
		path = get_env_value(env, "HOME");
		if (path == NULL){
			ft_printf_fd(2, "cd: HOME not set\n");
			return ;
		}
	}
	else
		path = tokens->data;
	if (chdir(path) == -1)
		ft_printf_fd(2, "cd: %s: %s\n", path, strerror(errno));
}

void	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	ft_printf_fd(1, "%s\n", path);
	free(path);
}

static bool	ft_isdigit_str(char *str)
{
	int	i;

	i = -1;
	if (str[0] == '-' || str[0] == '+')
		i = 0;
	while (str[++i])
		if (str[i] < '0' || str[i] > '9')
			return (false);
	return (true);
}
int	token_checker(char *str)
{
	int	i = 0;

	if (ft_isdigit(str[0]))
	{
		printf("minishell: export: `%s': not a valid identifier\n", str);
		return (-1);
	}
	else if (str[0] == '_' && str[1])
	{
		if(!ft_isalnum(str[1]) && str[1] != '_')
		{
			printf("minishell: export: `%s': not a valid identifier\n", str);
			return (-1);
		}
		else
			return (1);
	}
	else if (str[0] == '_' && (str[1] == '=' || str[1] == '\0'))
	{
		//printf("Apenas UNDERLINE o codigo nao faz nada\n");
		return (-2);
	}
	return (1);
}


void	ft_export(t_shell *shell)
{
    t_token *current_token = shell->tokens->next;

	if (shell->tokens->next)
	{
		while (current_token)
		{
			if(token_checker(current_token->data) > 0)
			{
				if (ft_strchr(current_token->data, '='))
					update_lists(shell, current_token->data, 1);
				else if (!ft_strchr(current_token->data, '='))
					update_lists(shell, current_token->data, 0);
			}
			current_token = current_token->next;
		}
	}
    else
        print_list(shell->exp, 0);
}

void	execute_builtin(t_shell *shell)
{
	if (ft_strncmp(shell->tokens->data, "echo", ft_strlen("echo")) == 0)
		ft_echo(shell->tokens->next);
	else if (ft_strncmp(shell->tokens->data, "cd", ft_strlen("cd")) == 0)
		ft_cd(shell->tokens->next, shell->env);
	else if (ft_strncmp(shell->tokens->data, "pwd", ft_strlen("pwd")) == 0)
		ft_pwd();
	else if (ft_strncmp(shell->tokens->data, "export", ft_strlen("export")) == 0)
		ft_export(shell);
	else if (ft_strncmp(shell->tokens->data, "unset", ft_strlen("unset")) == 0)
	{
		if (shell->tokens->next)
			ft_unset(shell, shell->tokens->next->data);
	}
	else if (ft_strncmp(shell->tokens->data, "env", ft_strlen("env")) == 0)
		print_list(shell->env, 1);
	/* else if (ft_strncmp(shell->tokens->data, "exit", ft_strlen("exit")) == 0
		|| ft_strncmp(shell->tokens->data, "quit", ft_strlen("quit")) == 0)
	{
		if (shell->tokens->next != NULL && ft_isdigit_str(shell->tokens->next->data))
			exit(ft_atoi(shell->tokens->next->data));
		exit(0); */
}

int lstsize(t_token *lst)
{
	int i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return i;
}

void	execute(t_shell *shell)
{
	t_shell	*sh;
	t_token	*tk;

	sh = shell;
	tk = sh->tokens;
	if (is_builtin(tk->data))
		execute_builtin(sh); 
	
}