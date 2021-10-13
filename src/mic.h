/*******************************************************************************************
*
*   mic (make-it-c) - A simple and easy-to-use library to build pipelines in C
*
*   NOTES:
*       Memory footprint of this library is aproximately xxx bytes (global variables)
*
*   CONFIGURATION:
*
*   #define MIC_IMPLEMENTATION
*       Generates the implementation of the library into the included file.
*       If not defined, the library is in header only mode and can be included in other headers
*       or source files without problems. But only ONE file should hold the implementation.
*
*   #define MIC_xxxx
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

#ifndef MIC_H
#define MIC_H

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
    #define TRACELOG(level, ...) micTraceLog(level, __VA_ARGS__)
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

#define MIC_STRING_STATIC_MAX_SIZE      2048


//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Trace log level
// NOTE: Organized by priority level
typedef enum {
    MIC_LOG_ALL = 0,        // Display all logs
    MIC_LOG_TRACE,          // Trace logging, intended for internal use only
    MIC_LOG_DEBUG,          // Debug logging, used for internal debugging, it should be disabled on release builds
    MIC_LOG_INFO,           // Info logging, used for program execution info
    MIC_LOG_WARNING,        // Warning logging, used on recoverable failures
    MIC_LOG_ERROR,          // Error logging, used on unrecoverable failures
    MIC_LOG_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    MIC_LOG_NONE            // Disable logging
} micTraceLogLevel;


typedef enum {
    MIC_ENV_INFO_OS,
    MIC_ENV_INFO_OS_VERSION,
    MIC_ENV_INFO_PLATFORM,
    MIC_ENV_INFO_MACHINE_NAME,
} micEnvInfo;

// Callbacks to hook some internal functions
typedef void (*micTraceLogCallback)(int logLevel, const char *text, va_list args);  // Logging: Redirect trace log messages

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

// Log System
MICAPI void micTraceLog(int logLevel, const char *text, ...);           // Show trace log messages (LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR...)
MICAPI void micSetTraceLogLevel(int logLevel);                          // Set the current threshold (minimum) log level
MICAPI void micSetTraceLogCallback(micTraceLogCallback callback);          // Set custom trace log

// Environment
MICAPI void micSetEnvironmentFlags(unsigned int flags);                 // Setup environment config flags
MICAPI void micSetEnvironmentPath(const char *path);                    // Set environment path (added to system PATH)
MICAPI const char *micGetEnvironmentInfo(int info);                     // Get environment info: enum micEnvInfo

// Processes execution
MICAPI void micBeginProcess(const char *description, int level);        // [!] Begin a new process -> Not sure yet how use this, maybe just for logging...
MICAPI void micEndProcess(void);                                        // End current process
MICAPI void micBeginStep(const char *description, int level);           // [!] Begin a new process step -> Not sure yet how use it
MICAPI void micEndStep(void);                                           // End current step

MICAPI int micExecuteCommand(const char *command, ...);                 // Execute command line command, parameters passed as additional arguments
MICAPI int micExecuteMIC(const char *micFile);                          // Compile and execute another mic file
//MICAPI void micPrintMessage(const char *message, ...);                // [!] Probably not required -> Just use printf()

// Timming
MICAPI void micInitTimer(void);                                         // [!] Initialize internal timer -> Support multiple timers?
MICAPI double micGetTime(void);                                         // Get elapsed time in seconds since micInitTimer()
MICAPI long micGetTimeStamp(void);                                      // Get current date and time (now)
MICAPI const char *micGetTimeStampString(long timestamp);               // [!] Get timestamp as a string -> format? or just let the user manage it?
MICAPI int micWaitTime(int milliseconds);                               // Wait (sleep) a specific amount of time

// File system: Edition
MICAPI int micMakeFile(const char *fileName);                           // Create an empty file, useful for further filling
MICAPI int micDeleteFile(const char *fileName);                         // Delete an existing file
MICAPI int micRenameFile(const char *fileName);                         // Rename an existing file
MICAPI int micCopyFile(const char *srcFileName, const char *dstPathFileName);   // Copy an existing file to a new path and filename
MICAPI int micMoveFile(const char *srcFileName, const char *dstpathFileName);   // Move an existing file to a new path and filename

MICAPI int micMakeDirectory(const char *dirPathName);                   // Create an empty directory
MICAPI int micDeleteDirectory(const char *dirPath);                     // Delete an existing and empty directory
MICAPI int micRenameDirectory(const char *dirPath);                     // Rename an existing directory
MICAPI int micCopyDirectory(const char *srcDirPath, const char *dstDirPath);    // Copy an existing directory to a new path
MICAPI int micMoveDirectory(const char *srcDirPath, const char *dstDirPath);    // Move an existing directory to a new path

// File system: Query
MICAPI bool micIsFileAvailable(const char *fileName);                   // Check if a file exists
MICAPI bool micIsDirectoryAvailable(const char *dirPath);               // Check if a directory path exists

MICAPI const char *micGetWorkingDirectory(void);                        // Get current working directory (uses static string)
MICAPI bool micChangeDirectory(const char *dirPath);                    // Change working directory, return true on success

MICAPI int micGetFileSize(const char *fileName);
MICAPI long micGetFileModTime(const char *fileName);                    // Get file modification time (last write time)
MICAPI bool micIsFileExtension(const char *fileName, const char *ext);  // Check file extension (including point: .png, .wav)
MICAPI const char *micGetFileExtension(const char *fileName);           // Get pointer to extension for a filename string (includes dot: '.png')
MICAPI const char *micGetFileName(const char *filePath);                // Get pointer to filename for a path string
MICAPI const char *micGetFileNameWithoutExt(const char *filePath);      // Get filename string without extension (uses static string)
MICAPI const char *micGetDirectoryPath(const char *filePath);           // Get full path for a given fileName with path (uses static string)
MICAPI const char *micGetPrevDirectoryPath(const char *dirPath);        // Get previous directory path for a given path (uses static string)
MICAPI const char *micGetFileFullPath(const char *fileName);            // Get full path for a file
MICAPI const char *micGetFileRelativePath(const char *fileName, const char *refPath);
MICAPI long micGetFileInfo(int info)    // MIC_FILE_INFO_TIME_CREATION, MIC_FILE_INFO_TIME_LAST_ACCESS, MIC_FILE_INFO_TIME_LAST_WRITE

MICAPI int micGetDirectorySize(const char *dirPath);                    // Get directory byte size (for all files contained)
MICAPI char **micGetDirectoryFiles(const char *dirPath, int *count);    // Get filenames in a directory path (memory should be freed)
MICAPI void micClearDirectoryFiles(void);                               // Clear directory files paths buffers (free memory)

// String management (no UTF-8 strings, only byte chars)
// NOTE: Some strings allocate memory internally for returned strings -> REVIEW
MICAPI int micStringCopy(char *dstStr, const char *srcStr);             // Copy one string to another, returns bytes copied
MICAPI bool micStringEqual(const char *str1, const char *str2);         // Check if two string are equal
MICAPI unsigned int micStringSize(const char *str);                     // Get string size in bytes, checks for '\0' ending
MICAPI const char *micStringFormat(const char *str, ...);               // String formatting with variables (sprintf() style)
MICAPI const char *micStringSubstring(const char *str, int position, int length);          // Get a piece of a string
MICAPI char *micStringReplace(char *str, const char *replace, const char *by);             // Replace string (WARNING: memory must be freed!)
MICAPI char *micStringInsert(const char *str, const char *insert, int position);           // Insert string in a position (WARNING: memory must be freed!)
MICAPI const char *micStringJoin(const char **strList, int count, const char *delimiter);  // Join strings with delimiter
MICAPI const char **micStringSplit(const char *str, char delimiter, int *count);           // Split string into multiple strings
MICAPI void micStringAppend(char *str, const char *append, int *position);                 // Append string at specific position and move cursor!
MICAPI int micStringFindIndex(const char *str, const char *find);       // Find first string occurrence within a string
MICAPI const char *micStringToUpper(const char *str);                   // Get upper case version of provided string
MICAPI const char *micStringToLower(const char *str);                   // Get lower case version of provided string
MICAPI const char *micStringToPascal(const char *str);                  // Get Pascal case notation version of provided string
MICAPI int micStringToInteger(const char *str);                         // Get integer value from string (negative values not supported)
MICAPI bool micStringContains(const char *str, const char *contain);    // Check if a string contains another string
MICAPI bool micStringStartsWith(const char *str, const char *start);    // Check if a string starts with another prefix string

// Misc functions
MICAPI bool micSaveStorageValue(unsigned int position, int value);      // Save integer value to storage file (to defined position), returns true on success
MICAPI int micLoadStorageValue(unsigned int position);                  // Load integer value from storage file (from defined position)

MICAPI int micGetRandomValue(int min, int max);                         // Get a random value between min and max (both included)
MICAPI void micSetRandomSeed(unsigned int seed);                        // Set the seed for the random number generator

MICAPI unsigned char *micLoadFileData(const char *fileName, unsigned int *bytesRead);     // Load file data as byte array (read)
MICAPI void micUnloadFileData(unsigned char *data);                     // Unload file data allocated by LoadFileData()
MICAPI bool micSaveFileData(const char *fileName, void *data, unsigned int bytesToWrite); // Save data to file from byte array (write), returns true on success
MICAPI char *micLoadFileText(const char *fileName);                     // Load text data from file (read), returns a '\0' terminated string
MICAPI void micUnloadFileText(char *text);                              // Unload file text data allocated by LoadFileText()
MICAPI bool micSaveFileText(const char *fileName, char *text);          // Save text data to file (write), string must be '\0' terminated, returns true on success

MICAPI int micZipFile(const char *srcFileName, const char *dstFileName);    // Compress file into a .zip
MICAPI int micZipDirectory(const char *srcPath, const char *dstFileName);   // Compress directory into a .zip

MICAPI unsigned char *micCompressData(unsigned char *data, int dataLength, int *compDataLength);        // Compress data (DEFLATE algorithm)
MICAPI unsigned char *micDecompressData(unsigned char *compData, int compDataLength, int *dataLength);  // Decompress data (DEFLATE algorithm)

#ifdef __cplusplus
}
#endif

#endif // MIC_H


/***********************************************************************************
*
*   MIC IMPLEMENTATION
*
************************************************************************************/

