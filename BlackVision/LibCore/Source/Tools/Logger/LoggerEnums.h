#pragma once

namespace bv
{

enum SeverityLevel : int
{
	debug			= 0,
	info			= 1,
	warning			= 2,
	error			= 3,
	critical		= 4
};

enum ModuleEnum : int
{
	ME_LibBlackVision	= 1 << 0,
	ME_LibCore			= 1 << 1,
	ME_LibImage			= 1 << 2,
	ME_Prototyper		= 1 << 3,
	ME_BlackVisionApp	= 1 << 4,
	ME_LibProjectManager= 1 << 5,
    ME_TCPServer        = 1 << 6,
    ME_LibVideoCards    = 1 << 7,
    ME_XMLScenParser    = 1 << 8,
    ME_Tests            = 1 << 9,
};

/**Adding new modules - instruction:
- Add new constant to ModuleEnum
- Add string with the name of module in InitializeModuleMapping function (Logger.cpp file).
- Uncomment line with module number in function SetFilter. Otherwise all messages from new module
will be filtered (Logger.cpp file).*/

} //bv
