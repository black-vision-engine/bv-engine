#pragma once

#include "Assets/Asset.h"


namespace bv
{

class FakeAsset;
DEFINE_CONST_PTR_TYPE( FakeAsset )



// ***********************
//
class FakeAsset : public Asset, public std::enable_shared_from_this< FakeAsset >
{
    static const std::string		uid;
private:

    std::string             m_path;

protected:
    virtual VoidConstPtr        QueryThis		() const override;

public:
    explicit					FakeAsset	    ( const std::string & path );


    static FakeAssetConstPtr	Create			( const std::string & path );

    virtual const std::string &	GetUID			() const override;
    static const std::string &	UID				();

    const std::string &         GetPath         () const { return m_path; }

};





}	// bv

