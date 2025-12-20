#include "ft_yacc.h"
#include "lexer.h"

/* If an identifier (or other rule) is immediately followed by ++ or -- with no
   intervening space, extend the token lexeme to include the operator and update
   the yacc position bookkeeping. This keeps "i++" as a single token lexeme.
   You may add more complex rules here (e.g., combined assignment operators). */
void	transform_merge_postfix(t_ytoken *token, t_yacc *yacc, size_t before_pos)
{
	(void)before_pos;
	/* current input pos is at end of the just-matched lexeme */
	size_t pos = yacc->input.pos;
	if (pos + 1 >= yacc->input.len)
		return;
	char a = dyn_str_idx(&yacc->input, pos);
	char b = dyn_str_idx(&yacc->input, pos + 1);
	if ((a == '+' && b == '+') || (a == '-' && b == '-'))
	{
		/* consume the two operator chars */
		yacc->input.pos = pos + 2;
		/* update position counters for these extra chars */
		yacc_update_position(yacc, 2);
		/* refresh token->input to reflect extended lexeme */
		token->input = yacc->input;
		/* keep token->type as identifier (or change to a dedicated combined token if desired) */
	}
}
