package compiler;

public interface FactoryForCompiler {
	
	public abstract Lexer createLexer();
	public abstract Parser createParser();
	public abstract Generator createGenerator();
}
