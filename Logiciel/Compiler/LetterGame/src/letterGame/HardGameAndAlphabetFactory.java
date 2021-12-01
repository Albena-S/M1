package letterGame;

public class HardGameAndAlphabetFactory implements GameAndAlphabetFactory {

	@Override
	public Game createGame(String word) {
		// TODO Auto-generated method stub
		return new Game();
	}

	@Override
	public EncriptedAlphabet createAlphabetCoding(String word) {
		// TODO Auto-generated method stub
		return new HardEncriptedAlphabet(word);
	}

}
