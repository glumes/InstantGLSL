precision mediump float;
varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;

const float uT = 0.0;

// 调节对比度
const vec3 w = vec3(0.2125,0.7154,0.0721);

void main(){

    highp vec3 textureColor = texture2D(inputImageTexture, textureCoordinate).rgb;

    float luminance = dot(textureColor,w);
    vec3 target = vec3(luminance,luminance,luminance);

    gl_FragColor = vec4(mix(target,textureColor,uT),1.);
}

