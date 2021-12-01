package compiler;
import compiler.text.*;

public interface Parser {
	public AST parse(ScanedText t);
}