#if defined(MIC_IMPLEMENTATION)

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

// Log System
//----------------------------------------------------------------------------------

// Show trace log messages (LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR...)
void micTraceLog(int logLevel, const char *text, ...)
{

}

// Set the current threshold (minimum) log level
void micSetTraceLogLevel(int logLevel)
{

}

// Set custom trace log
void micSetTraceLogCallback(micTraceLogCallback callback)
{

}

// Environment
//----------------------------------------------------------------------------------

// Setup environment config flags
void micSetEnvironmentFlags(unsigned int flags)
{

}

// Set environment path (added to system PATH)
void micSetEnvironmentPath(const char *path)
{

}

// Get environment info: enum micEnvInfo
const char *micGetEnvironmentInfo(int info)
{

}

// Processes execution
//----------------------------------------------------------------------------------

// [!] Begin a new process -> Not sure yet how use this, maybe just for logging...
void micBeginProcess(const char *description, int level)
{

}

// End current process
void micEndProcess(void)
{

}

// [!] Begin a new process step -> Not sure yet how use it
void micBeginStep(const char *description, int level)
{

}

// End current step
void micEndStep(void)
{

}

// Execute command line command, parameters passed as additional arguments
int micExecuteCommand(const char *command, ...)
{

}

// Compile and execute another mic file
int micExecuteMIC(const char *micFile)
{

}

