 #include "minishell.h"

void handle_ambiguous_redirect(t_command_block **tmp_blk, t_joined_lexer_list **tmp_list)
{
	if ((*tmp_list)->next->token[0] == '$')
	{
		write(2, "bash: ", 6);
		write(2, (*tmp_list)->next->token, ft_strlen((*tmp_list)->next->token));
		write(2, ": ambiguous redirect\n", 21);
		(*tmp_blk)->expnd->exit_value = 1;
		(*tmp_blk)->file_err = 1;
	}
}

void handle_fd_error(t_command_block **tmp_blk, t_joined_lexer_list **tmp_list)
{
	if (errno == EISDIR)
	{
		write(2, "bash: ", 6);
		write(2, (*tmp_list)->next->token, ft_strlen((*tmp_list)->next->token));
		write(2, ": Is a directory\n", 17);
	}
	else if (errno == ENOENT)
	{
		write(2, "bash: ", 6);
		write(2, (*tmp_list)->next->token, ft_strlen((*tmp_list)->next->token));
		write(2, ": no such file or directory\n", 28);
	}
	else
		perror("bash");
	(*tmp_blk)->last_fault = 1;
	(*tmp_blk)->expnd->exit_value = 1;
	(*tmp_blk)->file_err = 1;
}

void handle_input_redirection(t_command_block **tmp_blk, t_joined_lexer_list **tmp_list, char *file_pth, t_mng_heredocs *mng)
{
	int old_infd;

	old_infd = (*tmp_blk)->input_fd;
	(*tmp_blk)->input_fd = open(file_pth, O_RDONLY);
	if ((*tmp_blk)->input_fd == -1)
		handle_fd_error(tmp_blk, tmp_list);
	else
	{
		if (mng->heredoc_flags && mng->heredoc_flags[mng->index])
			close((*tmp_blk)->input_fd);
		else
			close_old_fd(old_infd);
	}
}

void handle_output_redirection(t_command_block **tmp_blk, t_joined_lexer_list **tmp_list, char *file_pth)
{
	int old_outfd;

	old_outfd = (*tmp_blk)->output_fd;
	(*tmp_blk)->output_fd = open(file_pth, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if ((*tmp_blk)->output_fd == -1)
		handle_fd_error(tmp_blk, tmp_list);
	else
		close_old_fd(old_outfd);
}

void handle_append_redirection(t_command_block **tmp_blk, t_joined_lexer_list **tmp_list, char *file_pth)
{
	int old_outfd;

	old_outfd = (*tmp_blk)->output_fd;
	(*tmp_blk)->output_fd = open(file_pth, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*tmp_blk)->output_fd == -1)
		handle_fd_error(tmp_blk, tmp_list);
	else
		close_old_fd(old_outfd);
} 
        
