/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process_group_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 20:01:48 by djean             #+#    #+#             */
/*   Updated: 2017/04/08 15:13:18 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include "exec.h"
#include "errors.h"
#include "sig.h"
#include "opt.h"
#include "exit_status.h"

static int	exec_set_process_group_child_side(int pid, int pgid)
{
	if (setpgid(pid, pgid) == -1)
	{
		error_set_context("child: setpgid: %s", strerror(errno));
		return (ERROR_);
	}
	return (OK_);
}

/*
** If the child perform the 'setpgid' and 'exec' call before the parent
** is scheduled, then the 'setpgid' call from the parent side will fail
** with errno set to 'EACCES'.
** Thus we avoid reporting an error in that case.
*/

static int	exec_set_process_group_parent_side(int pid, int pgid)
{
	if (setpgid(pid, pgid) == -1 && errno != EACCES)
	{
		error_set_context("setpgid: %s", strerror(errno));
		return (ERROR_);
	}
	return (OK_);
}

static int	exec_set_foreground_process_group(pid_t pgid)
{
	if (opt_is_set(OPT_INTERACTIVE))
	{
		if (tcsetpgrp(E_TTY_FD, pgid) == -1)
		{
			error_set_context("tcsetpgrp: %s", strerror(errno));
			return (ERROR_);
		}
	}
	return (OK_);
}

void		exec_child_set_context(void)
{
	signal_unmute_exit_signals();
	signal_unset_ignored_signals();
	signal_unblock_exit_signals();
	if (exec_set_process_group_child_side(0, 0) != OK_)
	{
		error_print("execution: child: failed to set process group");
		_exit(-1);
	}
	if (exec_set_foreground_process_group(getpgrp()) != OK_)
	{
		error_print("execution: child: failed to get the controlling terminal");
		exit(-1);
	}
}

int			exec_parent_wait_child_process_group(pid_t child_pgid,
		bool set_context)
{
	int	status;

	if (set_context)
	{
		if (exec_set_process_group_parent_side(child_pgid, child_pgid) != OK_)
			error_print("execution: parent: failed to set process group");
		else if (exec_set_foreground_process_group(child_pgid) != OK_)
			error_print("execution: parent: failed to give the controlling"
					" terminal to the child process");
	}
	status = wait_child_process_group(child_pgid, child_pgid);
	exit_status_set_last(status);
	if (set_context && exec_set_foreground_process_group(getpgrp()) != OK_)
	{
		error_print("execution: parent: failed to get back the controlling"
				" terminal");
		exit(-1);
	}
	return (status);
}
