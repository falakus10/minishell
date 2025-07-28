#include "minishell.h"

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
int	change_to_env(t_lexer_list *temp, int i, t_expander *expander, t_env *env_list)
{
	expander->start = i;
	while (temp->token[i] != '\0' && (is_valid_ch(temp->token ,i)))
	{
		i++;
	}
	expander->env_key = ft_substr(temp->token, expander->start, i - expander->start); // $ işaretinden sonraki karakterden başlayarak geçerli karaktere kadar olan kısmı alır
	expander->env_val = env_value(env_list, expander->env_key);
	if (expander->env_val != NULL)
	{
		temp->token = ft_strjoin_free(temp->token, expander);
		expander->i = 0;
		return (1);
	}
	else//burayı < > >> << göre güncelle null olma durumunu
	{
		expander->i = ft_strlen(expander->env_key);
	} 
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

void expander(t_lexer_list *temp, t_env *env_list, t_expander *expander)
{
	if (temp == NULL || temp->token == NULL)
	    return;
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
				if (change_to_env(temp, expander->i, expander, env_list))
					continue;
			}
			expander->i++;
		}
		temp = temp->next;
	}
}
