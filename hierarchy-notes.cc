class ClAbstractVariable {
public:
  String name() const
    { return my_name; }
  void set_name(String const &name)
    { my_name = name; }

  bool isDummy() const
    { return false; }
  bool isExternal() const = 0;
  bool isPivotable() const = 0;
  bool isRestricted() const = 0;

  ostream &printOn(ostream &xo);
    {  xo << "CV#" << name << endl;  return xo; }

private:
  String my_name;
};


class ClDummyVariable: public ClAbstractVariable {
public:
  bool isDummy() const
    { return true; }
  bool isExternal() const
    { return false; }
  bool isPivotable() const
    { return false; }
  bool isRestricted() const
    { return true; }
};


class ClObjectVariable: public ClAbstractVariable {
public:
  bool isExternal() const
    { return false; }
  bool isPivotable() const
    { return false; }
  bool isRestricted() const
    { return false; }
};

class ClSlackVariable: public ClAbstractVariable {
public:
  bool isExternal() const
    { return false; }
  bool isPivotable() const
    { return true; }
  bool isRestricted() const
    { return true; }
};

class ClVariable: public ClAbstractVariable {
public:
  bool isExternal() const
    { return true; }
  bool isPivotable() const
    { return false; }
  bool isRestricted() const
    { return false; }

  ostream &printOn(ostream &xo);
    {  
      xo << "[" << name << ":";
      value.printOn(xo);
      xo << "]" << endl;
      return xo;
    }


  Number value() const
    { return my_value; }
  void set_value(Number const &value)
    { my_value = value; }


  ClLinearExpression asLinearExpression() const;

  /* Also has "@" for creating point pairs */
  /* arithmetic linear expression builders */
  /* constraint creators */ 

private:
  Number my_value;
};

