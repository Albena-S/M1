package magasindeco;

public class MainMagasin {

	public static void main(String[] args) {
		Produit lgv = new Produit("La grande vadrouille", 10.0f);
		Client cl = new Client("Dupont");
//		exo1
//		Compte cmt = new Compte(cl);
//		cmt.prixLocation(lgv);
//		Compte cmt2 = new CompteAvecReduction(cl);
//		System.out.println("CompteReduction : " + cmt2.prixLocation(lgv));
//		Produit r4 = new ProduitSolde("RockyIV", 10.0f);
//		System.out.println("CompteNormal+ProduitSold ́e : " + cmt.prixLocation(r4));
	
		//un compte normal pour le client Dupont
		Compte cmt = new Compte(cl);
		System.out.println("basique lgv : " + cmt.prixLocation(lgv));
		
		//Dupont achete un forfait réduction.
		cmt = new ForfaitReduction (cmt);
		System.out.println("réduction lgv : " + cmt.prixLocation(lgv));
	}

}
