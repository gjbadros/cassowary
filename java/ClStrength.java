
/*
 * class ClStrength
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClStrength
{

/**
* ClStrength
* @param name
* @param symbolicWeight
*/
public
ClStrength(string& name, ClSymbolicWeight& symbolicWeight)
{
 my_name = name;
	my_symbolicWeight = symbolicWeight;
	 }

/**
* ClStrength
* @param name
* @param w1
* @param w2
* @param w3
*/
public
ClStrength(string& name, double w1, double w2, double w3)
{
 my_name = name;
	my_symbolicWeight = w1w2w3;
	 
}

/**
* ClStrength
*/
public
~ClStrength()
{ }

/**
* isRequired
* @return bool
*/
public
bool isRequired()
{ return (my_symbolicWeight == clsRequired().my_symbolicWeight); }

/**
* printOn
* @param xo
* @return ostream &
*/
public
/* @c2j++: "ostream & printOn(ostream& xo)" replacement:  &  to " " */
ostream printOn(ostream& xo)
{ 
    xo << name(); 
    if (!isRequired())
      xo << ":" << symbolicWeight(); 
    return xo; 
    }

/**
* symbolicWeight
* @return ClSymbolicWeight
*/
public
ClSymbolicWeight symbolicWeight()
{ return my_symbolicWeight; }

/**
* name
* @return string
*/
private
string name()
{ return my_name; }

/**
* set_name
* @param name
*/
private
void set_name(string name)
{ my_name = name; }

/**
* set_symbolicWeight
* @param symbolicWeight
*/
private
void set_symbolicWeight(ClSymbolicWeight& symbolicWeight)
{ my_symbolicWeight = symbolicWeight; }

/**
* <
* @param xos
* @param cls
* @return <
*/
private
< <(ostream& xos, ClStrength& cls)
{ cls.printOn(xos); return xos; }
string my_name;
ClSymbolicWeight my_symbolicWeight;

/**
* @return ClStrength&
*/
static ClStrength& clsRequired()
{
  static ClStrength required_strength("<Required>", 1000, 1000, 1000);
  return required_strength;
}
/**
* @return ClStrength&
*/
static ClStrength& clsStrong()
{
  static ClStrength strong_strength("strong", 1.0, 0.0, 0.0);
  return strong_strength;
}
/**
* @return ClStrength&
*/
static ClStrength& clsMedium()
{
  static ClStrength medium_strength("medium", 0.0, 1.0, 0.0);
  return medium_strength;
}
/**
* @return ClStrength&
*/
static ClStrength& clsWeak()
{
  static ClStrength weak_strength("weak", 0.0, 0.0, 1.0);
  return weak_strength;
}
}
