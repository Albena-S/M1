package bidons;

public class Transvaser extends Command {

	private Bidon bidonCible;
	
	
	public Transvaser(Bidon source, Bidon bidonCible) {
		this.bidonCible = bidonCible;
		this.bidon = source;
	}
	@Override
	public void execute() {
		int eauSource = this.bidon.vider();
		this.volumeEauDeplace = this.bidonCible.remplir(eauSource);
		if (eauSource != this.volumeEauDeplace) {
			this.bidon.remplir(eauSource - this.volumeEauDeplace);
		}
	}

	@Override
	public void undo() {
		this.bidon.remplir(this.volumeEauDeplace);
		this.bidonCible.verse(volumeEauDeplace);

	}

}
