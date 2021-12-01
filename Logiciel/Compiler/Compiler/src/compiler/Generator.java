package compiler;

import java.io.File;

import compiler.text.*;

public interface Generator {
	public abstract File generate(AST a);
}
