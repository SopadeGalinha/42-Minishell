/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_replace.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 20:17:05 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/05 20:17:05 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	char	*replace_aux(char *str, char *old, char *new, int start)
{
	char	*temp;
	char	*aux;

	temp = ft_substr(str, 0, start);
	aux = ft_strjoin(temp, new);
	free(temp);
	temp = ft_strjoin(aux, str + start + ft_strlen(old));
	free(aux);
	free(str);
	return (temp);
}

char	*ft_str_replace(char *str, char *old, char *new)
{
	int		n;
	int		start;
	char	*result;
	char	*temp;

	start = 0;
	result = ft_strdup(str);
	while (result[start])
	{
		n = 0;
		while (result[start + n] && result[start + n] == old[n])
			n++;
		if (!old[n])
		{
			temp = replace_aux(result, old, new, start);
			result = temp;
			start += ft_strlen(new);
			free(temp);
		}
		else
			start++;
	}
	return (result);
}
