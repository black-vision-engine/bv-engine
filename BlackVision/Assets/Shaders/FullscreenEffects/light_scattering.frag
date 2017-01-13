#version 400

layout (location = 0) out vec4 FragColor;

uniform float exposure;
uniform float decay;
uniform float density;
uniform float weight;
uniform vec2 lightPositionOnScreen;
uniform sampler2D Tex0;
uniform int numSamples;

in vec2 uvCoord;

void main()
{	
    vec2 deltaTextCoord     = vec2( uvCoord - lightPositionOnScreen );
    vec2 textCoo            = uvCoord;
    deltaTextCoord         *= 1.0 /  numSamples * density;
    float illuminationDecay = 1.0;

    if( length(lightPositionOnScreen - uvCoord) < 0.02 )
    {
        FragColor = vec4(1,0,0,1);
        return;
    }
    
    FragColor = texture( Tex0, uvCoord );

    for( int i = 0; i < numSamples; i++ )
    {
             textCoo -= deltaTextCoord;
             vec4 s = texture( Tex0, textCoo );
        
             s *= illuminationDecay * weight;

             FragColor += s;

             illuminationDecay *= decay;
     }
     
     FragColor *= exposure;
}