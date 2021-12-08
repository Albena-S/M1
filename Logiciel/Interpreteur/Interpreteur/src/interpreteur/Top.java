package interpreteur;

import java.util.HashMap;

public final class Top implements IForm {

	private static Top instance;

	private Top() {
	}
	
    public static Top getInstance() {
        if (instance == null) {
            instance = new Top();
        }
        return instance;
    }
    
	@Override
	public IForm simplif() {
		return this;
	}

	@Override
	public Boolean eval(HashMap<String, Boolean> a) {
		return true;
	}

	
}
