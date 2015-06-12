/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_management2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/25 17:31:43 by fschuber          #+#    #+#             */
/*   Updated: 2015/06/09 21:14:42 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char				**get_dirs(DIR *directory
						, char *dirname, int items, int *options)
{
	char			**list;
	int				i;
	struct dirent	*entry;

	i = 0;
	list = malloc(sizeof(char*) * (items + 1));
	while ((entry = readdir(directory)))
	{
		if (entry->d_type == DT_DIR
			&& ((entry->d_name)[0] != '.' || options[2] == 1)
			&& ft_strcmp(entry->d_name, ".")
			&& ft_strcmp(entry->d_name, ".."))
			list[i++] = ft_strjoin(dirname, ft_strdup(entry->d_name));
	}
	list[i] = NULL;
	return (list);
}

t_node				*sort_list(t_node *list, int *options)
{
	t_node			*current;

	current = list;
	if (options[4] == 1)
		list = sort_by_time_modified(list, NULL);
	else if (options[3] == 1)
	{
		list = sort_ascii(list);
		list = invert_list(list);
	}
	else
		list = sort_ascii(list);
	return (list);
}

long int			*get_sizes(t_node *current, int *options)
{
	long int		*sizes;

	sizes = malloc(sizeof(long int) * 6);
	sizes[0] = 0;
	sizes[1] = 0;
	sizes[2] = 0;
	sizes[3] = 0;
	sizes[4] = 0;
	sizes[5] = 0;
	while(current)
	{
		if ((current->name)[0] != '.' || options[2] == 1)
		{
			if (ft_strlen(ft_itoa(current->info.st_nlink)) > sizes[0])
				sizes[0] = ft_strlen(ft_itoa(current->info.st_nlink));
			if (ft_strlen(ft_itoa(current->info.st_size)) > sizes[1])         
				sizes[1] = ft_strlen(ft_itoa(current->info.st_size));
				sizes[2] += current->info.st_blocks;
			if (getgrgid(current->info.st_gid)
				&& sizes[3] < ft_strlen(getgrgid(current->info.st_gid)->gr_name))
				sizes[3] = ft_strlen(getgrgid(current->info.st_gid)->gr_name);
			else if (sizes[3] < ft_strlen(ft_itoa(current->info.st_gid)))
				sizes[3] = ft_strlen(ft_itoa(current->info.st_gid));
			if (getpwuid(current->info.st_uid)
				&& sizes[5] < ft_strlen(getpwuid(current->info.st_uid)->pw_name))
				sizes[5] = ft_strlen(getpwuid(current->info.st_uid)->pw_name);
			else if (sizes[5] < ft_strlen(ft_llitoa(current->info.st_uid)))
				sizes[5] = ft_strlen(ft_llitoa(current->info.st_uid));
		}
		current = current->next;
	}
	return (sizes);
}
