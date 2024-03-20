
#include "../include/minishell.h"

int	create_prompt(t_ms *ms)
{
	size_t	size;
	
	if (ms->prompt)
		free(ms->prompt);
	size = 9 + ft_strlen("ninjaSHELL:~$ ") + 6;
	ms->prompt = (char *)ft_calloc(size + 2, sizeof(char));
	if (ms->prompt == NULL)
		return (1);
	ft_strlcpy(ms->prompt, "\033[0;92m", size);
	ft_strlcat(ms->prompt, "ninjaSHELL:~$ ", size);
	ft_strlcat(ms->prompt, "\033[0m", size);
	return (0);
}
