//  Created by SelFree on 2019/2/26.
//  Copyright © 2019年 SelFree. All rights reserved.

#include <functional>
#include <vector>

#include "src/common/event.h"
#include "src/framework.h"

#include "src/practice/01_getting_started/01_triangle.h"
#include "src/practice/01_getting_started/01_triangle_indexed.h"
#include "src/practice/01_getting_started/02_texture_test.h"
#include "src/practice/01_getting_started/03_transformations.h"
#include "src/practice/01_getting_started/04_coordinate.h"
#include "src/practice/01_getting_started/04_coordinate_box.h"
#include "src/practice/01_getting_started/05_camera.h"

#include "src/practice/02_lighting/01_colors.h"
#include "src/practice/02_lighting/02_basic_lighting.h"
#include "src/practice/02_lighting/03_materials.h"
#include "src/practice/02_lighting/04_lighting_map.h"
#include "src/practice/02_lighting/05_point_light.h"
#include "src/practice/02_lighting/05_spotlight.h"
#include "src/practice/02_lighting/06_multiple_lights.h"

#include "src/practice/03_model_loading/01_model_loading.h"

namespace main_space {

std::vector<framework::IPencil*> pencils;

int index = 0;

}  // namespace main_space

// 左右方向键键切换画笔
void keyCallback(GLFWwindow* window, int key, int scan_code, int event, int d) {
    if (event != GLFW_RELEASE) {
        return;
    }

    switch (key) {
    case GLFW_KEY_LEFT:
        --main_space::index;
        break;
    case GLFW_KEY_RIGHT:
        ++main_space::index;
        break;
    default:
        return;
    }

    if (main_space::index < 0) {
        main_space::index = main_space::pencils.size() - 1;
    } else if (main_space::index >= main_space::pencils.size()) {
        main_space::index = 0;
    }

    glfwSetWindowTitle(window, typeid(*main_space::pencils[main_space::index]).name());
    framework::Instance()->set_pencil(main_space::pencils[main_space::index]);
}

int main() {
    if (framework::Instance()->Init("test")) {
        glEnable(GL_DEPTH_TEST);  // 开启景深

        main_space::pencils = std::vector<framework::IPencil*>{
            new getting_started::Triangle(),
            new getting_started::TriangleIndexed(),
            new getting_started::TextureTest(),
            new getting_started::Transformation(),
            new getting_started::Coordinate(),
            new getting_started::CoordinateBox(),
            new getting_started::Camera(),

            new lighting::Colors(),
            new lighting::BasicLighting(),
            new lighting::Materials(),
            new lighting::LightingMap(),
            new lighting::PointLight(),
            new lighting::Spotlight(),
            new lighting::MultipleLights(),

            new model_loading::ModelLoading(),
        };

        for (auto v : main_space::pencils) {
            v->Init();
        }

        // auto delegate = framework::ProcessInputDelegate<void>(processInput);
        // framework::Instance()->AppendProcessInputDelegate(&delegate);

        glfwSetKeyCallback(framework::Instance()->get_window(), keyCallback);

        glfwSetWindowTitle(framework::Instance()->get_window()
            , typeid(*main_space::pencils[main_space::index]).name());
        framework::Instance()->set_pencil(main_space::pencils[main_space::index]);
        framework::Instance()->Start();
    }
}
