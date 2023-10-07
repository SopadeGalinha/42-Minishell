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

char custom_callback(unsigned int index, char c)
{
    return ft_toupper(c);
}

int main(void)
{
    char *str = "ola";

    printf("%s\n", str);
    char *result = ft_strmapi(str, custom_callback);
    printf("%s\n", result);
    free(result); // Don't forget to free the memory allocated by ft_strmapi
    return (0);
}