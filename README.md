## 学习 [LearnOpenGL](https://learnopengl-cn.github.io/) 时的练习代码

编译运行：`make && ./openglTest.0.0.1`

```
namespace framework {
    class IPencil;  // 画笔接口，把每个练习实现为一个画笔
    sigleton Framework;  // 调用画笔
    class CameraMove;  // 相机移动
}

namespace camera {
    singleton Camera;  // 相机
}

class Shader;  // 编译&链接 glsl，设置 uniform
```

```
/src/
    /practice/  画笔代码
    /util/  几个辅助函数代码
/include/  头文件
/lib/  库文件
/Resources/
    /shaders/  glsl 文件
    /texture/  图片资源
```

