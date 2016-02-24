
/* 
 * File:   main.cpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 3:12 PM
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <cstring>
#include <cerrno>

#include "parser.hpp"
#include "lexer.hpp"
#include "compiler.hpp"
#include "machine.hpp"
#include "proc.hpp"
#include "debugger.hpp"

using namespace std;
using namespace ccl;

static const bool DEBUG = false;
static const char* FILE_INPUT = NULL; //"dist/Debug/Cygwin-Windows/indexer.ccl";

int main(int argc, char** argv) {
	if (FILE_INPUT) {
		argv = (char**) malloc(3 * sizeof(char*));
		argv[1] = (char*) FILE_INPUT;
	}
	
	procs::register_base_procs();
	types::register_proc_hooks();
	
	machine m = machine();
	
	string file_input;
	bool debug = false;
	
	for (int argi = 1; argi < argc; argi++) {
		string arg = string(argv[argi]);
		if (arg.compare("-d") == 0) {
			debug = true;
		} else {
			file_input = arg;
		}
	}
	
	if (!file_input.empty()) {
		ifstream f;
		errno = 0;
		f.open(argv[1]);
		if (f.fail()) {
			cerr << "ERROR: file " << file_input << ": " << strerror(errno) << endl;
			return EXIT_SUCCESS; //EXIT_FAILURE;
		}
		string s; string buf;
		while (!f.eof()) {
			errno = 0;
			getline(f, buf);
			if (f.fail()) {
				cerr << "ERROR: file " << file_input << ": " << strerror(errno) << endl;
				return EXIT_SUCCESS; //EXIT_FAILURE;
			}
			s += buf + '\n';
		}
		f.close();
		
		try {
			m.load(compile(lex(parse(argv[1], s))));
			if (DEBUG || debug) {
				begin_debugger(s, &m);
				return EXIT_SUCCESS;
			}
			
			m.run();
		} catch (exception& e) {
			cerr << "ERROR: " << e.what() << endl;
			return EXIT_SUCCESS; //EXIT_FAILURE;
		}
		
		return EXIT_SUCCESS;
	}
	
	while (true) {
		cout << "ccl> ";
		string s;
		getline(cin, s);
		s += "\n | print";
		
		try {
			list<token*>* tokens = parse("<input>", s);
			list<instruction*>* code = lex(tokens);
			program* p = compile(code);
			m.load(p);
			
			if (DEBUG || debug) {
				begin_debugger(s, &m);
				return EXIT_SUCCESS;
			}
			
			m.run();
		} catch (exception& e) {
			cerr << "ERROR: " << e.what() << endl;
		}
		
		//nuke the callstack so we can continue interactive input, even if error occured
		m.callstack = list<executor*>();
		m.traps = list<machine_trap*>();
	}
	
	return EXIT_SUCCESS;
}