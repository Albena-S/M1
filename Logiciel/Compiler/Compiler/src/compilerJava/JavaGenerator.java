package compilerJava;
import compiler.*;
import compiler.text.*;
import java.io.File;

public class JavaGenerator implements Generator {

	@Override
	public File generate(AST a) {
		System.out.println("I am generating a JVM program text from a Java AbstractSyntaxTree");
		return null;
	}

}
