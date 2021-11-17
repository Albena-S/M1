package bidons;

public class Bidon {
	
	private int volumeCourant;

	private int capacite;
	
	public Bidon(int capacite) {
		this.capacite = capacite;
	}

	public int vider() {
		int volVide = volumeCourant;
		this.volumeCourant = 0;
		return volVide;
	}
	
	public int remplir(int v) {
		int exVolCourant = volumeCourant;
		this.volumeCourant =  volumeCourant + v <= capacite ? volumeCourant + v : capacite;
		return volumeCourant - exVolCourant ;
	}
	
	public int verse(int v) {
		int exVolCourant = volumeCourant;
		this.volumeCourant =  volumeCourant - v >= 0 ? volumeCourant - v : 0;
		
		return exVolCourant - volumeCourant;
	}
	
	public int getVolumeCourant() {
		return volumeCourant;
	}

	public void setVolumeCourant(int volumeCourant) {
		this.volumeCourant = volumeCourant;
	}

	public int getCapacite() {
		return capacite;
	}

	public void setCapacite(int capacite) {
		this.capacite = capacite;
	}
	
	
}
