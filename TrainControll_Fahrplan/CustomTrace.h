#pragma once

#ifdef _DEBUG

bool _trace(TCHAR *format, ...);

#ifdef TRACE
#undef TRACE
#endif

#define TRACE _trace

#endif

