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

	void						Save( const std::string & filename );
	void						Save( std::ostream & out );

	void						SetName( const std::string & name );
	void						SetValue( const std::string & name, const std::string & value );
	void						SetContent( const std::string & value );
	void						Pop();
};

class JsonDeserializeObject : public ISerializer
{
private:
	JsonDeserializeObjectImpl*		pimpl_;
public:
	JsonDeserializeObject( const std::string& jsonString );
	JsonDeserializeObject( std::istream& stream );
	~JsonDeserializeObject();

	std::string					GetName();
	std::string					GetValue( std::string name ) const;

	bool						Push( const std::string& name );
	void						Pop();
};

} //bv
