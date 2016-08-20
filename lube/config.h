/*
 * Copyright 2015 by Philip N. Garner
 *
 * See the file COPYING for the licence associated with this software.
 *
 * Author(s):
 *   Phil Garner, May 2015
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <lube/var.h>

namespace libube
{
    /**
     * Option class.  A thin (but partial to pies) wrapper for getopt() from
     * the standard unix library.  getopt_long() is a GNU extension, so I don't
     * use it.
     */
    class Option
    {
    public:
        Option();
        Option(var iName);
        Option(int iArgc, char** iArgv, var iOptString=nil);
        operator bool();
        int get() const { return mOpt; }; /** Put this call in the switch() */
        var operator [](char iChar) { return mOpts[var(iChar)]; };
        ind index() const;
        var arg() const;
        var args();
        void operator ()(char iChar, var iDescription, var iDefault=nil);
        void operator ()(var iDescription);
        var parse(int iArgc, char** iArgv);
        void usage();
        void usage(int iError) { usage(); exit(iError); };
    private:
        var mName;
        int mArgc;
        char** mArgv;
        var mOptString;
        int mOpt;   // The option returned by getopt()
        var mOpts;  // Associative array of options
        var mUsage; // Array of text descriptions with defaults
    };

    /**
     * Configuration class.  Stores an associative array with attribute
     * information.  The string is used to identify the attributes in, say, an
     * ini file.
     */
    class Config
    {
    public:
        Config(var iStr=nil);
        void read(var iConfigFile);
    protected:
        var config();
        var mStr;
    private:
        var mCnf;
    };
}

#endif // CONFIG_H
