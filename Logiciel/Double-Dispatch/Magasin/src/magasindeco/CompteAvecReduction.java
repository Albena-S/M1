package magasindeco;

public class CompteAvecReduction extends Compte {

	public CompteAvecReduction(Client client) {
		super(client);
	}
	
	public float prixLocation(Produit produit){
		return super.prixLocation(produit) * (float) 0.9;
	}

}
