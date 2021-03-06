/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 20:22:43 by djean             #+#    #+#             */
/*   Updated: 2017/04/01 20:22:46 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "string_42.h"
#include "misc.h"

#define NON_ASCII_MASK (0x8080808080808080U)

static bool	is_buff_ascii(const void *buff, size_t buff_size)
{
	const uint64_t	*chunks_buff;
	size_t			chunks_nbr;
	size_t			i;

	chunks_nbr = buff_size / sizeof(uint64_t);
	chunks_buff = buff;
	i = 0;
	while (i < chunks_nbr)
	{
		if (chunks_buff[i] & NON_ASCII_MASK)
			return (false);
		i++;
	}
	return (true);
}

t_string	*string_read_from_fd(t_string *s, int fd)
{
	char		buff[MEM_PAGE_SIZE];
	ssize_t		ret;

	while ((ret = read(fd, buff, MEM_PAGE_SIZE)) > 0)
	{
		ft_memset(buff + ret, '\0', ~((size_t)ret - 1) % 8);
		if (!is_buff_ascii(buff, (size_t)ret))
			return (NULL);
		fatal_malloc(string_ncat(s, buff, (size_t)ret));
	}
	if (ret == -1)
		return (NULL);
	return (s);
}

int			string_write_to_fd(const t_string *s, int fd)
{
	ssize_t		ret;
	size_t		offset;

	offset = 0;
	while ((ret = write(fd, s->str + offset, s->len - offset)) > 0)
		offset += (size_t)ret;
	return ((int)ret);
}
