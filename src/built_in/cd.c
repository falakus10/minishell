#include "minishell.h"

void	add_deleted(t_env *env, char *line, char* value)
{
	t_env	*tmp;
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node || !value || !line)
	{
		free(node);
		free(value);
		free(line);
		ft_error();
	}
	tmp = env;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = node;
	node->line = line;
	node->value = value;
	node->next = NULL;
}

void	update_oldpwd(t_env *env, char *old_pwd)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp("OLDPWD=", tmp->line, 7) == 0)
		{
			free(tmp->value);
			free(tmp->line);
			tmp->value = ft_strdup(old_pwd);
			tmp->line = ft_strjoin("OLDPWD=", old_pwd);
			return;
		}
		tmp = tmp->next;
	}
	add_deleted(env, ft_strjoin("OLDPWD=", old_pwd), ft_strdup(old_pwd));
}

void	update_pwd(t_env *env, char *old_pwd)
{
	t_env	*tmp;
	char	*current_pwd;
	char	cwd[256];

	current_pwd = ft_strdup(getcwd(cwd, 256));
	tmp = env;
	while (tmp)
	{
		if (ft_strncmp("PWD=", tmp->line, 4) == 0)
		{
			free(tmp->value);
			free(tmp->line);
			tmp->value = current_pwd;
			tmp->line = ft_strjoin("PWD=", current_pwd);
			break;
		}
		tmp = tmp->next;
	}
	if (!tmp)
		add_deleted(env, ft_strjoin("PWD=", current_pwd), current_pwd);
	if (old_pwd)
	{
		update_oldpwd(env, old_pwd);
	}
}

int	go_home(t_env *env, char *old_pwd)
{
	t_env	*tmp;
	char 	*home_path;

	home_path = NULL; 
	tmp = env;
	while (tmp)
	{
		if(ft_strncmp("HOME=", tmp->line, 5) == 0)
			home_path = ft_strdup(tmp->value);
		tmp = tmp->next;
	}
	if (home_path == NULL)
	{
		write(2, "cd: HOME not set\n", 16);
		return (1);
	}
	if (chdir(home_path) != 0)
	{
		perror("cd");
		free(home_path);
		return (1);
	}
	update_pwd(env, old_pwd);
	free(home_path);
	return (0);
}

int	ft_cd(t_command_block *cmd, t_env *env)
{
	char	cwd[256];
	char	*old_pwd;
	int		status;

	status = 1;
	old_pwd = ft_strdup(getcwd(cwd, 256));
	if (!cmd->args[1])
		status = go_home(env, old_pwd);
	else if (!cmd->args[2])
	{
		if (chdir(cmd->args[1]) != 0)
			perror("cd");
		else
		{
			update_pwd(env, old_pwd);
			status = 0;
		}
	}
	else
		write(2, "cd: too many arguments\n", 23);
	free(old_pwd);
	return (status);
}

