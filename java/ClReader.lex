package edu.washington.grad.will;

import java_cup.runtime.Symbol;

%%

%cup

DIGIT		= [0-9]

ALPHA		= [A-Za-z]

ALPHANUM	= [A-Za-z0-9]

ID		= {ALPHA}{ALPHANUM}*

ws		= [ \t\r\n\f]+

%%

{ws}					{ /* skip white space */						}

">="					{ return new Symbol(sym.GEQ					);	}

">"					{ return new Symbol(sym.GT					);	}

"<="					{ return new Symbol(sym.LEQ					);	}

"<"					{ return new Symbol(sym.LT					);	}

"="					{ return new Symbol(sym.EQ					);	}

"+"					{ return new Symbol(sym.PLUS					);	}

"-"					{ return new Symbol(sym.MINUS					); 	}

"*"					{ return new Symbol(sym.TIMES					);	}

"/"					{ return new Symbol(sym.DIVIDE					);	}

"("					{ return new Symbol(sym.LPAREN					);	}

")"					{ return new Symbol(sym.RPAREN					);	}

{DIGIT}+("."{DIGIT}*)?|("."{DIGIT}+)	{ return new Symbol(sym.NUMBER		, new Double(yytext())	);	}

{ID}					{ return new Symbol(sym.VARIABLE	, new String(yytext())	);	}

.					{ System.err.println("Illegal character: " + yytext() );		}
