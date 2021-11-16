package magasin;

public class Compte {

	Client client;
	

	public Compte(Client client) {
		super();
		this.client = client;
	}
	
	
	public float prixLocation(Produit produit){
		return produit.calculerPrix();
	}

	public Client getClient() {
		return client;
	}

	public void setClient(Client client) {
		this.client = client;
	}
	
	
}
