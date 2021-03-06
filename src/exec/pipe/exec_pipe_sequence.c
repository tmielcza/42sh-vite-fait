/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_sequence.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 20:01:48 by djean             #+#    #+#             */
/*   Updated: 2017/04/01 20:10:18 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include "ast.h"
#include "exec.h"
#include "array_42.h"
#include "errors.h"

#define SELF_PGID	0

/*
** stdin -> pipe, read end
*/

static int	exec_last_child(const t_ast_node *node, int left_read,
		const t_string *input)
{
	pid_t	child;

	assert(node != NULL);
	child = -1;
	if (exec_fork(&child) != OK_)
		pipe_kill_pipe_sequence();
	if (child == 0)
	{
		if (pipe_replace_stdin(left_read) == OK_)
			exec_pipe_command(node, input);
		pipe_kill_pipe_sequence();
	}
	else
	{
		if (exec_close_fd(left_read) != OK_)
			pipe_kill_pipe_sequence();
	}
	return (child);
}

/*
** stdin -> pipe, read end
** stdout -> pipe, write end
*/

static int	exec_middle_child(const t_ast_node *node, int left_read,
		const t_string *input)
{
	t_pipe	right;
	pid_t	child;

	assert(node != NULL);
	child = -1;
	if (pipe_init(&right) != OK_ || exec_fork(&child) != OK_)
		pipe_kill_pipe_sequence();
	if (child == 0)
	{
		if (pipe_replace_stdin(left_read) == OK_
				&& pipe_replace_stdout(right.write) == OK_
				&& exec_close_fd(right.read) == OK_)
			exec_pipe_command(node, input);
		pipe_kill_pipe_sequence();
	}
	else
	{
		if (exec_close_fd(right.write) != OK_
				|| exec_close_fd(left_read) != OK_)
			pipe_kill_pipe_sequence();
	}
	return (right.read);
}

/*
** stdout -> pipe, write end (right write)
** return the read end of the pipe
*/

static int	exec_first_child(const t_ast_node *node, const t_string *input)
{
	t_pipe	right;
	pid_t	child;

	assert(node != NULL);
	child = -1;
	if (pipe_init(&right) != OK_ || exec_fork(&child) != OK_)
		pipe_kill_pipe_sequence();
	if (child == 0)
	{
		if (pipe_replace_stdout(right.write) == OK_
				&& exec_close_fd(right.read) == OK_)
			exec_pipe_command(node, input);
		pipe_kill_pipe_sequence();
	}
	else
	{
		if (exec_close_fd(right.write) != OK_)
			pipe_kill_pipe_sequence();
	}
	return (right.read);
}

/*
** We are in the _control fork_
*/

void		exec_pipe_sequence(const t_ast_node *node, const t_string *input)
{
	t_array	pipe_nodes_stack;
	pid_t	last_pid;
	int		read_end;

	pipe_nodes_stack = gather_childrens_nodes(node, E_AST_PIPE_SEQUENCE);
	array_pop(&pipe_nodes_stack, &node);
	read_end = exec_first_child(node, input);
	while (pipe_nodes_stack.len > 1)
	{
		array_pop(&pipe_nodes_stack, &node);
		read_end = exec_middle_child(node, read_end, input);
	}
	array_pop(&pipe_nodes_stack, &node);
	last_pid = exec_last_child(node, read_end, input);
	_exit(wait_child_process_group(last_pid, SELF_PGID));
}
