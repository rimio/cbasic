#ifndef BACKEND_H_
#define BACKEND_H_

#include <fstream>
#include <string>
#include "ilang/il-program.h"

//
// Backend interface
//
class Backend
{
public:
	Backend () { }
	virtual ~Backend () { }

	// Compile an intermediate language program
	virtual int compile (IlProgram *program, std::ofstream &file) = 0;

	// Get a backend instance for specified target
	static Backend *getBackend (std::string target);
};

#endif
