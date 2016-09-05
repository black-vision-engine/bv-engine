#pragma once


#include "Engine/Models/Interfaces/INodeLogic.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/BasicNode.h"

#include "Assets/Font/FontAssetDescriptor.h"

namespace bv { namespace model
{

class TextEffects;

DEFINE_PTR_TYPE( TextEffects )
DEFINE_CONST_PTR_TYPE( TextEffects )

class TextEffects : public model::INodeLogic, public std::enable_shared_from_this< TextEffects >
{
private:
    static const std::string        m_type;

    BasicNodePtr                    m_node;

    BasicNodePtr                    m_shadowNode;

    DefaultParamValModelPtr         m_paramValModel;

    UInt32                          m_blurSize;

    explicit                        TextEffects     ( const BasicNodePtr & node );

public:
                                    ~TextEffects    ();

    static const std::string &      Type            ();


	virtual void					Initialize		() override;
	virtual void					Update			( TimeType t ) override;
	virtual void					Deinitialize	() override;

    virtual IParameterPtr                           GetParameter        ( const std::string & name ) const override;
    virtual const std::vector< IParameterPtr > &    GetParameters       () const override;

    virtual const std::string &     GetType         () const override;
    virtual bool                    HandleEvent     ( IDeserializer & eventStr, ISerializer & response, BVProjectEditor * editor ) override;

    virtual void                    Serialize       ( ISerializer& ser ) const override;

    static TextEffectsPtr           Create          ( const BasicNodePtr & node );

    static TextEffectsPtr           Create          ( const IDeserializer & deser, const BasicNodePtr & node );

private:

    IPluginPtr                      GetTextPlugin           () const;
    IPluginPtr                      GetShadowTextPlugin     () const;

    FontAssetDescConstPtr           GetFontAssetDesc        () const;
    FontAssetDescConstPtr           GetShadowFontAssetDesc  () const;
    bool                            ReloadShadowNodeAsset   () const;
    bool                            ShadowAssetIsValid      () const;
};

} // model
} // bv