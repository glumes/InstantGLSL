varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;
void main(){

    highp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
    gl_FragColor = textureColor;
}