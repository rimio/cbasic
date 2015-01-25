#ifndef ERROR_H_
#define ERROR_H_

//
// Error codes
//
#define NO_ERROR				0
#define ER_FAILED				-1

//
// Errors handling routines gathered in a static class
//
class Error
{
private:
	static void reportError (const std::string error);

public:
	static void error (const std::string error);
	static void internalError (const std::string error);
	static void syntaxError (const std::string error);
};

#endif
