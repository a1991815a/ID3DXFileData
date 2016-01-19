#ifndef __GAMEMACRO__
#define __GAMEMACRO__
#include <crtdbg.h>
#include <stdexcept>

#define SAFE_RELEASE_COM(p) do{if(p) p->Release(); p = nullptr;} while(0)
#define SAFE_DELETE(p) do{if(p) delete p; p = nullptr; } while(0)
#define SAFE_DELETE_ARRAY(p) do{if(p) delete[] p; p = nullptr; } while(0)

#define GASSERT(cond, msg, ...) \
	if(!(cond)) \
	_CrtDbgReport(\
	_CRT_ASSERT, \
	__FILE__, \
	__LINE__, \
	nullptr, \
	msg, ##__VA_ARGS__)
#define GSTATICASSERT(cond, msg) static_assert(cond, msg) 
#define GTHROW_RUNTIME(cond, msg) while(!(cond)) throw std::runtime_error(msg)

#define GMOVE_POINTER(p, bytes) p = (decltype(p))((char*)p + bytes)


#endif