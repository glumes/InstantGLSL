precision mediump float;

varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;

void main() {
    vec2 offset0 = vec2(-1.0,-1.0);
    vec2 offset1 = vec2(0.0,-1.0);
    vec2 offset2 = vec2(1.0,-1.0);
    vec2 offset3 = vec2(-1.0,0.0);
    vec2 offset4 = vec2(0.0,0.0);
    vec2 offset5 = vec2(1.0,0.0);
    vec2 offset6 = vec2(-1.0,1.0);
    vec2 offset7 = vec2(0.0,1.0);
    vec2 offset8 = vec2(1.0,1.0);

    const float scaleFactor = 1.0;

    float kernelValue0 = 0.0;
    float kernelValue1 = -1.0;
    float kernelValue2 = 0.0;

    float kernelValue3 = -1.0;
    float kernelValue4 = 5.0;
    float kernelValue5 = -1.0;

    float kernelValue6 = 0.0;
    float kernelValue7 = -1.0;
    float kernelValue8 = 0.0;

    vec4 sum;

    vec4 cTemp0,cTemp1,cTemp2,cTemp3,cTemp4,cTemp5,cTemp6,cTemp7,cTemp8;

    cTemp0 = texture2D(inputImageTexture,textureCoordinate.st + offset0.xy/512.0);
    cTemp1 = texture2D(inputImageTexture,textureCoordinate.st + offset1.xy/512.0);
    cTemp2 = texture2D(inputImageTexture,textureCoordinate.st + offset1.xy/512.0);
    cTemp3 = texture2D(inputImageTexture,textureCoordinate.st + offset3.xy/512.0);
    cTemp4 = texture2D(inputImageTexture,textureCoordinate.st + offset4.xy/512.0);
    cTemp5 = texture2D(inputImageTexture,textureCoordinate.st + offset5.xy/512.0);
    cTemp6 = texture2D(inputImageTexture,textureCoordinate.st + offset6.xy/512.0);
    cTemp7 = texture2D(inputImageTexture,textureCoordinate.st + offset7.xy/512.0);
    cTemp8 = texture2D(inputImageTexture,textureCoordinate.st + offset8.xy/512.0);

    sum = kernelValue0*cTemp0 + kernelValue1*cTemp1 + kernelValue2*cTemp2 + kernelValue3*cTemp3  + kernelValue4*cTemp4 + kernelValue5*cTemp5 +kernelValue6*cTemp6 + kernelValue7*cTemp7 + kernelValue8*cTemp8;

    gl_FragColor = sum * scaleFactor;

}
