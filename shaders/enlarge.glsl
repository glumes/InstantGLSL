varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;



vec2 enlargeFun(vec2 curCoord,vec2 circleCenter,float radius,float intensity,float curve){

    float currentDistance = distance(curCoord,circleCenter);

    {
        float weight = currentDistance / radius;
        weight = 1.0 - intensity * (1.0 - pow(weight,curve));

        // 如果 intensity 为 1.0， curve 为 2.0 ，简化之后
        // weight = pow(weight,2.0);

        weight = clamp(weight,0.0,1.0);

        curCoord = circleCenter + (curCoord - circleCenter) * weight;

    }

    return curCoord;

}


void main(){



    float dis = distance(vec2(gPosition.x,gPosition.y/uXY),vec2(centerPoint.x,centerPoint.y));

    if(dis < RADIUS){

        glFragColor = texture2D(vTexture,vec2(aCoordinate.x/2.0+0.25,aCoordinate.y/2.0 + 0.25));
    }

    highp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
    gl_FragColor = textureColor;
}