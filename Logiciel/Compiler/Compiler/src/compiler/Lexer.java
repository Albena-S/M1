package compiler;
import compiler.text.*;

public interface Lexer {
	
	public abstract ScanedText scan(ProgramText t);
}
