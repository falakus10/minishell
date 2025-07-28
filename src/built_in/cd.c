#include "minishell.h"

void	update_oldpwd(t_env *env, char *old_pwd) //OLDPWD yok durumu incelenmeli mi
{
	t_env	*tmp;
	int		flag;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp("OLDPWD=", tmp->line, 7) == 0)
		{
			free(tmp->value);
			free(tmp->line);
			tmp->value = ft_strdup(old_pwd);
			tmp->line = ft_strjoin("OLDPWD=", old_pwd);	
			break;		
		}
		tmp = tmp->next;
	}
}

void	update_pwd(t_env *env, char *path)
{
	char	*old_pwd;
	t_env	*tmp;

	tmp = env;
	old_pwd = NULL;
	while (tmp)
	{
		if (ft_strncmp("PWD=", tmp->line, 4) == 0)
		{
			old_pwd = ft_strdup(tmp->value);
			free(tmp->value);
			free(tmp->line);
			tmp->value = ft_strdup(path);
			tmp->line = ft_strjoin("PWD=", path);
			break;			
		}
		tmp = tmp->next;
	}
	if(old_pwd)
	{
		update_oldpwd(env, old_pwd);
		free(old_pwd);
	}
}

int	go_home(t_env *env)
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
		perror("cd");
		return (1);
	}
	if (chdir(home_path) != 0)
	{
		perror("cd");
		free(home_path);
		return (1);
	}
	update_pwd(env, home_path);
	free(home_path);
	return (0);
}

int	ft_cd(t_command_block *cmd, t_env *env)
{
	char *path;
	
	if (cmd->args[1] == NULL)
	{
		if (go_home(env) == 0)
			return (0);
		return (1);
	}
	else
	{
		path = ft_strdup(cmd->args[1]);
		if (chdir(path) != 0)
		{
			perror("cd");
			free(path);
			return (1);
		}
		else
		{
			update_pwd(env, path);
			free(path);
			return (0);
		}
	}
	return (1);
}
