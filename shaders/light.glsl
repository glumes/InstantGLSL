precision mediump float;
varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;

const float uT = 0.5;

void main(){

    highp vec3 textureColor = texture2D(inputImageTexture, textureCoordinate).rgb;

    vec3 black = vec3(0.,0.,0.);

    gl_FragColor = vec4(mix(black,textureColor,uT),1.);
}

