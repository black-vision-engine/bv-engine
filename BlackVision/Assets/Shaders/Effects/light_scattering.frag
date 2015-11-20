#version 400

layout (location = 0) out vec4 FragColor;

uniform float exposure;
uniform float decay;
uniform float density;
uniform float weight;
uniform vec2 lightPositionOnScreen;
uniform sampler2D firstPassTexture;
uniform float numSamples = 100 ;

in vec2 uvCoord;

void main()
{	
    vec2 deltaTextCoord     = vec2( uvCoord - lightPositionOnScreen.xy );
    vec2 textCoo            = uvCoord;
    deltaTextCoord         *= 1.0 /  numSamples * density;
    float illuminationDecay = 1.0;


    for( int i = 0; i < int(numSamples); i++ )
    {
             textCoo -= deltaTextCoord;
             vec4 s = texture2D( firstPassTexture, textCoo );
        
             s *= illuminationDecay * weight;

             FragColor += s;

             illuminationDecay *= decay;
     }
     
     FragColor *= exposure;
     FragColor = vec4(1,0,0,1);
}