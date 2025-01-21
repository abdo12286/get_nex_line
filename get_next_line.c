/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atigzim <atigzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:56:29 by atigzim           #+#    #+#             */
/*   Updated: 2025/01/21 19:26:16 by atigzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_line(int fd, char **buffer)
{
	char	*str;
	int		bytes;
	int		i;
	char	*tmp;

	str = NULL;
	i = -1;
	while (i == -1)
	{
		bytes = read(fd, *buffer, BUFFER_SIZE);
		if (bytes < 0)
			return (NULL);
		if (bytes == 0)
			break ;
		(*buffer)[bytes] = '\0';
		if (!str)
			str = ft_strjoin(str, "");
		tmp = str;
		str = ft_strjoin(str, *buffer);
		i = check_new_line(*buffer);
		free(tmp);
	}
	return (str);
}

char	*append_buffer(char **full_buffer, int fd, char **buffer)
{
	int		i;
	char	*tmp;
	char	*str;
	char	*line;
	i = check_new_line(*full_buffer);
	if (i != -1)
	{
		line = ft_substr((*full_buffer), 0, i + 1);
		tmp = (*full_buffer);
		(*full_buffer) = ft_substr((*full_buffer), i + 1,
				ft_strlen((*full_buffer)) - (i + 1));
		free(tmp);
	}
	else
	{
		str = read_line(fd, buffer);
		if (!str)
		{
			if (*full_buffer)
			{
				line = ft_strdup(*full_buffer);
				free(*full_buffer);
				*full_buffer = NULL;
				return (line);
			}
			return (NULL);
		}
		tmp = ft_strjoin((*full_buffer), str);
		(free(*full_buffer), free(str));
		i = check_new_line(tmp);
		line = ft_substr(tmp, 0, i + 1);
		(*full_buffer) = ft_substr(tmp, i + 1, ft_strlen(tmp) - (i - 1));
		free(tmp);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*full_buffer;
	int			i;
	char		*buffer;
	char		*line;
	char		*tmp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!full_buffer)
	{
		line = read_line(fd, &buffer);
		if (!line)
			return (free(buffer), NULL);
		i = check_new_line(line);
		if (i == -1)
		{
			if (ft_strlen(line) > 0)
				i = ft_strlen(line);
		}
		tmp = line;
		line = ft_substr(tmp, 0, i + 1);
		full_buffer = ft_substr(tmp, i + 1, ft_strlen(tmp) - (i + 1));
		free(tmp);
	}
	else
		line = append_buffer(&full_buffer, fd, &buffer);
	if (!line[0])
		return (free(line), NULL);
	return (free(buffer), line);
}

int	main(void)
{
	char	*ptr;
	int		fd;

	fd = open("abdo.txt", O_RDONLY);
	if (fd <= 0)
		return (-1);
	while ((ptr = get_next_line(fd)) != NULL)
	{
		printf("%s", ptr);
		free(ptr);
	}
}
