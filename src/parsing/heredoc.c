/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	refine_delimiter(char **delimiter)
{
	char	*new_delimiter;
	char	quote;
	int		delimiter_len;
	int		quote_nb;

	quote = 0;
	if (!delimiter || !*delimiter)
		return (TRUE);
	set_current_quote((*delimiter)[0], &quote);
	if (quote == 0)
		return (TRUE);
	delimiter_len = ft_strlen(*delimiter);
	quote_nb = str_count_char(*delimiter, quote);
	new_delimiter = malloc((delimiter_len - quote_nb + 1) * sizeof(char));
	if (!new_delimiter)
		return (ERROR);
	cpy_refined_delimiter(*delimiter, new_delimiter, quote);
	free(*delimiter);
	*delimiter = new_delimiter;
	return (TRUE);
}

int	expand_heredoc_var(char **line, t_environment_var *env)
{
	int		line_len;
	int		vars_len;
	int		values_len;
	char	*new_line;

	vars_len = 0;
	values_len = 0;
	line_len = ft_strlen(*line);
	line_len++;
	if (vars_values_lens(*line, env, &vars_len, &values_len) == ERROR)
		return (print_error_message("HD vars_values_lens() error"), ERROR);
	new_line = malloc((line_len + values_len - vars_len) * sizeof(char));
	if (!new_line)
		return (print_error_message("expand heredoc var error"), ERROR);
	if (cpy_and_expand_heredoc_arg(new_line, *line,
			line_len + values_len - vars_len, env) == ERROR)
	{
		free(new_line);
		return (print_error_message("HD cpy_and_update_arg() error"), ERROR);
	}
	free(*line);
	*line = new_line;
	return (TRUE);
}

int	handle_heredoc_expand(char **line, t_environment_var *env)
{
	if (!is_expandable_var(*line))
		return (TRUE);
	if (expand_heredoc_var(line, env) == ERROR)
		return (print_error_message("error expand here doc var"), ERROR);
	return (TRUE);
}

int	create_here_doc_file(char *file_path, char *delimiter,
	t_environment_var *env)
{
	int		fd;
	char	*line;

	if (access(TMP_DIR, F_OK | W_OK) == -1)
		return (print_error_message("error access create here doc"), ERROR);
	fd = open(file_path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (print_error_message("error fd create here doc file"), ERROR);
	while (1)
	{
		line = readline("> ");
		if (check_heredoc_line(line, delimiter))
		{
			if (!line)
				handle_heredoc_ctr_d(delimiter, env);
			free(line);
			break ;
		}
		if (handle_heredoc_expand(&line, env) == ERROR)
			return (close(fd), free(line), ERROR);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (close(fd), TRUE);
}

char	*handle_here_doc(int *file_nb, char *delimiter, t_environment_var *env)
{
	char	*file_name;
	char	*file_path;
	char	*str_file_nb;
	int		tmp_dir_len;

	file_name = NULL;
	str_file_nb = ft_itoa(*file_nb);
	if (!str_file_nb)
		return (NULL);
	file_path = ft_strjoin(STD_HEREDOC_FILE_PATH, str_file_nb);
	free(str_file_nb);
	(*file_nb)++;
	if (!file_path)
		return (NULL);
	if (file_path && !*file_path)
		return (free(file_path), NULL);
	if (create_here_doc_file(file_path, delimiter, env) == ERROR)
		return (free(file_path), NULL);
	tmp_dir_len = ft_strlen(TMP_DIR);
	file_name = ft_strdup(&file_path[tmp_dir_len]);
	free(file_path);
	if (!file_name)
		return (NULL);
	return (file_name);
}
