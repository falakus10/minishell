#include "minishell.h"

int find_index(const char *haystack, const char *needle, t_expander *expander)
{
	char *ptr;
	//int i;

	//i = 0;
	/* while (haystack[i]!= '\0')
	{
		if(haystack[i] == '$')
		{
			if(haystack[i+1] == '$')
			{

			}
		}

		i++
	} */
	ptr = ft_strnstr(haystack + expander->dollar_index , needle, ft_strlen(haystack));
	if (ptr == NULL)
		return (-1);
	return (ptr - haystack);  // pointer farkı → index
}

char	*ft_strjoin_free(char *token, t_expander *expander)
{
	int		j;
	char	*new_token;

	j = 0;
	expander->key_len = ft_strlen(expander->env_key);
	expander->val_len = ft_strlen(expander->env_val);
	if (!token || !expander->env_val)
		return (NULL);
	expander->new_len = ft_strlen(token) + (expander->val_len - expander->key_len);
	new_token = malloc(expander->new_len + 1);
	if (!new_token)
		return (NULL);
	expander->index = find_index(token, expander->env_key, expander);
	if (expander->index == -1)
		return (NULL);
	if (token[expander->index - 1] == '$')
	{
		ft_memcpy(new_token, token, (expander->index - 1));
		ft_memcpy(new_token + (expander->index - 1), expander->env_val, expander->val_len);
		j = expander->index + expander->key_len;
		expander->index += expander->val_len - 1;
		ft_memcpy(new_token + expander->index, token + j, ft_strlen(token) - j);
	}
	expander->i = 0;
	return (new_token);
}

char *env_value(char **env, const char *key)
{
	size_t	key_len;
	int		i;
	char	*env_value;

	if (env == NULL || key == NULL)
		return (0);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			env_value = ft_substr(env[i], key_len + 1, ft_strlen(env[i]) - (key_len + 1));                         
			return (env_value);
		}
		i++;
	}
	return (NULL);                                
}

int is_valid_ch(char *token, int i) //$ işaretinden sonraki karakter geçerli bir karakter mi kontrol eder
{
	if (ft_isalnum(token[i]) || token[i] == '_')
		return (1);
	return (0);
}

int special_ch_check(char c)
{
	if (c >= '1' && c <= '9')
		return (1);
	else if (c == '#')
		return (1);
	else if (c == '*')
		return (1);
	else if (c == '@')
		return (1);
	else if (c == '-')
		return (1);
	else if (c == '$')
		return (1);
	else if (c == '!')
		return (1);
	else
		return (0);
}

int special_character(char *token, t_expander *expander)
{
	if (token[expander->i] == '$')
	{
		expander->i++;
		return (1);
	}
	else if (special_ch_check(token[expander->i]))
	{
		expander->i++;
		while (special_ch_check(token[expander->i]))
			expander->i++;
		while(token[expander->i] != '\0' && (is_valid_ch(token, expander->i))) 
			expander->i++;
		return(1);
	}
	return (0);
}
int	question_mark(t_lexer_list *temp, int i, t_expander *expander)
{
	if (temp->token[i] == '?')
	{
		//execde işlem sonucunda dönen sayıyıyı yazdırır şimdlilik full 0 koyucam 
		expander->start = i;
		while (temp->token[i] != '\0' && is_valid_ch(temp->token, i))
			i++;
		expander->env_key = "?";
		expander->env_val = "0";
		temp->token = ft_strjoin_free(temp->token, expander);
		expander->i = 0;
		return (1);
	}
	return (0);
}
int	change_to_env(t_lexer_list *temp, int i, t_expander *expander, char **env)
{
	expander->start = i;
	while (temp->token[i] != '\0' && (is_valid_ch(temp->token ,i)))
	{
		i++;
	}
	expander->env_key = ft_substr(temp->token, expander->start, i - expander->start); // $ işaretinden sonraki karakterden başlayarak geçerli karaktere kadar olan kısmı alır
	expander->env_val = env_value(env, expander->env_key);
	if (expander->env_val != NULL)
	{
		temp->token = ft_strjoin_free(temp->token, expander);
		expander->i = 0;
		return (1);
	}
	expander->i = 0; 
	return (0);
}

int quote(char *token, t_expander *expander)
{
	if (token[expander->i] == '\'')
	{
		expander->i++;
		while (token[expander->i] != '\'' && token[expander->i] != '\0')
			expander->i++;
		return (1);
	}
	return (0);
}

void expander(t_lexer_list *temp, char **env, t_expander *expander)
{
	expander->token_len = ft_strlen(temp->token);
	while (temp != NULL)
	{
		expander->i = 0;//$$$$USER$$$USER$$USER$USE
		while (temp->token[expander->i] != '\0')
		{
			if (quote(temp->token, expander))
				continue;
			else if (temp->token[expander->i] == '$')
			{
				expander->dollar_index = expander->i;
				expander->i++;
				if (special_character(temp->token, expander))
					continue;
				if (question_mark(temp, expander->i, expander))
					continue;
				if (change_to_env(temp, expander->i, expander, env))
					continue;
			}
			expander->i++;
		}
		temp = temp->next;
	}
}
