varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;

// 图像的卷积操作，不同的卷积核的图像效果不同

void main(){

    // width 和 height 为图片的宽高
    float width = 512.0;
    float height = 512.0;

    // delta 表示在 纹理坐标的 UV 上移动
    // 找到当前片段的上下左右相邻片段的间隔
    float delta_x = 1.0 / width;
    float delta_y = 1.0 / height;

    // 选用的卷积
    float factor[9];
    factor[0] = 0.0; factor[1] = 0.0; factor[2] = 0.0;
    factor[3] = 0.0; factor[4] = 1.0; factor[5] = 0.0;
    factor[6] = 0.0; factor[7] = 0.0; factor[8] = 0.0;

    vec4 color = vec4(0.0);

    // 卷积计算过程
    for (int i = -1; i<=1 ; i++){
        for (int j = -1;j <= 1; j++){
            float x = max(0.0,textureCoordinate.x + float(i) * delta_x);
            float y = max(0.0,textureCoordinate.y + float(i) * delta_y);
            color += texture2D(inputImageTexture,vec2(x,y)) * factor[(i+1) * 3 + (j+1)];
        }
    }

    // 透明度直接用不透明的
    gl_FragColor = vec4(vec3(color),1.0);
}