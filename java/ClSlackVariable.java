
/*
 * class ClSlackVariable
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClSlackVariable extends ClAbstractVariable
{
friend ClTableau;
friend ClSimplexSolver;

/**
* ClSlackVariable
*/
protected
ClSlackVariable()
{
 super(name);
	 }

/**
* ClSlackVariable
* @param number
* @param prefix
*/
protected
/* @c2j++: "ClSlackVariable(long number, char* prefix)" replacement: char*  to String  */
ClSlackVariable(long number, String prefix)
{
 super(number,prefix);
	 }

/**
* printOn
* @param xo
* @return ostream &
*/
protected
/* @c2j++: "ostream & printOn(ostream& xo)" replacement:  &  to " " */
ostream printOn(ostream& xo)
{  
    xo << "[" << name() << ":slack]";
    return xo;
  }

/**
* isExternal
* @return bool
*/
protected
bool isExternal()
{ return false; }

/**
* isPivotable
* @return bool
*/
protected
bool isPivotable()
{ return true; }

/**
* isRestricted
* @return bool
*/
protected
bool isRestricted()
{ return true; }

}
