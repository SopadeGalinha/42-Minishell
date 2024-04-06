/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 00:00:01 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/14 00:00:01 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_isspace(const char str)
{
	if (str == ' ' || str == '\t' || str == '\n' || \
	str == '\v' || str == '\f' || str == '\r')
		return (true);
	return (false);
}
