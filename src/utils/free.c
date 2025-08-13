#include "minishell.h"


void	free_cmd_blk(t_command_block *cmd)
{
	t_command_block *tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->command)
			free(cmd->command);
		if (cmd->args)
			free_arr(cmd->args);
		if (cmd->fd)
			free(cmd->fd);
		if(cmd->wrong_cmd)
			free(cmd->wrong_cmd);
		free(cmd);
		cmd = tmp;
	}
	// cmd = NULL; // bu satırın anlamı nedir? cmd dışarıda da kullanılıyor
	cmd = NULL; 
}

/* void	free_env(t_env **env_list)
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
} */

void	free_lexer(t_lexer_list **lex)
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
	*lex = NULL;
	free(lex);
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

void free_expander(t_expander *exp)
{
	if (exp)
	{
		if (exp->env_key)
			free(exp->env_key);
		if (exp->env_val)
			free(exp->env_val);
		free(exp);
	}
	exp = NULL;
}

void free_env(t_env **env_list)
{
	t_env	*tmp;
	t_env	*cur;

	if (!env_list || !*env_list)
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
	*env_list = NULL; // dışarıdaki pointer'ı da sıfırla
	free(env_list);
}

void	free_all(t_init	*init)
{
	if ((init->lxr_lst ))
		free_lexer(init->lxr_lst);
	if (init->cmd_blk && !init->heredoc)
	{
		free_cmd_blk(init->cmd_blk);
	}
	if ((init->jnd_lxr_lst && *(init->jnd_lxr_lst))|| init->exec) //bunu da ayır
		free_joined_exec(init->jnd_lxr_lst, init->exec);
	if (init->mng_hrdcs && init->mng_hrdcs->f_flag)
		free_mng(init->mng_hrdcs );
	if(init->expnd && init->exit_flag)
		free_expander(init->expnd);
	if (init->env && *init->env && init->exit_flag)
		free_env(init->env);
	free(init);
	init = NULL;
}

