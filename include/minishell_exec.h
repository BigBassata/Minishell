/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:32:04 by licohen           #+#    #+#             */
/*   Updated: 2025/02/06 18:45:45 by licohen          ###   ########.fr       */
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

#define PATH_SEPARATOR ':'
#define DIR_SEPARATOR "/"

#define PATH_MAX 4096 //Max length of a path

//Built-in functions
int                 ft_echo(char **array, int fd_out);
int                 ft_exit(t_command *cmd, int fd_out);
int                 ft_pwd(int fd_out);
int	                ft_env(t_environment_var *env, int fd_out);
int                 ft_cd(char **args, t_environment_var *env);
int                 ft_unset(t_environment_var **env, char **args);
int	                ft_export(char *arg_1, t_environment_var *env, int fd_out);
void                execute_builtin(t_command *cmd, t_environment_var **env);
int	                is_builtin(char *cmd);

//utils
int                 nbr_of_args(char **array);
int                 is_valid_env_name(const char *name);
int                 is_readonly_var(const char *name);
t_environment_var   *create_new_env_node(const char *name, const char *value);
int	                update_or_create_oldpwd(t_environment_var *env);
int	                update_or_create_pwd(t_environment_var *env);
int	                cd_to_oldpwd(t_environment_var *env);
int	                cd_with_tild(char **args, t_environment_var *env);
void                free_array(char **array);

//memory management 
void                cleanup_ptr(void *ptr);
void                cleanup_env_node(t_environment_var *node);
void                cleanup_environment(t_environment_var *environment);
void                cleanup_command(t_command *cmd);
void                cleanup_all(t_environment_var *env, t_command *cmd, int exit_code);


//init
t_environment_var   *initialize_shell(char **envp);
t_environment_var   *init_environment(char **envp);
t_environment_var   *create_environment_node(char *envp);

//exec
char                *get_env_value(t_environment_var *environment, const char *key);
char                *find_command_path(const char *command, t_environment_var *environment);
int                 execute_command(t_command *cmd, t_environment_var *environment);
t_cmd_type          get_command_type(t_command *cmd, t_environment_var *environment);
enum e_state        check_command_type(const char *cmd);

//env
int                 create_env_var(t_environment_var **environment, const char *name, const char *value);
int                 custom_setenv(t_environment_var **environment, const char *name, const char *value);
char                *get_env_value(t_environment_var *environment, const char *key);
char                **convert_env_to_array(t_environment_var *environment);

//redir
int                 setup_redirections(t_command *cmd);
int                 setup_output_fd(t_command *cmd, int flags, int *prev_fds);
int                 check_output_permissions(t_command *cmd, int *prev_fds);
int                 check_input_permissions(t_command *cmd, int *prev_fds);
int                 setup_fd_backup(int *prev_in_fd, int *prev_out_fd);
void                cleanup_fds(int prev_in_fd, int prev_out_fd);

//process 
int                 wait_for_child(pid_t pid);


#endif