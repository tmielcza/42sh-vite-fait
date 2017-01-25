#include <unistd.h>
#include "exec.h"
#include "errors.h"

/* int	pipe_connect_write_end(int write_end) */
int	pipe_replace_stdout(int write_end)
{
	if (dup2(write_end, STDOUT_FILENO) == -1)
		return (ERR_DUP2);
	return (NO_ERROR);
}

/* int	pipe_connect_read_end(int read_end) */
int	pipe_replace_stdin(int read_end)
{
	if (dup2(read_end, STDIN_FILENO) == -1)
		return (ERR_DUP2);
	return (NO_ERROR);
}

int pipe_replace_stdfd(int read_end, int write_end)
{
	if (dup2(write_end, STDOUT_FILENO) == -1)
		return (ERR_DUP2);
	if (dup2(read_end, STDIN_FILENO) == -1)
		return (ERR_DUP2);
	return (NO_ERROR);
}