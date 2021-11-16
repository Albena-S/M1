package magasindeco;

import magasin.Client;

abstract class Compte {

	public float prixLocation(Produit produit){
		return produit.calculerPrix();
	}

	
	
}
