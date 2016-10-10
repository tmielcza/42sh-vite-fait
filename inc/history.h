#ifndef HISTORY_H
# define HISTORY_H

# include <stdlib.h>
# include <stdbool.h>
# include "typedefs_42.h"
# include "structs_42.h"
# include "cbuffer_42.h"

struct		s_history
{
	t_cbuffer	cbuffer; // TODO t_buffer (need to be named t_string)
	size_t		last_id;
};

struct		s_result
{
	size_t	command_id;
	size_t	offset;
};

typedef struct s_history	t_history;
typedef struct s_result		t_result;

# define HIST_CONTAINS(h, i) (i <= h.last_id && i > h.last_id - h.cbuffer.len)
# define CMD_LEN (64)

int			history_init(size_t limit);

int			history_save_into_file(const char *path);
int			history_load_from_file(const char *path);

size_t		history_push(const char *command);

const char	*history_get(size_t id);

bool		history_find(t_result *ret, const char *pattern);
bool		history_find_from(t_result *ret, const char *patt, t_result from);
bool		history_find_start_with(t_result *res, const char *pattern);

#endif
