#pragma once
/**
@File UseLogger.h

This file includes Logger and defines macros LOG_MODULE.
Macro is used to identify from which module logged message was sent, and is obligatory.

You can write for each module your own file like this or you can
just include Logger.h and define macro LOG_MODULE.


There're two macros which can be use to log message:
- LOG_MESSAGE
- LOG_MESSAGE_FILE_LINE

First logs message with timestamp, SeverityLevel and module name. Second
adds furthermore file nam and line number.

Too write mesage to log you can simply write:
LOG_MESSAGE( SeverityLevel::warning ) << "Message";
*/

#include "Tools/Logger/Logger.h"

#if defined(LOG_MODULE)
    #error LOG_MODULE already defined.
#endif

#define LOG_MODULE ModuleEnum::ME_LibEffect
