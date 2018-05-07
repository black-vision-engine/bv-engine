#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Simple/SpecialPlugins/BlendHelper.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultBlendTexturePluginDesc : public BasePluginDescriptor
{
public:

    DefaultBlendTexturePluginDesc                               ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();

};



/**@page BlendingModes Blending modes

@section BlendingModes_Modes Blending modes

There are two mechanisms for blending layers:
1. Engine blends objects while rendering to render targets. This mode can be set in mode of plugins texture, color, gradient. Since we use
OpenGL state to set blending equation, we can use only limited number of modes.
2. Engine can blend two textures in shader by using @ref DefaultBlendTexturePlugin "BlendTexturePlugin". In this case user can choose from
Photoshop-like blending modes listed below.

@subsection BlendingModes_PremultipliedAlpha Premutliplied alpha

User should load textures with color channels mulitplied by alpha value. The exception is <b>Alpha</b> mode introduced for compatibility
with not premultiplied textures.

Textures should be premultiplied, to proprly handle mipmapping in texels with alpha diffferent then 1. More about reasons to
use premultiplied alpha here: https://developer.nvidia.com/content/alpha-blending-pre-or-not-pre

@subsection BlendingModes_ModesSupport Support for blending modes

Most of blending equations is based on document: https://mouaif.wordpress.com/2009/01/05/photoshop-math-with-glsl-shaders/

- <b>Nonde</b> means no blending. New pixel color replaces old pixel color.
- <b>Alpha</b> is compatibility mode when user provides texture without premultiplied alpha. This mode multplies color by alpha value.

Blend mode name | BlendTexture support | Blend with Background support   | Effects support
--------------- | -------------------- | ------------------------------- | -----------------
Normal          | yes                  | yes                             | -
Lighten         | yes                  | no                              | -
Darken          | yes                  | no                              | -
Multiply        | yes                  | yes                             | -
Average         | yes                  | yes                             | -
Add             | yes                  | yes                             | -
Substract       | yes                  | no                              | -
Difference      | yes                  | no                              | -
Negation        | yes                  | no                              | -
Exclusion       | yes                  | no                              | -
Screen          | yes                  | no                              | -
Overlay         | yes                  | no                              | -
SoftLight       | yes                  | no                              | -
HardLight       | yes                  | no                              | -
ColorDodge      | yes                  | no                              | -
ColorBurn       | yes                  | no                              | -
LinearDodge     | yes                  | no                              | -
LinearBurn      | yes                  | no                              | -
LinearLight     | yes                  | no                              | -
VividLight      | yes                  | no                              | -
PinLight        | yes                  | no                              | -
HardMix         | yes                  | no                              | -
Reflect         | yes                  | no                              | -
Glow            | yes                  | no                              | -
Phoenix         | yes                  | no                              | -
Hue             | yes                  | no                              | -
Saturation      | yes                  | no                              | -
Color           | yes                  | no                              | -
Luminosity      | yes                  | no                              | -
None            | yes                  | yes                             | -
Alpha           | yes                  | yes                             | -
*/


/**@brief Blends texture with advanced blending equations.

This plugin should follow @ref DefaultTexturePlugin "TexturePlugin". BlendTexture plugin can load additional
texture which can be blended using Photoshop blending modes. More about blending modes: @ref BlendingModes "Blending Modes".

<b>Name:</b> blend texture

<b>Name abbreviation:</b> blend

<b>UID:</b> DEFAULT_BLEND_TEXTURE


@subsection DefaultBlendTexturePlugin_Parameters Parameters

Parameter name         	| Type      	| Default Value     | Description
----------------------- | -------------	| ----------------- | -----------
alpha                   | float         | 1.0f              | Alpha to attenuate loaded texture
blendMode               | enum BlendMode| Normal            | Mode used to blend two textures
txBlendMat              | transform     |                   | Asset transformation applied only to texture loaded by this plugin.

@subsection DefaultBlendTexturePlugin_Assets Assets

- <b>BlendTex0</b>

Texture to blend.


@ingroup PluginsList
@todo Inherit from bv::model::TexturePluginBase.*/
// ***************************** PLUGIN ********************************** 

class DefaultBlendTexturePlugin : public BasePlugin
{
public:

    struct PARAMS
    {
        static const std::string        ALPHA;
        static const std::string        BLENDING_MODE;
    };

protected:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    SizeType	                    m_textureWidth;
    SizeType	                    m_textureHeight;

public:

    explicit                                    DefaultBlendTexturePlugin   ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultBlendTexturePlugin  ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    SizeType									GetTextureWidth             () const;
    SizeType									GetTextureHeight            () const;

    virtual void                                Update                      ( TimeType t ) override;

private:
    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;
};


} // model
} // bv


