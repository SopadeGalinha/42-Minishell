/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:47:52 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/30 13:47:52 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int				i;
	unsigned char	*str1;
	unsigned char	*str2;
	int				len1;
	int				len2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	i = -1;
	while (++i < len1 && i < len2)
		if (str1[i] != str2[i])
			return ((int)(str1[i] - str2[i]));
	if (len1 != len2)
		return ((int)(str1[i] - str2[i]));
	return (0);
}
