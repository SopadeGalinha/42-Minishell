/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 02:54:11 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/14 02:54:11 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*__________________________________HEADERS___________________________________*/

# include <math.h>
# include <errno.h>
# include <signal.h>
# include <string.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "./libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>

//---------------------------------END HEADERS--------------------------------//

/*__________________________________STRUCTS___________________________________*/

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

/*__________________________________MACROS____________________________________*/

// COLORS
# define ESC_RESET			"\001\033[0m\002"
# define ESC_BOLD_RED		"\001\033[1;31m\002"
# define ESC_BOLD_GREEN		"\001\033[1;32m\002"
# define ESC_BOLD_ORANGE	"\001\033[1;33m\002"
# define ESC_BOLD_PURPLE	"\001\033[1;35m\002"
# define ESC_BOLD_WHITE		"\001\033[1;37m\002"
# define ESC_BOLD_GREY		"\001\033[1;39m\002" 
# define ESC_BOLD_YELLOW	"\001\033[1;93m\002"
# define ESC_BOLD_BLUE		"\001\033[1;94m\002"
# define ESC_BOLD_MAGENTA	"\001\033[1;95m\002"
# define ESC_BOLD_CYAN		"\001\033[1;96m\002"
# define ESC_BOLD_BLACK		"\001\033[1;98m\002"

// BEAUTIFUL PROMPT
# define M "\001\033[1;92m\002m\001\033[0m\002"
# define I "\001\033[1;93m\002i\001\033[0m\002"
# define N "\001\033[1;31m\002n\001\033[0m\002"
# define I "\001\033[1;93m\002i\001\033[0m\002"
# define S "\001\033[1;94m\002s\001\033[0m\002"
# define H "\001\033[1;96m\002h\001\033[0m\002"
# define E "\001\033[1;31m\002e\001\033[0m\002"
# define L "\001\033[1;35m\002l\001\033[0m\002"
# define Z "\001\033[1;98m\002$ \001\033[0m\002"

// ERRORS

# define ERROR_INVALID_ARGS	"\001\033[1;31m\002 Invalid arguments\n"
# define TRY_AGAIN	"\001\033[1;93m\002 Try: ./minishell\n\001\033[0m\002"
//---------------------------------END MACROS---------------------------------//

#endif