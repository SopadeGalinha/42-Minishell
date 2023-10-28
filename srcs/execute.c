/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:18:41 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/28 17:52:38 by jhogonca         ###   ########.fr       */
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
/*void	ft_echo(t_token *tokens)
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
	else if (ft_strncmp(shell->tokens->data, "exit", ft_strlen("exit")) == 0
		|| ft_strncmp(shell->tokens->data, "quit", ft_strlen("quit")) == 0)
	{
		if (shell->tokens->next != NULL && ft_isdigit_str(shell->tokens->next->data))
			exit(ft_atoi(shell->tokens->next->data));
		exit(0); 
}
*/

void	execute(t_shell *shell)
{
	t_shell	*sh;
	t_token	*tk;

	sh = shell;
	tk = sh->tokens;	
	if (ft_strncmp(shell->pipes->cmds[0], "exit", ft_strlen("exit")) == 0)
		exit(0);
}
