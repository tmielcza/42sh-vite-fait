/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tmpfile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 20:37:04 by djean             #+#    #+#             */
/*   Updated: 2017/04/01 20:37:06 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include "ft_printf.h"
#include "errors.h"

#ifdef P_tmpdir
# define DFLT_PATH P_tmpdir "/"
#else
# define DFLT_PATH "/tmp/"
#endif

static char	*gen_candidate(const char *prefix)
{
	struct timeval	time;
	char			*filename;
	int				ret;

	if (gettimeofday(&time, NULL) == -1)
	{
		error_set_context("gettimeofday: %s", strerror(errno));
		return (NULL);
	}
	ret = ft_asprintf(&filename, "%s%s%zu%zu",
			DFLT_PATH,
			prefix,
			time.tv_sec,
			time.tv_usec);
	if (ret == -1)
	{
		error_set_context("write: %s", strerror(errno));
		return (NULL);
	}
	return (filename);
}

static char	*gen_tmp_name(const char *prefix)
{
	char	*filename;

	if (access(DFLT_PATH, W_OK) == -1)
	{
		error_set_context(DFLT_PATH " : access: %s", strerror(errno));
		return (NULL);
	}
	filename = gen_candidate(prefix);
	while (filename != NULL && (access(filename, F_OK) == 0 || errno != ENOENT))
	{
		free(filename);
		filename = gen_candidate(prefix);
	}
	return (filename);
}

char		*ft_tmpfile(const char *prefix)
{
	if (prefix == NULL)
		prefix = "";
	return (gen_tmp_name(prefix));
}
