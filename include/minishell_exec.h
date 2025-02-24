/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:32:04 by licohen           #+#    #+#             */
/*   Updated: 2025/02/25 00:10:46 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_EXEC_H
# define MINISHELL_EXEC_H

# include "minishell.h"
# include "minishell_struct.h"

# define CD 0
# define ECHO 1
# define ENV 2
# define EXIT 3
# define EXPORT 4
# define PWD 5
# define UNSET 6

# define MAX_LONG "9223372036854775807"
# define MAX_LONG_PLUS_1 "9223372036854775808"

# define PATH_SEPARATOR ':'
# define DIR_SEPARATOR "/"

# define PATH_MAX 4096 //Max length of a path

//Built-in commands
int					ft_echo(char **array, int fd_out);
int					ft_exit(t_command *cmd, int fd_out);
int					ft_pwd(int fd_out);
int					ft_env(t_environment_var *env, int fd_out);
int					ft_cd(char **args, t_environment_var *env);
int					ft_unset(t_environment_var **env, char **args);
int					ft_export(char *arg_1, t_environment_var *env, int fd_out);
int					is_builtin(char *cmd);

//Command execution
int					execute_command(t_command *cmd,
						t_environment_var *environment);
void				execute_builtin(t_command *cmd,
						t_environment_var **env);
t_cmd_type			get_command_type(t_command *cmd,
						t_environment_var *environment);
enum e_state		check_command_type(const char *cmd);
char				*find_command_path(const char *command,
						t_environment_var *environment);
int					wait_for_child(pid_t pid);
int					handle_external_parent(pid_t pid,
						t_environment_var *environment);

//env management
t_environment_var	*initialize_shell(char **envp);
t_environment_var	*init_environment(char **envp);
t_environment_var	*create_environment_node(char *envp);
int 				add_default_env_var(t_environment_var **last_node,
					char *key, char *value);
int 				add_minimal_env(t_environment_var *environment);
int 				process_envp(t_environment_var *env, char **envp, int *has_env);

char				*get_env_value(t_environment_var *environment,
						const char *key);
int					create_env_var(t_environment_var **environment,
						const char *name, const char *value);
int					custom_setenv(t_environment_var **environment,
						const char *name, const char *value);
char				**convert_env_to_array(t_environment_var *environment);
void				del_environment(t_environment_var *envp);
int					is_valid_env_name(const char *name);
int					is_readonly_var(const char *name);
t_environment_var	*create_new_env_node(const char *name, const char *value);

//Pipeline Handling
int					execute_pipeline(t_command *cmd,
						t_environment_var *environment);
int					initialize_pipeline(t_pipeline_info *info, t_command *cmd);
int					execute_piped_command(t_command *cmd,
						t_environment_var *env, t_pipeline_info *info);
int					execute_builtin_parent(t_command *cmd,
						t_environment_var *env);
int					check_builtin_execution(t_command *cmd);
int					wait_for_pipeline(t_pipeline_info *info);
void				close_pipe_fds(int *pipe_fds);
void				cleanup_pipeline(t_pipeline_info *info);
void 				close_saved_fds(int stdin_fd, int stdout_fd);
int 				restore_fds2(int stdin_fd, int stdout_fd);

//Signal handling
void				setup_child_signals(void);
void				setup_signals_interactive_mode(
						t_environment_var *environment);

//Redir management
int					setup_redirections(t_command *cmd);
int					build_heredoc_file_path(char *heredoc_file_path,
						char *input_path);
int					setup_output_fd(t_command *cmd, int flags, int *prev_fds);
int					check_output_permissions(t_command *cmd, int *prev_fds);
int					check_input_permissions(t_command *cmd, int *prev_fds);
int					setup_fd_backup(int *prev_in_fd, int *prev_out_fd);
void				cleanup_fds(int prev_in_fd, int prev_out_fd);
int					handle_dup2_error(int fd, char *path);
int					handle_dup2_input_error(int fd, char *path);

//Memory management
void				cleanup_ptr(void *ptr);
void				cleanup_env_node(t_environment_var *node);
void				cleanup_environment(t_environment_var *environment);
void				cleanup_command(t_command *cmd);
void				cleanup_all(t_environment_var *env, t_command *cmd,
						int exit_code);
void				free_array(char **array);

//CD command utils
int					update_or_create_oldpwd(t_environment_var *env);
int					update_or_create_pwd(t_environment_var *env);
int					cd_to_oldpwd(t_environment_var *env);
int					cd_with_tild(char **args, t_environment_var *env);
int					create_env_var_in_cd(t_environment_var **environment,
						const char *name, const char *value);

//other utils
int					nbr_of_args(char **array);
void				init_next_pipe(int *next_pipe);
int					is_running_programm(char *cmd, char *prog_name);

#endif