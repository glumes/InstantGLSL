precision mediump float;
varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;

const float uT = 0.5;

void main(){

    highp vec3 textureColor = texture2D(inputImageTexture, textureCoordinate).rgb;

    vec3 target = vec3(0.5,0.5,0.5);

    gl_FragColor = vec4(mix(target,textureColor,uT),1.);
}

