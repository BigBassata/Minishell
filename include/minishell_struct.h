/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_struct.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:22:55 by licohen           #+#    #+#             */
/*   Updated: 2025/02/11 12:11:17 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_STRUCT_H
# define MINISHELL_STRUCT_H

// STRUCTURES ///////////////////////////////////////////////////////////////typedef enum e_error constant

# define TMP_DIR "./src/tmp/"
# define HEREDOC_FILE_PREFIX "heredoc_file_"
# define STD_HEREDOC_FILE_PATH TMP_DIR HEREDOC_FILE_PREFIX

# define ARG_MAX_SIZE 4096

typedef enum e_error
{
	SYNTAX_ERROR_REDIRECTION,
	SYNTAX_ERROR_PIPE,
	SYNTAX_ERROR_UNCLOSED_QUOTES,
	COMMAND_NOT_FOUND,
	NOT_A_VALID_IDENTIFIER,
	NO_SUCH_FILE_OR_DIRECTORY,
	NUMERIC_ARGUMENT_REQUIRED,
	INVALID_OPTION,
	TOO_MANY_ARGUMENTS,
	PERMISSION_DENIED,
    FILE_NOT_FOUND,
	CD_NO_SUCH_FILE_OR_DIRECTORY,
	CD_NOT_DIRECTORY,
	CD_NEED_RELATIVE_OR_ABSOLUTE_PATH,
	CD_ERR_RETRIEVING_CURR_DIRECTORY,
	CD_TOO_MANY_LVL_OF_SYMBOLIC_LINKS,
	OLDPWD_NOT_SET,
	HOME_NOT_SET,
    READONLY_VARIABLE,
	ENVIRONMENT_INIT_FAILED,
    MEMORY_ALLOCATION_FAILED,
	DUP2_ERROR,
	PIPE_ERROR, 
	FORK_ERROR, 
	PIPE_CREATION_ERROR,
	WAITPID_ERROR,
	HEREDOC_ERROR, 
}	t_error;

enum e_state
{
	ERROR = -1,
	FALSE,
	TRUE
};

typedef enum e_token_type
{
	TYPE_REDIR_IN,
	TYPE_REDIR_OUT,
	TYPE_REDIR_APPEND,
	TYPE_HEREDOC,
	TYPE_PIPE,
	TYPE_WORD
}	t_token_type;

typedef enum e_cmd_type {
    CMD_BUILTIN,
    CMD_EXTERNAL,
    CMD_NOT_FOUND,
    CMD_ERROR
} 	t_cmd_type;

typedef struct s_token_node
{
	t_token_type			type;
	char					*value;
	struct s_token_node		*next;
}	t_token;

typedef struct s_environment_var
{
    char 	*key;
    char 	*value;
    int 	ctr_d_in_heredoc;
    int 	last_exit_code;
    struct 	s_environment_var *next;
} t_environment_var;

typedef struct s_pipeline_info
{
    pid_t	*process_ids;
    int		pipe_fds[2];
    int		total_commands;
    int		current_index;
} t_pipeline_info;

typedef struct s_command
{
	char	**args;
	char	*input_path;
	char	*output_path;
	int		input_fd;
	int		output_fd;
	int		is_heredoc;
	int		is_append_mode;
	int		exit_code;
	char	*heredoc_delim;
	struct 	s_command *prev;
	struct 	s_command *next;
} t_command;

#endif