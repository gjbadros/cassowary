#!/uns/bin/python

import string, os
import cass

# Various Constant strings
MARSHALLEDFILENAME = "grammar.mar"
STRREGEX = '[_a-zA-Z]+'
INTREGEX = '[0-9]+'
ParseException = 'Parse exception'

variables = {}

# The string defining the rules in the grammar
GRAMMARSTRING = """
	Equation ::
	@R Rule0		:: Equation >> Expression Operator Expression
	@R Rule1		:: Operator >> =
	@R Rule2		:: Operator >> >
	@R Rule3		:: Operator >> <
	@R Rule4		:: Expression >> Term
	@R Rule5		:: Expression >> Term Plus Term
	@R Rule6		:: Plus >> +
	@R Rule7		:: Term >> Variable
	@R Rule8 		:: Term >> Variable Times Constant
	@R Rule9		:: Variable >> string
	@R Rule10		:: Times >> *
	@R Rule11		:: Constant >> int
	@R Rule12		:: Term >> Constant
"""

class Parser:
	def __init__(self):
		self._grammar = None
		self._variables = {}

	# Set up some variables
	def add_variable(self, Name, var):
		self._variables[Name] = var

	# Clear the set of variables
	def clear(self):
		self._variables = {}

	# Semantic interpretation functions for terminals
	def intInterp(self, s):
		return string.atoi(s)
	def strInterp(self, s):
		if not self._variables.has_key(s):
			print self._variables
			raise ParseException, 'no variable named ' + s
		return self._variables[s]
	# The function which binds terminals to their interpretation funcs
	def DeclareTerminals(self):
		self._grammar.Addterm("string", STRREGEX, self.strInterp)
		self._grammar.Addterm("int", INTREGEX, self.intInterp)

	# Semantic interpretation functions for rules
	def EchoValue(self, list, context):
		return list[0]
	def MakeEquation(self, list, context):
		return list
#		if list[1] == '=':
#			return cass.Equality(list[0], list[-1])
#		elif list[1] == '>=':
#			return Inequality(list[0], list[-1])
#		elif list[1] == '<=':
#			return Inequality(list[0], list[-1])
	def MakePlus(self, list, context):
		return list
#		return list[0] + list[-1]
	def MakeTimes(self, list, context):
		return list
#		return list[0] * list[-1]

	# Bind rules to their semantic functions
	def BindRules(self):
		self._grammar.Bind("Rule0", self.MakeEquation)
		self._grammar.Bind("Rule1", self.EchoValue)
		self._grammar.Bind("Rule2", self.EchoValue)
		self._grammar.Bind("Rule3", self.EchoValue)
		self._grammar.Bind("Rule4", self.EchoValue)
		self._grammar.Bind("Rule5", self.MakePlus)
		self._grammar.Bind("Rule6", self.EchoValue)
		self._grammar.Bind("Rule7", self.EchoValue)
		self._grammar.Bind("Rule8", self.MakeTimes)
		self._grammar.Bind("Rule9", self.EchoValue)
		self._grammar.Bind("Rule10", self.EchoValue)
		self._grammar.Bind("Rule11", self.EchoValue)
		self._grammar.Bind("Rule12", self.EchoValue)

	# The main function, builds the grammar
	def GrammarBuild(self, load=1):
		if load and os.path.exists(MARSHALLEDFILENAME):
			import kjParser
			infile = open(MARSHALLEDFILENAME, 'r')
			self._grammar = kjParser.UnMarshalGram(infile)
			infile.close()
			self.DeclareTerminals()

		else:
			import kjParseBuild
			self._grammar = kjParseBuild.NullCGrammar()
			self._grammar.SetCaseSensitivity(0)
			self._grammar.punct('=<>+*')
			self._grammar.Nonterms("Equation Expression Operator ")
			self._grammar.Nonterms("Term Variable Plus Times Constant ")
			self.DeclareTerminals()
			self._grammar.Declarerules(GRAMMARSTRING)
			self._grammar.Compile()

			outfile = open(MARSHALLEDFILENAME, "w")
			self._grammar.MarshalDump(outfile)
			outfile.close()

		self.BindRules()

	#-----------------------------------------------------------------
	# Main entry point

	# Parse(s : string)
	def Parse(self, s):
		if not self._grammar:
			self.GrammarBuild()
		return self._grammar.DoParse1(s)

def main():
	p = Parser()

if __name__ == '__main__':
	main()
