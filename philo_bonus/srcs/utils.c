/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:17:29 by sudas             #+#    #+#             */
/*   Updated: 2025/10/13 15:23:00 by sudas            ###   ########.fr       */
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

void	msleep(long int msec)
{
	t_eval		tv;
	long int	start_sec;
	long int	start_usec;
	long int	dmsec;

	gettimeofday(&tv, NULL);
	start_sec = tv.tv_sec;
	start_usec = tv.tv_usec;
	dmsec = 0;
	// printf ("started_msleep msec: %ld, usec: %ld\n", dmsec, tv.tv_usec);
	while (dmsec < msec)
	{
		usleep(100);
		gettimeofday(&tv, NULL);
		dmsec = (tv.tv_sec - start_sec) * 1000
			+ (tv.tv_usec - start_usec) / 1000;
	}
	// printf ("Stopped_msleep msec: %ld, usec: %ld\n", dmsec, tv.tv_usec);
}
