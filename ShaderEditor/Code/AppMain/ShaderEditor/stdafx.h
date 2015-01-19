// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef STD_AFX_H
#define STD_AFX_H

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>

#ifdef _WIN64
#pragma warning( disable : 4244 ) //warning C4244: 'return' : conversion from 'const gint64' to 'int', possible loss of data
#endif

// TODO: reference additional headers your program requires here

// Warning C4099: 'Gtk::PaperSizeTraits' : type name first seen using 'class' now seen using 'struct'
#pragma warning( disable : 4099 )

#endif //STD_AFX_H