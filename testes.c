/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 20:15:21 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/05 20:15:21 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "includes/libft/libft.h"

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
			temp = ft_strjoin(ft_strjoin(ft_substr(result, 0, start), new), \
			result + start + ft_strlen(old));
			free(result);
			result = temp;
			start += ft_strlen(new);
		}
		else
			start++;
	}
	return (result);
}
