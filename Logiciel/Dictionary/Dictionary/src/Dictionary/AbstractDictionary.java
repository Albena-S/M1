package Dictionary;

public abstract class AbstractDictionary implements IDictionary {
	
	protected Object[] keys;
	protected Object[] valeurs;
	protected int size;
	
	abstract int indexOf(Object key);
	abstract int newIndexOf(Object key) ;
	
	public AbstractDictionary(int n) {
		keys = new Object[n];
		valeurs = new Object[n];
		this.size = n;
	}
	@Override
	public Object get(Object key){
		int index = this.indexOf(key);
		if (index != -1)
			return valeurs[index];
		return null;
	}

	@Override
	public IDictionary put(Object key, Object value) {
		if (this.indexOf(key) == -1) {
			int index = this.newIndexOf(key);
			valeurs[index] = value;
			keys[index] = key;
		}
		return this;			
	}

	@Override
	public boolean isEmpty() {
		return size() == 0;
	}

	@Override
	public boolean containsKey(Object key) {
		return indexOf(key) != -1;
	}

	@Override
	public int size() {
		return this.size;
	}
	

	
}
