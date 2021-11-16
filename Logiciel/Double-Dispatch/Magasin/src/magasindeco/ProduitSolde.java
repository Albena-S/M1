package magasindeco;

public class ProduitSolde extends Produit {

	public ProduitSolde(String nom, float prix) {
		super(nom, prix);
	}
	
	public float calculerPrix() {
		return super.calculerPrix() /2;
	}
}
