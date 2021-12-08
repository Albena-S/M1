package interpreteur;

import java.util.HashMap;

//Expression interface used to
//check the interpreter.
interface IForm
{
 public IForm simplif();
 public Boolean eval(HashMap<String,Boolean> a);
}