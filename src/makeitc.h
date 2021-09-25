/*******************************************************************************************
*
*   makeitc - A simple and easy-to-use library to build pipelines in C
*
*   NOTES: 
*       Memory footprint of this library is aproximately xxx bytes (global variables)
*
*   CONFIGURATION:
*
*   #define MAKEITC_IMPLEMENTATION
*       Generates the implementation of the library into the included file.
*       If not defined, the library is in header only mode and can be included in other headers
*       or source files without problems. But only ONE file should hold the implementation.
*
*   #define MAKEITC_xxxx
*       If defined, the library can...
*
*   DEPENDENCIES:
*       None.
*
*   CONTRIBUTORS:
*       Ramon Santamaria:   Main developer, maintainer
*
*
*   LICENSE: MIT License
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*   
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*   
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*   
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#ifndef MAKEITC_H
#define MAKEITC_H

#ifndef MICAPI
    #define MICAPI   // We are building or using the library as a static library (or Linux shared library)
#endif

#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #define MICAPI __declspec(dllexport)         // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define MICAPI __declspec(dllimport)         // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

// Support TRACELOG macros
#ifndef TRACELOG
    #define TRACELOG(level, ...) (void)0
    #define TRACELOGD(...) (void)0
#endif

// Allow custom memory allocators
#ifndef MIC_MALLOC
    #define MIC_MALLOC(sz)     malloc(sz)
#endif
#ifndef MIC_CALLOC
    #define MIC_CALLOC(n,sz)   calloc(n,sz)
#endif
#ifndef MIC_REALLOC
    #define MIC_REALLOC(n,sz)  realloc(n,sz)
#endif
#ifndef MIC_FREE
    #define MIC_FREE(p)        free(p)
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

// TODO: Define the API here
MICAPI void micBeginProcess(const char *description);
MICAPI void micEndProcess(void);
MICAPI void micBeginStep(const char *description);
MICAPI void micEndStep(void);

#ifdef __cplusplus
}
#endif

#endif // MAKEITC_H


/***********************************************************************************
*
*   MAKEITC IMPLEMENTATION
*
************************************************************************************/

#if defined(MAKEITC_IMPLEMENTATION)

#include <stdlib.h>
#include <math.h>               // Required for: sinf(), cosf(), sqrtf()

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition (internal)
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module internal Functions Declaration
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
//...
