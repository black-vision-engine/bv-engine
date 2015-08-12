#pragma once

#include "Logger.h"

#if defined(LOG_MODULE)
    #error LOG_MODULE already defined.
#endif

#define LOG_MODULE ModuleEnum::LibCore
