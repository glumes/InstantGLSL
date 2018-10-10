
// 通过着色器将图像上下翻转

varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;
void main(){

    vec2 st = textureCoordinate;
    st.t = 1.0 - st.t;
    highp vec4 textureColor = texture2D(inputImageTexture, st);
    gl_FragColor = textureColor;
}


