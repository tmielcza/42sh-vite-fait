/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_caps.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 20:19:58 by djean             #+#    #+#             */
/*   Updated: 2017/04/01 20:20:01 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <unistd.h>
#include <term.h>
#include "errors.h"
#include "macros_42.h"
#include "terminal.h"

static const char	*g_needed_caps_list[] = {
	CAPS_CURSOR_UP,
	CAPS_CURSOR_DOWN,
	CAPS_CURSOR_LEFT,
	CAPS_CURSOR_RIGHT,
	CAPS_CLEAR_EOL,
	CAPS_CLEAR_EOS,
	CAPS_SCROLL_UP,
};

static int	wrap_putchar(int c)
{
	return ((int)write(STDERR_FILENO, &c, 1));
}

void		terminal_move_cursor(const char *cap, unsigned len)
{
	char	buf[32];
	char	*pbuf;
	void	*ret;

	pbuf = buf;
	ret = tgetstr((char *)cap, &pbuf);
	assert(ret != NULL);
	pbuf = tgoto(buf, 0, len);
	assert(pbuf != NULL);
	tputs(pbuf, 1, &wrap_putchar);
}

void		terminal_execute_caps(const char *cap)
{
	const char	*str;

	str = tgetstr((char*)cap, NULL);
	assert(str != NULL);
	tputs(str, 1, &wrap_putchar);
}

int			terminal_check_caps(void)
{
	size_t	i;

	i = 0;
	while (i < ARR_SIZ_MAX(g_needed_caps_list))
	{
		if (tgetstr((char*)g_needed_caps_list[i], NULL) == NULL)
		{
			error_set_context("capability '%s' not available",
					g_needed_caps_list[i]);
			return (ERROR_);
		}
		i += 1;
	}
	return (OK_);
}
