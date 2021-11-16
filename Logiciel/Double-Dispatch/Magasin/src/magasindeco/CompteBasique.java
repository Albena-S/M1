package magasindeco;

import magasin.Client;

abstract class CompteBasique extends Compte {


	Client client;
	

	public CompteBasique(Client client) {
		super();
		this.client = client;
	}
	public Client getClient() {
		return client;
	}

	public void setClient(Client client) {
		this.client = client;
	}

	
	
}