// Timming
//----------------------------------------------------------------------------------
// [!] Initialize internal timer -> Support multiple timers?
void micInitTimer(void)
{

}

// Get elapsed time in seconds since micInitTimer()
double micGetTime(void)
{

}

// Get current date and time (now)
long micGetTimeStamp(void)
{

}

// [!] Get timestamp as a string -> format? or just let the user manage it?
const char *micGetTimeStampString(long timestamp)
{

}

// Wait (sleep) a specific amount of time
int micWaitTime(int milliseconds)
{

}

// File system: Edition
//----------------------------------------------------------------------------------

// Create an empty file, useful for further filling
int micMakeFile(const char *fileName)
{

}

// Delete an existing file
int micDeleteFile(const char *fileName)
{

}

// Rename an existing file
int micRenameFile(const char *fileName, const char *newFileName)
{

}

// Copy an existing file to a new path and filename
int micCopyFile(const char *srcFileName, const char *dstPathFileName)
{

}

// Move an existing file to a new path and filename
int micMoveFile(const char *srcFileName, const char *dstpathFileName)
{

}

// Create an empty directory
int micMakeDirectory(const char *dirPathName)
{

}

// Delete an existing and empty directory
int micDeleteDirectory(const char *dirPath)
{

}

// Rename an existing directory
int micRenameDirectory(const char *dirPath)
{

}

// Copy an existing directory to a new path
int micCopyDirectory(const char *srcDirPath, const char *dstDirPath)
{

}

// Move an existing directory to a new path
int micMoveDirectory(const char *srcDirPath, const char *dstDirPath)
{

}

// File system: Query
//----------------------------------------------------------------------------------

// Check if a file exists
bool micIsFileAvailable(const char *fileName)
{

}

// Check if a directory path exists
bool micIsDirectoryAvailable(const char *dirPath)
{

}

// Get current working directory (uses static string)
const char *micGetWorkingDirectory(void)
{

}

// Change working directory, return true on success
bool micChangeDirectory(const char *dirPath)
{

}

// Get file size
int micGetFileSize(const char *fileName)
{

}

// Get file modification time (last write time)
long micGetFileModTime(const char *fileName)
{

}

// Check file extension (including point: .png, .wav)
bool micIsFileExtension(const char *fileName, const char *ext)
{

}

// Get pointer to extension for a filename string (includes dot: '.png')
const char *micGetFileExtension(const char *fileName)
{

}

