#include "backend.h"
#include "backends/x86-nasm/x86-nasm-backend.h"
#include "error/error.h"

Backend *Backend::getBackend (std::string target)
{
	if (target.compare ("x86") == 0)
	{
		return new X86NasmBackend ();
	}
	else
	{
		Error::error ("no backend for target '" + target + "'");
		return nullptr;
	}
}
