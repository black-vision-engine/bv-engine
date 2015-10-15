#pragma once

#include <string>
#include <iostream>
#include "Serialization\ISerializer.h"

namespace bv
{

class JsonDeserializeObjectImpl;
class JsonSerializeObjectImpl;


class JsonSerializeObject : public ISerializer
{
private:
	JsonSerializeObjectImpl*		pimpl_;
public:
	JsonSerializeObject();
	~JsonSerializeObject();

	void						Save                ( const std::string & filename );
	void						Save                ( std::ostream & out );

	void						SetAttribute        ( const std::string & name, const std::string & value ) override;
    std::string                 GetAttribute        ( const std::string & name );

    bool						EnterChild          ( const std::string & name ) override;
	bool						ExitChild           () override;
};


} //bv
