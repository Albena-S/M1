package bidons;

public abstract class Command {
	
	protected int volumeEauDeplace;
	protected Bidon bidon;
	
	
	public abstract void execute();
	public abstract void undo();
	
	
	public int getVolumeEauDeplace() {
		return volumeEauDeplace;
	}
	public void setVolumeEauDeplace(int volumeEauDeplace) {
		this.volumeEauDeplace = volumeEauDeplace;
	}
}
