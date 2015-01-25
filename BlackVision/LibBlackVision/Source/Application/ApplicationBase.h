#pragma once

namespace bv {
	
class ApplicationBase
{
protected:

	//So that it can only get instantiated by a derived class
	ApplicationBase				();

public:

	virtual ~ApplicationBase	();

	static ApplicationBase * ApplicationInstance;

	typedef int (*ApplicationMain)( int, char ** );

	static ApplicationMain MainFun;

protected:

	template< typename Derived >
	static int MainImpl( int argc, char ** argv )
	{
		return static_cast< Derived * >( ApplicationBase::ApplicationInstance )->MainFun( argc, argv );
	}

};

} //bv
