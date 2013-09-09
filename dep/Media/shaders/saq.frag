uniform sampler2D Texture0;

varying vec2 texCoord;

void main(void)
{
    gl_FragColor = texture2D( Texture0, texCoord );
}
