
/*
 * class ClDummyVariable
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClDummyVariable extends ClAbstractVariable
{
friend ClTableau;
friend ClSimplexSolver;

/**
* ClDummyVariable
*/
protected
ClDummyVariable()
{
 super(name);
	 }

/**
* ClDummyVariable
* @param number
* @param prefix
*/
protected
/* @c2j++: "ClDummyVariable(long number, char* prefix)" replacement: char*  to String  */
ClDummyVariable(long number, String prefix)
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
    xo << "[" << name() << ":dummy]";
    return xo;
  }

/**
* isDummy
* @return bool
*/
protected
bool isDummy()
{ return true; }

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
{ return false; }

/**
* isRestricted
* @return bool
*/
protected
bool isRestricted()
{ return true; }

}
