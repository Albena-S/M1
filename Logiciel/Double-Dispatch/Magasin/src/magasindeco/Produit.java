package magasindeco;

public class Produit {
	String nom;

	float prix;

	public Produit(String nom, float prix) {
		super();
		this.nom = nom;
		this.prix = prix;
	}

	public String getNom() {
		return nom;
	}

	public void setNom(String nom) {
		this.nom = nom;
	}

	public float getPrix() {
		return prix;
	}

	public void setPrix(float prix) {
		this.prix = prix;
	}

	public float calculerPrix() {
		return this.getPrix();
	}
	
	
	
}
