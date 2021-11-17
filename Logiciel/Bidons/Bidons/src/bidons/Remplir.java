package bidons;

public class Remplir extends Command {

	
	public Remplir(Bidon b) {
		this.bidon = b;
		// TODO Auto-generated constructor stub
	}


	@Override
	public void execute() {
		volumeEauDeplace = this.bidon.remplir(this.bidon.getCapacite());
	}

	@Override
	public void undo() {
		this.bidon.verse(volumeEauDeplace);
		this.volumeEauDeplace = 0;
		
	}
}
