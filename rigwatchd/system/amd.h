#ifndef RIGWATCH_DAEMON_AMD_H
#define RIGWATCH_DAEMON_AMD_H

#define __stdcall

#define RIGWATCH_AMD
#ifdef RIGWATCH_AMD

#include <wchar.h>
#include <stdbool.h>

#include <adl_defines.h>
#include <adl_sdk.h>
#include <adl_structures.h>

typedef int (*ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int);
typedef int (*ADL_MAIN_CONTROL_DESTROY)();
typedef int (*ADL_ADAPTER_NUMBEROFADAPTERS_GET)(int*);
typedef int (*ADL_ADAPTER_ADAPTERINFO_GET)(LPAdapterInfo, int);

#endif

void amd_init();

#endif //RIGWATCH_DAEMON_AMD_H
