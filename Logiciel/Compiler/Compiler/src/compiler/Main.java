package compiler;

import compiler.text.*;
import compilerJava.*;

public class Main {

	public static void main(String[] args) {
		try{
		System.out.println("-----------------------");
		Compiler c1 = new Compiler("Java");
		c1.compile(new ProgramText("..."));
		System.out.println("-----------------------");
		Compiler c2 = new Compiler ("C++");
		c2.compile(new ProgramText("..."));
		}
		catch(Exception e){System.out.println(e.getMessage());}}

}
