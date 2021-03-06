package Dictionary;

import java.util.Arrays;
import java.lang.Math;

public class FastDictionary extends AbstractDictionary {

	public FastDictionary(int n) {
		super(n);
	}
	@Override
	public int size() {
		int nb=0;
		for (int i = 0; i < keys.length; i++) {
			if (keys[i]!=null)
				nb++;
		}
		size=nb;
		return nb;
	}
	@Override
	int indexOf(Object key) {
		int pos = Math.abs(key.hashCode()) % this.keys.length; 
		while ( keys[pos] != null) {
			if (!keys[pos].equals(key) )
				pos = (pos+1) % this.keys.length;
			else
				return pos;
		}
		return -1;
	}
	@Override
	int newIndexOf(Object key) {
		if (this.mustGrow())
			grow();
		int pos = Math.abs(key.hashCode()) % this.keys.length; 
		while ( true ) {
			if (keys[pos]!= null)
				pos = (pos+1) % this.keys.length;
			else
				break;
		}
		return pos;
	}
	
	private boolean mustGrow() {
		return this.keys.length <= this.size()*(3/4);
	}
	private void grow() {
		Object[] tamponKeys =  Arrays.copyOf(keys, keys.length);
		Object[] tamponValeurs =  Arrays.copyOf(valeurs, valeurs.length);
		this.keys = new Object[keys.length*2];
		this.valeurs = new Object[keys.length];
		for (int i = 0; i < keys.length; i++) {
			this.put(tamponKeys[i], tamponValeurs[i]);
		}
		
	}

}
