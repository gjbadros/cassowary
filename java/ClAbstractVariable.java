
/*
 * class ClAbstractVariable
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClAbstractVariable
{

/**
* ClAbstractVariable
*/
public
ClAbstractVariable()
{
 my_name = name;
	 
    iVariableNumber++;
    if (name.length() == 0)
      {
      char sz[16];
      sprintf(sz,"v%ld",iVariableNumber++);
      my_name = string(sz);
      }
    }

/**
* ClAbstractVariable
* @param varnumber
* @param prefix
*/
public
/* @c2j++: "ClAbstractVariable(long varnumber, char* prefix)" replacement: char*  to String  */
ClAbstractVariable(long varnumber, String prefix)
{
    auto_ptr<char> pch (new char[16+strlen(prefix)]);
    iVariableNumber++;
    sprintf(pch.get(),"%s%ld",prefix,varnumber);
    my_name = string(pch.get());
    }

/**
* ClAbstractVariable
*/
public
~ClAbstractVariable()
{ }

/**
* name
* @return string
*/
public
string name()
{ return my_name; }

/**
* setName
* @param name
*/
public
void setName(const& name)
{ my_name = name; }

/**
* isDummy
* @return bool
*/
public
bool isDummy()
{ return false; }

/**
* isExternal

public


/**
* isPivotable

public


/**
* isRestricted

public


/**
* printOn

public


/**
* <
* @param xos
* @param clv
* @return <
*/
public
< <(ostream& xos, ClAbstractVariable& clv)
{ clv.printOn(xos); return xos; }

/**
* <
* @param cl1
* @param cl2
* @return operator
*/
public
operator <(ClAbstractVariable& cl1, ClAbstractVariable& cl2)
{ return &cl1 < &cl2; }

/**
* =
* @param cl1
* @param cl2
* @return =
*/
public
= =(ClAbstractVariable& cl1, ClAbstractVariable& cl2)
{ 
    return &cl1 == &cl2;
    }

/**
* =
* @param cl1
* @param cl2
* @return =
*/
public
= =(ClAbstractVariable& cl1, ClAbstractVariable& cl2)
{ 
    return &cl1 == &cl2;
    }
string my_name;
long iVariableNumber;

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "typedef ClAbstractVariable * PClAbstractVariable ;" replacement: * to " " */
typedef ClAbstractVariable   PClAbstractVariable ; 

/*@c2j++ The following variable used to be declared global */
long ClAbstractVariable : : iVariableNumber = 0 ; 

}
