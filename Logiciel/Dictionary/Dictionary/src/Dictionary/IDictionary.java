package Dictionary;
public interface IDictionary {
	Object get(Object key);
	IDictionary put(Object key, Object value);
	boolean isEmpty();
	boolean containsKey(Object key);
	int size();
}
