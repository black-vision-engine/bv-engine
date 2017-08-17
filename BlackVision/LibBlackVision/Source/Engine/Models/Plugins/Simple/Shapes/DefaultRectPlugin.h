#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"


namespace bv { namespace model { namespace DefaultRect {


// *******************************
//
class PluginDesc : public DefaultGeometryPluginDescBase
{
public:

    PluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();

};

// *******************************
//
struct PN 
{

    static const std::string WIDTH;
    static const std::string HEIGHT;
    static const std::string WEIGHTCENTERX; // enum WeightCenter (MAX, MIN, CENTER)
	static const std::string WEIGHTCENTERY; // enum WeightCenter (MAX, MIN, CENTER)

};

// *******************************
//
class Plugin : public DefaultGeometryPluginBase
{
public:
    
    enum WeightCenter : int 
    { 
        MAX, 
        MIN, 
        CENTER 
    };

    ValueFloatPtr									m_width;
    ValueFloatPtr									m_height;
	std::shared_ptr< ParamEnum< WeightCenter >	>	m_weightCenterX;
	std::shared_ptr< ParamEnum< WeightCenter >	>	m_weightCenterY;

private:

    virtual std::vector< IGeometryGeneratorPtr >    GetGenerators       () override;
    virtual bool                                    NeedsTopologyUpdate () override;

public:

    Plugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );

};

// *******************************
//
class Generator : public IGeometryNormalsGenerator
{
private:

    float       m_width;
    float       m_height;
    glm::vec2   m_centerTranslate;

public:

                                        Generator               ( float w, float h, Plugin::WeightCenter wcx, Plugin::WeightCenter wcy );
    virtual void                        GenerateGeometryNormals ( Float3AttributeChannelPtr posChannel, Float3AttributeChannelPtr normChannel ) override;

private:

    glm::vec2                           GetWeightCenter     ( Plugin::WeightCenter centerX, Plugin::WeightCenter centerY );

};

} //DefaultRect

DEFINE_ENUM_SET_PARAMETER( DefaultRect::Plugin::WeightCenter );


} // model
} // bv