// Get pointer to filename for a path string
const char *micGetFileName(const char *filePath)
{

}

// Get filename string without extension (uses static string)
const char *micGetFileNameWithoutExt(const char *filePath)
{

}

// Get full path for a given fileName with path (uses static string)
const char *micGetDirectoryPath(const char *filePath)
{

}

// Get previous directory path for a given path (uses static string)
const char *micGetPrevDirectoryPath(const char *dirPath)
{

}

// Get full path for a file
const char *micGetFileFullPath(const char *fileName)
{

}

// Get file path relative to another path
const char *micGetFileRelativePath(const char *fileName, const char *refPath)
{

}

// Get file info: MIC_FILE_INFO_TIME_CREATION, MIC_FILE_INFO_TIME_LAST_ACCESS, MIC_FILE_INFO_TIME_LAST_WRITE
long micGetFileInfo(const char *fileName, int info)
{

}

// Get directory byte size (for all files contained)
int micGetDirectorySize(const char *dirPath)
{

}

// Get filenames in a directory path (memory should be freed)
char **micGetDirectoryFiles(const char *dirPath, int *count)
{

}


// String management (no UTF-8 strings, only byte chars)
// NOTE: Some strings allocate memory internally for returned strings -> REVIEW
//----------------------------------------------------------------------------------

// Copy one string to another, returns bytes copied
int micStringCopy(char *dstStr, const char *srcStr)
{

}

// Check if two string are equal
bool micStringEqual(const char *str1, const char *str2)
{

}

// Get string size in bytes, checks for '\0' ending
unsigned int micStringSize(const char *str)
{

}

// String formatting with variables (sprintf() style)
const char *micStringFormat(const char *str, ...)
{

}

// Get a piece of a string
const char *micStringSubstring(const char *str, int position, int length)
{

}

// Replace string (WARNING: memory must be freed!)
char *micStringReplace(char *str, const char *replace, const char *by)
{

}

// Insert string in a position (WARNING: memory must be freed!)
char *micStringInsert(const char *str, const char *insert, int position)
{

}

// Join strings with delimiter
const char *micStringJoin(const char **strList, int count, const char *delimiter)
{

}

// Split string into multiple strings
const char **micStringSplit(const char *str, char delimiter, int *count)
{

}

// Append string at specific position and move cursor
void micStringAppend(char *str, const char *append, int *position)
{

}

// Find first string occurrence within a string
int micStringFindIndex(const char *str, const char *find)
{

}

// Get upper case version of provided string
const char *micStringToUpper(const char *str)
{

}

// Get lower case version of provided string
const char *micStringToLower(const char *str)
{

}

// Get Pascal case notation version of provided string
const char *micStringToPascal(const char *str)
{

}

// Get integer value from string (negative values not supported)
int micStringToInteger(const char *str)
{

}

// Check if a string contains another string
bool micStringContains(const char *str, const char *contain)
{

}

// Check if a string starts with another prefix string
bool micStringStartsWith(const char *str, const char *start)
{

}

// Misc functions
//----------------------------------------------------------------------------------

// Save integer value to storage file (to defined position), returns true on success
bool micSaveStorageValue(unsigned int position, int value)
{

}

// Load integer value from storage file (from defined position)
int micLoadStorageValue(unsigned int position)
{

}

// Get a random value between min and max (both included)
int micGetRandomValue(int min, int max)
{

}

// Set the seed for the random number generator
void micSetRandomSeed(unsigned int seed)
{

}

// Load file data as byte array (read)
unsigned char *micLoadFileData(const char *fileName, unsigned int *bytesRead)
{

}

// Unload file data allocated by LoadFileData()
void micUnloadFileData(unsigned char *data)
{

}

// Save data to file from byte array (write), returns true on success
bool micSaveFileData(const char *fileName, void *data, unsigned int bytesToWrite)
{

}

// Load text data from file (read), returns a '\0' terminated string
char *micLoadFileText(const char *fileName)
{

}

// Unload file text data allocated by LoadFileText()
void micUnloadFileText(char *text)
{

}

// Save text data to file (write), string must be '\0' terminated, returns true on success
bool micSaveFileText(const char *fileName, char *text)
{

}

// Compress file into a .zip
int micZipFile(const char *srcFileName, const char *dstFileName)
{

}

// Compress directory into a .zip
int micZipDirectory(const char *srcPath, const char *dstFileName)
{

}

// Compress data (DEFLATE algorithm)
unsigned char *micCompressData(unsigned char *data, int dataLength, int *compDataLength)
{

}

// Decompress data (DEFLATE algorithm)
unsigned char *micDecompressData(unsigned char *compData, int compDataLength, int *dataLength)
{

}

#endif   // MIC_IMPLEMENTATION
