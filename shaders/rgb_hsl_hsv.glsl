varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;



vec3 rgb2hsv(vec3 rgb){
    float h = 0.0;
    float s = 0.0;
    float v = 0.0;

    float r = rgb.x;
    float g = rgb.y;
    float b = rgb.z;

    float min = min(min(r,g),b);
    float max = max(max(r,g),b);

    float delta = max - min;

    // 先从 v 的值下手，hsv 中 v 就等于 max
    v = max ;

    // 再从 s 的值下手
    if(max != 0.0){
        s = delta / max ;
    }else{
        s = 0.0;
    }

    // 最后计算 h , 这里的 2.0 和 4.0 最后用来乘以 60 得到度数
    if(s == 0.0)
        h = 0.0;
    else if( r == max)
        h = (g - b) / delta;
    else if ( g == max )
        h = 2.0 + (b - r) / delta;
    else if ( b == max )
        h = 4.0 + (r - g) / delta;

    h = h * 60.0;
    if ( h < 0.0){
        h += 360.0;
    }
    return vec3(h,s,v);
}


vec3 hsv2rgb(vec3 hsv){

    float h = hsv.x;
    float s = hsv.y;
    float v = hsv.z;

    // 先从 s 下手
    if ( s == 0.0){
        return vec3(v,v,v);
    }else {
        if( h > 360.0)
            h = 360.0;
        if ( h < 0.0)
            h = 0.0;
        h /= 60.0;
        int k = int(h);
        float f = h - float(k);
        float p = v * (1.0 -s);
        float q = v * (1.0 - (s * f));
        float t = v * (1.0 - (s * (1.0 - f)));

        if(k == 0)
            return vec3(v,t,p);
        if(k == 1)
            return vec3(q,v,p);
        if(k == 2)
            return vec3(p,v,t);
        if(k == 3)
            return vec3(p,q,v);
        if(k == 4)
            return vec3(t,p,v);
        if(k == 5)
            return vec3(v,p,q);
    }
}

void main(){

    highp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
    gl_FragColor = textureColor;
}