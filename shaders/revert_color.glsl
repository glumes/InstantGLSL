precision mediump float;
varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;

// 反像
const float uT = 1.0;

void main(){

    highp vec3 textureColor = texture2D(inputImageTexture, textureCoordinate).rgb;
    vec3 neg = vec3(1.,1.,1.) - textureColor;

    gl_FragColor = vec4(mix(textureColor,neg,uT),1.);
}

