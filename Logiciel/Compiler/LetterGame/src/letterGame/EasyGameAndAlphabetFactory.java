package letterGame;

public class EasyGameAndAlphabetFactory implements GameAndAlphabetFactory {

	@Override
	public Game createGame(String word) {
		// TODO Auto-generated method stub
		return new AssistedGame(word);
	}

	@Override
	public EncriptedAlphabet createAlphabetCoding(String word) {
		// TODO Auto-generated method stub
		return new EasyEncriptedAlphabet(word);
	}

}
