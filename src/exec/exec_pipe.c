#include <unistd.h>
#include "exec.h"
#include "errors.h"
#include "ast.h"
#include "array_42.h"

static t_array	gather_piped_children(const t_ast_node *node)
{
	t_array	stack;

	fatal_malloc(array_init(&stack, sizeof(*node)));
	while (node->type == E_AST_PIPE_SEQUENCE)
	{
		fatal_malloc(array_push(&stack, &node->right));
		node = node->left;
	}
	array_push(&stack, &node);
	return (stack);
}

static pid_t	exec_first_child(t_ast_node *node, const t_pipe left)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_set_context("fork: %s", strerror(errno));
	else if (pid == 0)
	{
		close(left.read);
		pipe_replace_stdout(left.write);
		exec_command(node->command);
	}
	return (pid);
}

static pid_t	exec_last_child(t_ast_node *node, const t_pipe right)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_set_context("fork: %s", strerror(errno));
	else if (pid == 0)
	{
		pipe_replace_stdin(right.read);
		exec_command(node);
	}
	return (pid);
}

static pid_t	exec_child(t_ast_node *node, const t_pipe left,
		const t_pipe right)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_set_context("fork: %s", strerror(errno));
	else if (pid == 0)
	{
		close(right.read);
		pipe_replace_stdout(right.write);
		pipe_replace_stdin(left.read);
		exec_node(node);
	}
	return (pid);
}

static int	pipe_init(t_pipe *pype)
{
	int	p[2];

	if (pipe(p) == -1)
	{
		error_set_context("pipe: %s", strerror(errno));
		return (-1);
	}
	pype->read = p[0];
	pype->write = p[1];
	return (0);
}

int exec_node_pipe(t_ast_node *node, t_array pids)
{
	t_array		stack;
	t_ast_node	*child;
	t_pipe 		left;
	t_pipe 		right;
	pid_t		pid;

	stack = gather_piped_children(node);

	// get the first child from the stack
	array_pop(&stack, &child);

	// first piped child
	if (pipe_init(&left) == -1)
		return (-1);
	if ((pid = exec_first_child(node, left)) == -1)
		return (-1);
	array_push(&pids, &pid);
	close(left.write);

	// begin multiple children
	while (stack.len > 1)
	{
		array_pop(&stack, &child);
		if (pipe_init(&right) == -1)
			return (-1);
		if ((pid = exec_child(child, left, right)) == -1)
			return (-1);

		array_push(&pids, &pid);
		close(left.read);
		close(right.write);
		left.read = right.read;
	}

	// last child
	if ((pid = exec_last_child(node, left)) == -1)
		return (-1);
	array_push(&pids, &pid);
	close(left.read); // read end first pipe
	return (pid);
}