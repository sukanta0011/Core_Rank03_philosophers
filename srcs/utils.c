/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:17:29 by sudas             #+#    #+#             */
/*   Updated: 2025/10/10 12:01:29 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	str_to_unum(char *str, long int *num)
{
	int			sign;
	int			i;

	i = 0;
	sign = 1;
	*num = 0;
	if (str[i] == '-')
		return (0);
	else if (str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			(*num) = (*num) * 10 + (str[i] - '0');
			i++;
		}
		else
			return (0);
	}
	(*num) = (*num) * sign;
	return (1);
}
