package bidons;

import java.util.Scanner;
import java.util.Stack;

public class Partie {
	
	private int volumeCible;
	private Stack<Command> historique;
	private Bidon[] bidons;
	
	
	public Partie(int nbBidons, int[] capacitesBidons, int volumeCible) {
		bidons = new Bidon[nbBidons];
		for (int i = 0; i < nbBidons; i++) {
			bidons[i] = new Bidon(capacitesBidons[i]);
		}
		this.volumeCible = volumeCible;
		this.historique = new Stack<Command>();
	}

	private boolean gagne() {
		for (Bidon b: bidons) {
			if (b.getVolumeCourant() == volumeCible) {
				return true;
			}
		}
		return false;
	}
	
	private void jouerTour() {
		Command command = null;
		Scanner myObj = new Scanner(System.in);  // Create a Scanner object
	    System.out.println("Entrer votre choix (V x, R x, T x1 x2, u)");

	    String choix = myObj.nextLine();  // Read user input
	    boolean tourPasFini = true;
	    while (tourPasFini)
	    switch (choix.charAt(0) ){
	    	case 'V':
	    		command = new Vider(bidons[Character.getNumericValue(choix.charAt(2))]);
	    		tourPasFini = false;
	    		break;
	    	case 'R':
	    		command = new Remplir(bidons[Character.getNumericValue(choix.charAt(2))]);
	    		tourPasFini = false;
	    		break;
	    	case 'T':
	    		command = new Transvaser(bidons[Character.getNumericValue(choix.charAt(2))],
	    								bidons[Character.getNumericValue(choix.charAt(4))]);
	    		tourPasFini = false;
	    		break;
	    	case 'u':
	    		if (undo())
	    			{
	    			System.out.println("Undo effectue.");
		    		printBidons();
		    		System.out.println("Entrer votre choix (V x, R x, T x1 x2, u)\");");
		    		choix = myObj.nextLine();  // Read user input
		    		break;
	    			}
	    	default:
	    		System.out.println("Choix incorrect, ressayez en choissisant parmi un des 3 commandes : V x, R x, T x1 x2, u");
	    		choix = myObj.nextLine();  // Read user input
	    		break;
	    }
	    //execute command
	    
	    command.execute();
	    historique.add(command);

	    printBidons();
	}
	
	private boolean undo() {
		if (historique.size()> 0) {
			historique.peek().undo();
			System.out.println("Enleve la commande " + historique.pop().getClass());
			return true;
		}
		return false;
	}

	private void printBidons() {
		System.out.println("Volume des bidons:");
		for (Bidon b: bidons) {
			System.out.println(b.getVolumeCourant());
		}
	}
	
	public void jouer() {
		while (! gagne()) {
			jouerTour();
			System.out.println("commande " + historique.peek().getClass());

			
		}
	}
}
