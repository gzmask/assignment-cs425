// stdafx.h : What is this?
//               This is a "precompiled header" file.
//
//            How do I use this?
//               Include, in this file, header files of data/functions that
//               you need for in main "CPP_template.cpp" file, for example
//				 "<math.h>".
//
//            Why can't I just directly include these header files in my
//            main "CPP_template.cpp" file?
//               EVERY time you compile your main "CPP_template.cpp" file,
//               ALL of the header files included in the main
//               "CPP_template.cpp" file are RECOMPILED, a slow and
//               needless process. But, if you include your header files
//               in this "stdafx.h" file, they will only be recompiled if
//               you change this "stdafx.h" file. Since you will not often
//               change this "stdafx.h" file, the compiler will not have to
//               recompile these included header files very often, speeding
//               up the overall compilation process.
//
//            Note:
//               This precompiled header "stdafx.h" file was created using
//               Microsoft Visual Studio 2008. It is intended for students
//               coding in Microsoft Visual Studio. It should NOT be used if
//               you are working in UNIX or with some other development tool,
//               such as CodeWarrior or Borland C++.
//

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <atlimage.h>	/* allows us to use CImage class */
#include <math.h>
#include <stdio.h>

//
// TODO: Include any additional header files that you need here.
//
