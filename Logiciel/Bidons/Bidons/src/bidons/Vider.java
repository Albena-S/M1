package bidons;

public class Vider extends Command {


	public Vider(Bidon b) {
		this.bidon = b;
		// TODO Auto-generated constructor stub
	}

	@Override
	public void execute() {
		volumeEauDeplace = this.bidon.vider();
	}

	@Override
	public void undo() {
		this.bidon.remplir(volumeEauDeplace);
		this.volumeEauDeplace = 0;
		
	}

}
