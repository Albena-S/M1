package compilerJava;
import compiler.*;
public class FactoryForJavaCompiler implements FactoryForCompiler {

	@Override
	public Lexer createLexer() {
		// TODO Auto-generated method stub
		return new JavaLexer();
	}

	@Override
	public Parser createParser() {
		// TODO Auto-generated method stub
		return new JavaParser();
	}

	@Override
	public Generator createGenerator() {
		// TODO Auto-generated method stub
		return new JavaGenerator();
	}

}
