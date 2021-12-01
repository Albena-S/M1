package letterGame;

public interface GameAndAlphabetFactory {
	
	Game createGame(String word);
	EncriptedAlphabet createAlphabetCoding(String word);
}
