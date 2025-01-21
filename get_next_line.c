/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atigzim <atigzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:56:29 by atigzim           #+#    #+#             */
/*   Updated: 2025/01/21 14:44:59 by atigzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	find_new_line(char *buffer, int bytes)
{
	int	i;

	i = 0;
	bytes--;
	if (!buffer)
		return (0);
	while (bytes >= 0)
	{
		if (buffer[bytes] == '\n')
			return (1);
		bytes--;
	}
	return (0);
}

int	size_new_line(char *s)
{
	int	i;

	i = 0;
	if (s[0] == '\0')
		return (-2);
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i])
		i++;
	return (i);
}

char	*read_line(int fd, char **buffer)
{
	char	*str;
	int		bytes;
	int		i;
	char	*tmp;

	str = NULL;
	i = 0;
	while (i != 1)
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
		i = find_new_line(*buffer, bytes);
		free(tmp);
	}
	return (str);
}

char	*append_buffer(char **full_buffer, char *line, int fd, char **buffer)
{
	char (*tmp), (*str);
	int (i);
	if (find_new_line((*full_buffer), ft_strlen((*full_buffer))) == 1)
	{
		i = size_new_line((*full_buffer));
		line = ft_substr((*full_buffer), 0, i);
		tmp = (*full_buffer);
		(*full_buffer) = ft_substr((*full_buffer), i, ft_strlen((*full_buffer))
				- i);
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
		i = size_new_line(tmp);
		if (i == -2)
			return (free(tmp), NULL);
		line = ft_substr(tmp, 0, i);
		(*full_buffer) = ft_substr(tmp, i, ft_strlen(tmp) - i);
		free(tmp);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*full_buffer;
	int			i;

	char(*buffer), (*line), (*tmp);
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	line = NULL;
	if (!full_buffer)
	{
		line = read_line(fd, &buffer);
		if (!line)
		{
			free(buffer);
			return (NULL);
		}
		i = size_new_line(line);
		if (i == -2)
			return (free(line), free(buffer), NULL);
		full_buffer = ft_substr(line, i, ft_strlen(line) - i);
		tmp = line;
		line = ft_substr(tmp, 0, i);
		free(tmp);
	}
	else
		line = append_buffer(&full_buffer, line, fd, &buffer);
	if (!line[0])
		return (free(line), (NULL));
	return (free(buffer), line);
}

// int main ()
// {
// 	char *ptr;
// 	int fd;
// 	fd = open("abdo.txt",O_RDONLY);
// 	if(fd <= 0)
// 		return(-1);
// 	ptr = get_next_line(fd);
// 	printf("%s",ptr);
// 	while (ptr)
// 	{
// 		printf("%s",ptr);
// 		free(ptr);
// 		ptr = get_next_line(fd);
// 	}
// }
