/*
 * Copyright 2014 by Philip N. Garner
 *
 * See the file COPYING for the licence associated with this software.
 *
 * Author(s):
 *   Phil Garner, December 2014
 */

#include "lube.h"
#include "lube/path.h"

int main()
{
    lube::module m("path");
    lube::path* p = lube::create(m, CMAKE_DIR);

    std::cout << "Dir:" << std::endl;
    var dir = p->dir();
    std::cout << dir << std::endl;

    std::cout << "Recursive Dir:" << std::endl;
    var rdir = p->rdir();
    std::cout << rdir << std::endl;

    std::cout << "Tree:" << std::endl;
    var tree = p->tree();
    std::cout << tree << std::endl;

    return 0;
}
