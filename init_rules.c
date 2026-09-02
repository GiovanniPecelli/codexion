/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_rules.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 11:50:12 by marvin            #+#    #+#             */
/*   Updated: 2026/09/02 13:06:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_scheduler(char *arg, t_rules *rules)
{
	if (strcmp(arg, "fifo") == 0)
		rules->is_fifo = 1;
	else if (strcmp(arg, "edf") == 0)
		rules->is_fifo = 0;
	else
	{
		printf("Scheduler not valid\n");
		return (-1);
	}
	return (0);
}

/*
** Fills the t_rules structure converting valid strings to integers.
** Returns -1 if the scheduler name is invalid.
*/
int	fill_rules(char **argv, t_rules *rules)
{
	if (atoi(argv[1]) <= 0)
	{
		printf("%s: invalid value, must be positive\n", argv[1]);
		return (-1);
	}
	rules->num_coders = atoi(argv[1]);
	if (atoi(argv[2]) <= 0)
	{
		printf("%s: invalid value, must be positive\n", argv[2]);
		return (-1);
	}
	rules->time_to_burnout = atoi(argv[2]);
	rules->time_to_compile = atoi(argv[3]);
	rules->time_to_debug = atoi(argv[4]);
	rules->time_to_refactor = atoi(argv[5]);
	rules->compiles_required = atoi(argv[6]);
	rules->dongle_cooldown = atoi(argv[7]);
	return (check_scheduler(argv[8], rules));
}

/*
** Checks that every args[i] contain only valid digits
** skipping spaces and '+'.
** Returns 0 on success, or -1 if non-numeric characters are found.
*/
int	is_number(char *arg)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	if (!arg || !arg[0])
		return (-1);
	while (arg[i] == ' ' || (arg[i] >= '\t' && arg[i] <= '\r'))
		i++;
	if (arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (-1);
		i++;
	}
	return (n);
}

/*
** Main parsing routine: validates argument count and format.
** Calls fill_rules to populate the rules structure if input is clean.
*/
int	init_rules(int argc, char **argv, t_rules *rules)
{
	int	i;

	if (argc != 9)
	{
		printf("Invalid number of arguments\n");
		return (-1);
	}
	i = 1;
	while (i < 8)
	{
		if (is_number(argv[i]) < 0)
		{
			printf("%s: invalid format or negative number\n", argv[i]);
			return (-1);
		}
		i++;
	}
	if (fill_rules(argv, rules) == -1)
		return (-1);
	return (0);
}
