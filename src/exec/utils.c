/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:45:20 by licohen           #+#    #+#             */
/*   Updated: 2025/02/11 12:17:50 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

// void cleanup_ptr(void *ptr)
// {
//     if (ptr)
//     {
//         free(ptr);
//         ptr = NULL;
//     }
// }

// void cleanup_env_node(t_environment_var *node)
// {
//     if (node)
//     {
//         if (node->key)
//             free(node->key);
//         if (node->value)
//             free(node->value);
//         free(node);
//     }
// }

// void cleanup_environment(t_environment_var *environment)
// {
//     t_environment_var *current;
//     t_environment_var *next;

//     current = environment;
//     while (current)
//     {
//         next = current->next;
//         cleanup_env_node(current);
//         current = next;
//     }
// }

// void cleanup_command(t_command *cmd)
// {
//     if (!cmd)
//         return;
//     if (cmd->args)
//         free_array(cmd->args);
//     if (cmd->input_path)
//         free(cmd->input_path);
//     if (cmd->output_path)
//         free(cmd->output_path);
//     if (cmd->heredoc_delim)
//         free(cmd->heredoc_delim);
//     free(cmd);
// }

// void cleanup_all(t_environment_var *env, t_command *cmd, int exit_code)
// {
//     if (env)
//         cleanup_environment(env);
//     while (cmd)
//     {
//         t_command *next = cmd->next;
//         cleanup_command(cmd);
//         cmd = next;
//     }
//     if (exit_code != -1)
//         exit(exit_code);
// }

// void    free_array(char **array)
// {
//     int i;

//     if (!array)
//         return;
//     i = 0;
//     while (array[i])
//     {
//         free(array[i]);
//         array[i] = NULL;
//         i++;
//     }
//     free(array);
// }


void cleanup_ptr(void *ptr)
{
    if (ptr)
    {
        free(ptr);
        ptr = NULL;
    }
}

void cleanup_env_node(t_environment_var *node)
{
    if (node)
    {
        if (node->key)
            free(node->key);
        if (node->value)
            free(node->value);
        free(node);
    }
}

void cleanup_environment(t_environment_var *environment)
{
    t_environment_var *current;
    t_environment_var *next;

    current = environment;
    while (current)
    {
        next = current->next;
        cleanup_env_node(current);
        current = next;
    }
}

void cleanup_command(t_command *cmd)
{
    if (!cmd)
        return;
    if (cmd->args)
        free_array(cmd->args);
    if (cmd->input_path)
        free(cmd->input_path);
    if (cmd->output_path)
        free(cmd->output_path);
    if (cmd->heredoc_delim)
        free(cmd->heredoc_delim);
    free(cmd);
}

void cleanup_all(t_environment_var *env, t_command *cmd, int exit_code)
{
    if (env)
        cleanup_environment(env);
    while (cmd)
    {
        t_command *next = cmd->next;
        cleanup_command(cmd);
        cmd = next;
    }
    if (exit_code != -1)
        exit(exit_code);
}

void    free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        array[i] = NULL;
        i++;
    }
    free(array);
}
