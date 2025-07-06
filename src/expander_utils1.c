#include "minishell.h"

int	is_valid_ch(t_lexer_list *node, int i)
/* $ işaretinden sonraki karakter geçerli mi? */
{
	char c = node->token[i];
	return (ft_isalnum(c) || c == '_');
}

char	*env_value(char **env, const char *key)
/* Bulursa değerini, bulamazsa "" (heap kopya) döndürür               */
{
	size_t	key_len;
	int		i;

	if (!env || !key)
		return (ft_strdup(""));

	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
			return (ft_substr(env[i], key_len + 1,
					ft_strlen(env[i]) - (key_len + 1)));   /* malloc’lı */
		++i;
	}
	return (ft_strdup(""));                                 /* malloc’lı "" */
}


char	*ft_strjoin_free(char *val, char *tok, const char *key)
{
	char	*dollar;
	size_t	pre, key_len, val_len, new_len;
	char	*new;

	if (!val || !tok || !key)
		return (NULL);

	key_len = ft_strlen(key);
	dollar  = ft_strnstr(tok, key, ft_strlen(tok));
	if (!dollar || dollar == tok || *(dollar - 1) != '$')
		return (tok);                               /* güvenlik: bulunamadı */

	pre      = (size_t)(dollar - 1 - tok);          /* '$' hariç prefix   */
	val_len  = ft_strlen(val);
	new_len  = pre + val_len
			 + (ft_strlen(tok) - pre - key_len - 1); /* -1: '$'           */

	new = malloc(new_len + 1);
	if (!new)
		return (NULL);

	/* prefix */
	ft_memcpy(new, tok, pre);
	/* value  */
	ft_memcpy(new + pre, val, val_len);
	/* suffix */
	ft_memcpy(new + pre + val_len,
			  dollar + key_len,
			  ft_strlen(tok) - pre - key_len - 1);

	new[new_len] = '\0';

	free(tok);
	free(val);
	return (new);
}

void	expander(t_lexer_list *lst, char **env)
{
	t_lexer_list *cur;
	size_t		  i, start;
	char		 *key, *val;
    
	for (cur = lst; cur; cur = cur->next)
	{
		i = 0;
		while (cur->token[i])
		{
			/* $ + geçerli isim mi? */
			if (cur->token[i] == '$'
				&& (ft_isalpha(cur->token[i + 1]) || cur->token[i + 1] == '_'))
			{
				i++;                           /* '$' atla          */
				start = i;
				while (cur->token[i] && is_valid_ch(cur, i))
					++i;                       /* KEY sonu          */

				key = ft_substr(cur->token, start, i - start);
				val = env_value(env, key);     /* her zaman malloc  */
				cur->token = ft_strjoin_free(val, cur->token, key);
				free(key);

				i = 0;                         /* yeni tokene baştan tarama */
				continue;
			}
			/* tek / çift tırnak bloklarını geç                   */
			if (cur->token[i] == '\'' || cur->token[i] == '\"')
			{
				char q = cur->token[i++];
				while (cur->token[i] && cur->token[i] != q)
					++i;
				if (cur->token[i])
					++i;                       /* kapanış tırnağı   */
				continue;
			}
			++i;                               /* sıradan karakter  */
		}
	}
}