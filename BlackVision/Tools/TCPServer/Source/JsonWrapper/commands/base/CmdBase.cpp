
#include "CmdBase.h"


CmdBase::CmdBase(void)
{
}


CmdBase::~CmdBase(void)
{
}

wstring CmdBase::ParseWstring(const wObject& obj, wstring key)
{
	
	const json_spirit::Pair_impl<json_spirit::Config_vector<wstring>>::Value_type &val = find_value( obj, key );
	
	if(val == -1)
	{
		return L"";
	}
	return val.get_str();
}