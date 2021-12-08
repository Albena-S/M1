package interpreteur;

import java.util.HashMap;

public class And extends  IForm {

	// OrExpression class implementing
	// the above interface. This interpreter
	// just returns the or condition of the
	// data is same as the interpreter data.

	IForm expr1;
	IForm expr2;

	public And(IForm expr1, IForm expr2) {
		this.expr1 = expr1;
		this.expr2 = expr2;
	}

	@Override
	public IForm simplif() {
			// (f, Top) | (Top, f) −> f
		if (expr1 == Top) {
			return expr2;
		}
		if (expr2 == Top) {
			return expr1;
		}
		// (_, Bot) | (Bot, _) −> Bot
		if (expr1 == Bot || expr2 == Bot ) {
			return Bot;
		}
		//(l, r) −> And (l, r);;
		else {
			return new And(expr1, expr2);
		}
	}

	@Override
	public Boolean eval(HashMap<String, Boolean> a) {
		return expr1.eval(a) && expr2.eval(a);
	}

	
}
