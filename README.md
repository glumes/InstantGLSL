# InstantGLSL

一个快速编写，即时显示 GLSL 效果的项目。

通过该项目，在 Android 设备上进行 OpenGL ES 的 Shader 相关开发时，不需要每次编写完着色器脚本 GLSL 后运行应用看效果，而是即时显示着色器效果。

在 PC 上编写 GLSL 脚本，借助一些编辑器对 GLSL 语法的高亮、自动补全或错误提示，从此加快对 GLSL 的学习和开发。

## 使用

1. 在 Android 设备上运行 InstantGLSL 应用。

2. 在 PC 上通过 ADB 连接到设备。

3. 在 PC 的编辑器上编写 GLSL 脚本。

4. 运行 Python 程序 shaderwatch.py ，监听 GLSL 脚本的变化。

5. GLSL 脚本编辑结束，点击保存，在 Android 看到显示效果。


## 版本

libpng 1.6.37

## 待做 

- [ ] GLSL 编译失败提示
- [ ] 支持在相机预览时改变 GLSL，即时响应
- [ ] 支持 OpenGL ES 3.x 版本的 GLSL
- [ ] 提供更多内置变量 attribute、uniform 之类


