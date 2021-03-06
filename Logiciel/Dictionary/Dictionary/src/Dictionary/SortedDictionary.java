package Dictionary;

import java.util.Arrays;

public class SortedDictionary extends AbstractDictionary {
	//	comme ca on a 2 tableaux keys - objet et comparable
	//	protected Comparable[] keys;
	//	protected Comparable[] values;
	//	pas la bonne solution

	public SortedDictionary(int n) {
		super(n);
		// TODO Auto-generated constructor stub
	}
	public SortedDictionary(){
		super(0);
		this.size = 0;
	}
	
	@Override
	int indexOf(Object key) {
		if (size == 0)
			return -1;
		return binarySearch(this.keys,0,this.size,(Comparable<Object>) key);
	}

	@Override
	int newIndexOf(Object key) {		
		int index = size == 0 ? 0 : binarySearchIndex(this.keys, 0, size, key);

		
		Object[] tamponKeys =  Arrays.copyOf(keys, size);
		Object[] tamponValeurs =  Arrays.copyOf(valeurs, size);
		size++;
		keys = new Object[size];
		valeurs = new Object[size];
		if (size-1 != 0 ) {
			System.arraycopy(tamponKeys, 0, keys, 0, index-1);
			System.arraycopy(tamponValeurs, 0, valeurs, 0, index-1);
			
			System.arraycopy(tamponKeys, index , keys, index +1  , size-1-index);
			System.arraycopy(tamponValeurs,index  , valeurs, index+1  , size-1-index );
		}

		return index;
	}

	int binarySearch(Object a[], int l, int h, Object x) {
		if (h >= l) {
			int mid = l + (h - l) / 2;
			Comparable aMidComparable = (Comparable<Object>)a[mid], xComparable = (Comparable<Object>)x;
			// If the element is present at the middle itself
			if (aMidComparable.compareTo(xComparable) == 0)
				return mid;
			// If element is smaller than mid, then it can only be present in left subarray
			if  (aMidComparable.compareTo(xComparable) == -1 )
				return binarySearch(a, l, mid - 1, x);
			// Else the element can only be present in right subarray
			return binarySearch(a, mid + 1, h, x);
		}
		// We reach here when element is not present in array
		return -1;
	}
	
	int binarySearchIndex(Object a[], int l, int h, Object x)
	{
		if (h > l) {
			int mid = l + (h - l) / 2;
			Comparable aMidComparable = (Comparable<Object>)a[mid], xComparable = (Comparable<Object>)x;
			// If the element is present at the middle itself
			if (aMidComparable.compareTo(xComparable) == 0)
				return mid;
			// If element is smaller than mid, then it can only be present in left subarray
			if  (aMidComparable.compareTo(xComparable) == 1 )
				return binarySearchIndex(a, l, mid - 1, x);
			// Else the element can only be present in right subarray
			return binarySearchIndex(a, mid + 1, h, x);
		}
		// We reach here when element is not present in array
		return h;
	}
}
