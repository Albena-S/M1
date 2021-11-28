package calculetteInfixe;

public class MainCalculette {
	public static void main(String[] args) {


		Calculette calclette = new Calculette();
		double acc = calclette.getAcc();

		calclette.enter("plus");
		calclette.enter("1");
		calclette.enter("1");
		acc = calclette.getAcc();
		calclette.enter("1");
	}
}
