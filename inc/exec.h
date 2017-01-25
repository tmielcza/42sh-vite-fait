#ifndef EXEC_H
# define EXEC_H

# include <unistd.h>
# include "ast.h"
# include "errors.h"

enum e_exec_errors
{
	EXEC_NO_ERROR = NO_ERROR,
	ERR_OPEN, 
	ERR_DUP2, 
	ERR_CLOSE, 
	ERR_FCNTL, 
	ERR_BADFD, 
	ERR_GRLIMIT, 
	ERR_SRLIMIT, 
	ERR_FDLIMIT, 
};

typedef int (t_tree_walker)(const t_ast_node *);
typedef struct s_pipe	t_pipe;
struct s_pipe
{
	int	read;
	int	write;
};

// Tree Walker
int	exec_node_list(const t_ast_node *node);
int	exec_node_andor(const t_ast_node *node);
int	exec_node_pipe(const t_ast_node *node);
int	exec_node_subshell(const t_ast_node *node);
int	exec_node_simple_command(const t_ast_node *node);

// Execution
int	exec_builtin(const t_command command);
int	exec_binary(const t_command command);

/*
** Pipe
*/
int	pipe_replace_stdout(int write_end);
int	pipe_replace_stdin(int read_end);
int pipe_replace_stdfd(int read_end, int write_end);

#endif