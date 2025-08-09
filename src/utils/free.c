#include "minishell.h"


void	free_cmd_blk(t_command_block *cmd)
{
	t_command_block *tmp;

	while (cmd)
	{
        //printf("command :%s\n",cmd->command);
        tmp = cmd->next;
		if (cmd->command)
			free(cmd->command);
		free_arr(cmd->args);
		if (cmd->fd)
			free(cmd->fd);
		if(cmd->wrong_cmd)
			free(cmd->wrong_cmd);
		free(cmd);
		cmd = tmp;
	}
	cmd = NULL;
}

void	free_env(t_env **env_list)
{
	t_env	*tmp;
	t_env	*cur;

	if (!env_list)
		return;
	cur = *env_list;
	while (cur)
	{
		tmp = cur->next;
		if (cur->line)
			free(cur->line);
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = tmp;
	}
	free(env_list);
	env_list = NULL;
}

void	free_lexer_expander(t_lexer_list **lex, t_expander *exp)
{
	t_lexer_list	*tmp;

	while (*lex)
	{
		tmp = (*lex)->next;
		if ((*lex)->token)
			free((*lex)->token);
		free(*lex);
		*lex = tmp;
	}
	*lex = NULL; // dışarıdaki pointer'ı da sıfırla
	free(lex);
	if (exp)
	{
		if (exp->env_key)
			free(exp->env_key);
		if (exp->env_val)
			free(exp->env_val);
		free(exp);
	}
}

void	free_joined_exec(t_joined_lexer_list **jll, t_executor *exe)
{
	t_joined_lexer_list	*tmp;

	while (*jll)
	{
		tmp = (*jll)->next;
		if ((*jll)->token)
			free((*jll)->token);
		(*jll)->token = NULL;
		free(*jll);
		*jll = tmp;
	}
	*jll = NULL; // dışarıdaki pointer'ı da sıfırla
	free(jll);
	if (exe)
	{
		if (exe->fd)
		{
			free(exe->fd);
			exe->fd = NULL;
		}
		free(exe);
	}
	exe = NULL;
}

void free_mng(t_mng_heredocs *mng)
{
	if (mng)
	{
		if (mng->heredoc_delims)
			free_arr(mng->heredoc_delims);
		if (mng->heredoc_fds)
			free(mng->heredoc_fds);
		if (mng->heredoc_flags)
			free(mng->heredoc_flags);
		if (mng->heredoc_nums)
			free(mng->heredoc_nums);
		free(mng);
	}
	mng = NULL;
}

void	free_all(t_init	*init, t_env **env_list)
{
    if ((init->lxr_lst && *(init->lxr_lst)) || init->expnd)
       free_lexer_expander(init->lxr_lst, init->expnd);
	if (init->cmd_blk)
		free_cmd_blk(init->cmd_blk);
	if (env_list && *env_list)
		free_env((env_list));
	if ((init->jnd_lxr_lst && *(init->jnd_lxr_lst))|| init->exec)
		free_joined_exec(init->jnd_lxr_lst, init->exec);
	if (init->mng_hrdcs)
		free_mng(init->mng_hrdcs);
	free(init);
	init = NULL;
}

