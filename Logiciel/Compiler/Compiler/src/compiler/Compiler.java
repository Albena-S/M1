package compiler;
import java.lang.reflect.Constructor;

//import compiler.readables.ProgramText;
import compiler.text.*;
import compilerJava.*;

public class Compiler {

	protected Lexer lexer; // analyse lexicale
	protected Parser parser; // analyse syntaxique
	protected Generator gen; // g´en´eration de code
	protected String langage;

	public Compiler(String langage) throws Exception {
		FactoryForCompiler f;
		this.langage = langage;
		String classeName = "compiler"+langage+".FactoryFor" + langage + "Compiler";

		try{
			Class classe = Class.forName(classeName);
		
		Constructor constructor = classe.getDeclaredConstructor();
		f = (FactoryForCompiler) constructor.newInstance();
		}catch(Exception e) {
			throw new Exception("Non supported Language : " +langage + ", Extend the framework to support it */");
		}
//		if (langage.equals("Java"))
//			f = new FactoryforJavaCompiler();
//		else {
//			throw new Exception("Non supported Language : " +langage + ", Extend the framework to support it */");
//		}
		init(f);
	}

	private void init(FactoryForCompiler f) {
		lexer = f.createLexer();
		parser = f.createParser();
		gen = f.createGenerator();

	}

	public void compile(ProgramText pt) {
		System.out.println("Compiling a: " + this.langage + " program.");
		
		gen.generate(parser.parse(lexer.scan(pt)));

		System.out.println("Compilation finished");
	}
}
