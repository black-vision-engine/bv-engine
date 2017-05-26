#pragma once

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "CoreDEF.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"

namespace bv {

class RectNodeBuilder
{
protected:

    model::BasicNodePtr         m_node;
    model::ITimeEvaluatorPtr    m_timeEvaluator;
    std::string                 m_shaderPluginUID;

    std::string                 m_transformPluginName;
    std::string                 m_rectPluginName;

    float                       m_w;
    float                       m_h;
    float                       m_t;

protected:

    RectNodeBuilder             ( model::ITimeEvaluatorPtr timeEvaluator, const std::string & shaderPluginUID, float w = 1.0f, float h = 1.0f, TimeType t = TimeType( 0.0 ) );

public:

    virtual ~RectNodeBuilder    ();

    void                        SetW                ( float w, TimeType t = TimeType( 0.0 ) );
    void                        SetH                ( float h, TimeType t = TimeType( 0.0 ) );

    void                        SetPosition         ( float x, float y, float z, TimeType t = TimeType( 0.0 ) );
    void                        SetRotation         ( float x, float y, float z, TimeType t = TimeType( 0.0 ) );
    void                        SetScale            ( float sx, float sy, float sz, TimeType t = TimeType( 0.0 ) );

    virtual model::BasicNodePtr CreateNode          ( const std::string & name, bool resetToBuilderDefaults = false );
    virtual void                SetDefaults         ();

    std::string                 TransformPluginName () const;
    std::string                 RectPluginName      () const;
    virtual std::string         ShaderPluginName    () const = 0;

    std::string                 WidthParamName      () const;
    std::string                 HeightParamName     () const;
    std::string                 TransformParamName  () const;

protected:

    model::IPluginPtr           GetTRansformPlugin  ();
    model::IPluginPtr           GetRectPlugin       ();
    model::IPluginPtr           GetShaderPlugin     ();

    model::BasicNodePtr         CreateNewNode       () const;

};

class SolidRectNodeBuilder : public RectNodeBuilder
{
private:

    glm::vec4 m_color;

public:

    SolidRectNodeBuilder    ( model::ITimeEvaluatorPtr timeEvaluator, const glm::vec4 & color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), float w = 1.0f, float h = 1.0f, TimeType t = TimeType( 0.0 ) );
    ~SolidRectNodeBuilder   ();

    void                    SetColor        ( float r, float g, float b, float a, TimeType t = TimeType( 0.0 ) );
    virtual void            SetDefaults     () override;

    virtual std::string     ShaderPluginName() const override;

    std::string             ColorParamName  () const;

};

class TexturedRectNodeBuilder : public RectNodeBuilder
{
private:

    std::string     m_textureFile;
    bool            m_hasAlpha;
public:

    TexturedRectNodeBuilder     ( model::ITimeEvaluatorPtr timeEvaluator, const std::string & textureFile, bool hasAlpha, float w = 1.0f, float h = 1.0f, TimeType t = TimeType( 0.0 ) );
    ~TexturedRectNodeBuilder    ();

    void    SetTextureFile      ( const std::string & textureFile, bool hasAlpha );

    virtual std::string         ShaderPluginName() const override;

    virtual model::BasicNodePtr CreateNode      ( const std::string & name, bool resetToBuilderDefaults = false ) override;

};

} //bv
