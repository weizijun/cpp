#ifndef __PP_BASE_H__
#define __PP_BASE_H__

#if (defined(WIN32) || defined(WIN64))

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:

#include <winsock2.h>
#include <windows.h>
#ifndef PPAPI
#define PPAPI __stdcall
#endif

//////////////////////////////////////////////////////////////////////////////////////////

#ifndef WINVER                          // Specifies that the minimum required platform is Windows XP.
#define WINVER 0x0501           // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows XP.
#define _WIN32_WINNT 0x0501     // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#if (defined(_WIN32_WINNT_VISTA) || defined(_WIN32_WINNT_WIN6) || defined(_WIN32_WINNT_WS08) || defined(_WIN32_WINNT_LONGHORN) || defined(_WIN32_WINNT_WIN7))
//#define _PPCOOL_USE_CONDITION_VARIABLE
#endif

#else

#ifndef PPAPI
#define PPAPI
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef NULL
#include <cstddef>
#endif

#include <xlocale.h>
#include <unistd.h>
#include <iostream>

#endif


#endif
