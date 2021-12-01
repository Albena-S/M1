package compilerJava;
import compiler.*;
import compiler.text.*;

public class JavaLexer implements Lexer {

	@Override
	public ScanedText scan(ProgramText t) {
		System.out.println("I am scanning a Java program text");
		return null;
	}

}
