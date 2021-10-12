package Dictionary;
import java.util.Arrays;


public class OrderedDictionary extends AbstractDictionary {

	OrderedDictionary(){
		super(0);
		this.size = 0;
	}
	@Override
	int indexOf(Object key) {
	int i=0;
	while (i<this.size) {
		if (key.equals(keys[i]))
			return i;
		i++;
	}
	return -1;
	}

	@Override
	int newIndexOf(Object key) {
		Object[] tamponKeys =  Arrays.copyOf(keys, size);
		Object[] tamponValeurs =  Arrays.copyOf(valeurs, size);
		size++;
		keys = new Object[size];
		valeurs = new Object[size];
		
		System.arraycopy(tamponKeys, 0, keys, 0, size-1);
		System.arraycopy(tamponValeurs, 0, valeurs, 0, size-1);
		return size-1;
	}

}
