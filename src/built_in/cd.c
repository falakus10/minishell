#include "minishell.h"

int	add_deleted(t_env *env, char *line, char* value)
{
	t_env	*tmp;
	t_env	*node;
	
	if(!env)
	{
		write(2, "bash: cd: ", 11);
		write(2, value, ft_strlen(value));
		write(2, ": No such file or directory\n", 28);
		free(line);
		free(value);
		return(1);
	}

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
	return (0);
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

int	update_pwd(t_env *env, char *old_pwd)
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
	{
		if (add_deleted(env, ft_strjoin("PWD=", current_pwd), current_pwd))
			return (2);
	}

	if (old_pwd)
	{
		update_oldpwd(env, old_pwd);
	}
	return (0);
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
		write(2, "bash: cd: HOME not set\n", 23);
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
	if (!cmd->args[1] || (cmd->args[1][0] == '~' && cmd->args[1][0] == '\0'))
		status = go_home(env, old_pwd);
	else if (!cmd->args[2])
	{
		if (chdir(cmd->args[1]) != 0)
			perror("cd");
		else
		{
				
			if(update_pwd(env, old_pwd))
				status = 2;
			else
				status = 0;
		}
	}
	else
		write(2, "cd: too many arguments\n", 23);
	free(old_pwd);
	return (status);
}

