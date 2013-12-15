/*
 * Copyright 2013 by Philip N. Garner
 *
 * See the file COPYING for the licence associated with this software.
 *
 * Author(s):
 *   Phil Garner, October 2013
 */

#include <var>
#include <stdexcept>
#include <dlfcn.h>

vfile::vfile(const char* iType)
{
    // Initialise
    mVarFile = 0;
    mHandle = 0;
    char *error = dlerror();

    // Open the library
    var lib = "lib";
    lib += iType;
    lib += ".so";
    mHandle = dlopen(&lib, RTLD_LAZY);
    if ((error = dlerror()) != NULL)
        throw std::runtime_error(error);
    if (!mHandle)
        throw std::runtime_error("vfile::vfile(): dlopen failed");

    // Find the factory function
    void (*fileFactory)(varfile** oFile);
    *(void **)(&fileFactory) = dlsym(mHandle, "factory");
    if ((error = dlerror()) != NULL)
        throw std::runtime_error(error);

    // Run the dynamically loaded factory function
    (*fileFactory)(&mVarFile);
    if (!mVarFile)
        throw std::runtime_error("vfile::vfile(): factory() failed");
}


vfile::~vfile()
{
    // Destroy the handler and close the library
    if (mVarFile)
        delete mVarFile;
    dlclose(mHandle);
}
