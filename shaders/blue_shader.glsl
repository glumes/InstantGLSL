varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;
void main(){

    highp float red = 0.0;
    highp float green = 0.0;
    highp float blue = 1.0;
    highp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
    gl_FragColor = vec4(textureColor.r * red, textureColor.g * green, textureColor.b * blue, 1.0);
}
