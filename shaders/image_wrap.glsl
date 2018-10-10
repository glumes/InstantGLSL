precision mediump float;

// 图像扭曲的操作
// 通过 x = x + t * sin(pi * x) 实现

const float PI = 3.1415926;

// 调整 uT 值改变扭曲效果
const float uT = 0.2;

varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;

void main(){

    vec2 st = textureCoordinate;

    vec2 xy = st;

    xy = 2. * xy -1.;

    xy += uT * sin(PI * xy);

    st = (xy + 1.) / 2.0;

    vec3 irgb = texture2D(inputImageTexture,st).rgb;

    gl_FragColor = vec4(irgb,1);
}
