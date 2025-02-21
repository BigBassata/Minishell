/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:32:08 by licohen           #+#    #+#             */
/*   Updated: 2025/02/06 16:59:44 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_PARSING_H
# define MINISHELL_PARSING_H

# include "minishell.h"
# include "minishell_struct.h"

// line_validation_1.c
int			is_null_line(char *line);
int			is_void_case(char *line);
int			is_ignored_case(char *line);
int			is_syntax_error(char *line);
char		*filtered_line(char *line);

// line_validation_2.c
int			unclosed_quotes_case(char *line);
int			not_handled_case(char *line, char *message);
int			bad_redirection_syntax(char *line);
int			bad_pipe_syntax(char *line);

// line_validation_utils.c
void		set_current_quote(char c, char *current_quote);

// tokenization.c
t_token		*tokenize_line(char *line);
t_token		*handle_tokenization(char *line);
int			tokenize_meta_chr(char **line, t_token **token_list);
int			tokenize_word(char **line, t_token **token_list);

// tokenization_utils.c
t_token		*new_token(char *value, t_token_type type);
void		add_last_token(t_token **first_token, t_token *new_token);
void		free_token_list(t_token *first_token);

// parsing_1.c
t_command	*parse_token_list(t_token *token_list, t_environment_var *env);
t_command	*handle_parsing(t_token *token_list, t_environment_var *env);
t_command	*init_cmd_list(t_token *token_list);
int			parse_infile_and_heredoc(t_command *curr_cmd, t_token *token_list,
				int *file_nb, t_environment_var *env);
int			parse_infile(t_command *curr_cmd, t_token *token);

// parsing_2.c
int			parse_heredoc(t_command *curr_cmd, t_token *token, int *file_nb,
				t_environment_var *env);
int			parse_outfile(t_command *curr_cmd, t_token *tok);
int			args_arr_counter(t_token *token);
char		**init_args_arr(t_token *token);
int			parse_cmd_args(t_command *curr_cmd, t_token *token);

// parsing_3.c
void		moove_to_next_pipe(t_token **token);

// parsing_utils.c
t_command	*new_cmd(void);
void		add_last_cmd(t_command **first_cmd, t_command *new_cmd);
void		free_cmd_args(char **args);
void		free_cmd_list(t_command *first_cmd);

// expand_args_1.c
t_command	*expand_args(t_command *cmd, t_environment_var *env);
int			handle_expand_args(t_command *cmd, t_environment_var *env);
int			is_expandable_var(char *arg);
int			expand_vars_in_arg(t_command *cmd, t_environment_var *env, int n);
int			vars_values_lens(char *arg, t_environment_var *env, int *vars_len,
				int *values_len);

// expand_args_2.c
int			is_env_var_format(char *arg);
int			get_env_var_len(char *arg);
int			get_env_var_value_len(char *env_var, int var_len,
				t_environment_var *env);
char		*get_env_var(char *arg);
int			cpy_and_expand_arg(char **dest, char *src, t_environment_var *env);

// expand_args_3.c
int			last_status_len(int exit_code);
void		cpy_expanded_var(char *dest, int *index, char *env_var,
				t_environment_var *env);
void		expand_last_status(char *dest, int *index, int exit_code,
				char *base_10);
int			remove_surrounding_quotes(t_command *cmd);
int			remove_quotes_in_arg(t_command *cmd, int n);

// expand_args_4.c
int			resize_and_copy_expandable_dest(char **dest, int *dest_size);
int			resize_and_copy_dest(char **dest);
void		toggle_in_single_quotes(char c, int *in_squote);
int			are_quotes_in_arg(char *arg);
int			cpy_arg_without_quotes(char **dst, char *src);

// expand_args_5.c
int			handle_cpy_expanded_var(char **dest, char **src,
				t_environment_var *env, int *len);

// heredoc.c
char		*handle_here_doc(int *file_nb, char *delimiter,
				t_environment_var *env);
int			create_here_doc_file(char *file_path, char *delimiter,
				t_environment_var *env);
int			handle_heredoc_expand(char **line, t_environment_var *env);
int			expand_heredoc_var(char **line, t_environment_var *env);
int			refine_delimiter(char **delimiter);

// heredoc_utils.c
int			cpy_and_expand_heredoc_arg(char *dest, char *src, int size,
				t_environment_var *env);
int			str_count_char(char *s, char c);
void		cpy_refined_delimiter(char *delimiter, char *new_delimiter,
				char quote);
int			check_heredoc_line(char *line, char *delimiter);

#endif