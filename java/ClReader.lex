package edu.washington.grad.will;

import java_cup.runtime.Symbol;

import EDU.Washington.grad.gjb.cassowary.*;

%%

%cup

%public

DIGIT		= [0-9]

ALPHA		= [A-Za-z]

ALPHANUM	= [A-Za-z0-9]

ID		= {ALPHA}{ALPHANUM}*

ws		= [ \t\r\n\f]+

%init{

	// code to go into constructor

%init}

%{
	// added code to lexer class

	private java.util.Hashtable m_variable_name_object_hash;

	public void setVariableNameObjectHash(java.util.Hashtable variable_name_object_hash)
	{
		m_variable_name_object_hash = variable_name_object_hash;
	}

%}

%%

{ws}					{ /* skip white space */					}

">="					{ return new Symbol(sym.GEQ				);	}

"<="					{ return new Symbol(sym.LEQ				);	}

"="					{ return new Symbol(sym.EQ				);	}

"+"					{ return new Symbol(sym.PLUS				);	}

"-"					{ return new Symbol(sym.MINUS				); 	}

"*"					{ return new Symbol(sym.TIMES				);	}

"/"					{ return new Symbol(sym.DIVIDE				);	}

"("					{ return new Symbol(sym.LPAREN				);	}

")"					{ return new Symbol(sym.RPAREN				);	}

{DIGIT}+("."{DIGIT}*)?|("."{DIGIT}+)	{ return new Symbol(sym.NUMBER	, new Double(yytext())	);	}

{ID}					{
						String variable_name = new String(yytext());

						System.out.println("Lexical analysis found <" + variable_name + ">");

						if (! m_variable_name_object_hash.containsKey(variable_name))
						{
							System.out.println("	Putting it in hash for the first time.");

							ClVariable variable = new ClVariable();

							Object return_value = m_variable_name_object_hash.put(variable_name, variable);

							if (return_value != null)
							{
								System.out.println("Variable was already in hash!!!!!");
							}

							if (m_variable_name_object_hash.containsKey(variable_name))
							{
								System.out.println("	Hash table now contains object.");
							}
							else
							{
								System.out.println("	Hash table does not contain object.");
							}

							if (m_variable_name_object_hash.isEmpty())
							{
								System.out.println("	How can the hashtable be empty after inserting something?");
							}
						}
						else
						{
							System.out.println("	Already in Hash.");
						}

						return new Symbol(sym.VARIABLE, variable_name);
					}

.					{ System.err.println("Illegal character: " + yytext() );		}
