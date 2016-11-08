#pragma once

//#define DISABLE_PRECOMPILED_HEADER

#ifndef DISABLE_PRECOMPILED_HEADER



#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4127)
#pragma warning(disable : 4503)

#pragma warning( disable : 4512 )
// warning: could not generate contructor for...
#pragma warning( disable : 4100 )
// warning: unreferenced formal parameter x


#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"

#include "boost/filesystem/operations.hpp"

#include "boost/log/sources/severity_channel_logger.hpp"
#include "boost/log/sources/record_ostream.hpp"
#include "boost/log/sinks/async_frontend.hpp"


//#pragma warning( pop )

#include "CoreDEF.h"
#include "Expected.h"

#include "Tools/Base64.h"
#include "Tools/StringHeplers.h"

#include "System/Path.h"
#include "IO/DirIO.h"
#include "IO/FileIO.h"


#include "UseLoggerLibCoreModule.h"


#include <cstdlib>
#include <cassert>
#include <cstdarg>

#include <mutex>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <iomanip>


#include <sys/stat.h>
#include "win_sock.h"

#endif

#undef near
#undef far
#undef max
#undef min
#undef LoadImage
#undef LoadImageW

