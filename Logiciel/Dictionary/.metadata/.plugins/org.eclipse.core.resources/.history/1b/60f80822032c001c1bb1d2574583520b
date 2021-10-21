package Dictionary;

public class UtilisationDictionnaire {

	public static void main(String[] args) {
		IDictionary dic;
		//dic = new OrderedDictionary();
		dic = new FastDictionary(4);
		dic.put("0", 10);
		Object value = dic.get("0");
		System.out.println((int)value ==10 ? "Test sucessful." : "Test unsucessful.");
		dic.put(1, "January");
		value = dic.get(1);
		System.out.println((value.toString()).equals("January") ? "Test sucessful." : "Test unsucessful.");
		
		boolean empty = dic.isEmpty();
		System.out.println(empty==false ? "Test sucessful." : "Test unsucessful.");
		
		boolean containsKey = dic.containsKey(1);
		int size = dic.size();
		System.out.println("value -" + value + " empty -" + empty + " size - "+ size );
		
	}

}
