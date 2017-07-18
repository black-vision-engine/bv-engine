#pragma once

#include "Assets/AssetDescriptor.h"

#include <string>



namespace bv
{

class FakeDescriptor;
DEFINE_CONST_PTR_TYPE( FakeDescriptor )


// ***********************
//
class FakeDescriptor : public AssetDesc, public std::enable_shared_from_this< AssetDesc >
{
private:

    static const std::string						uid;

    std::string             m_path;

public:

    explicit                        FakeDescriptor              ( const std::string & path );

    virtual void                    Serialize                   ( ISerializer & ser ) const override;
    virtual VoidConstPtr            QueryThis                   () const override;
    virtual const std::string &     GetUID                      () const override;
    virtual bool                    IsCacheable                 () const override;
    virtual SizeType                EstimateMemoryUsage         () const override;

    virtual std::string				GetKey				        () const override;
    virtual std::string             GetProposedShortKey         () const override;

    static const std::string &      UID					        ();

};



}	// bv