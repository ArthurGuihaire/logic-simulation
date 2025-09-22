#include <GLFW/glfw3.h>
namespace Key {
    constexpr int forward = GLFW_KEY_W;
    constexpr int backward = GLFW_KEY_S;
    constexpr int left = GLFW_KEY_A;
    constexpr int right = GLFW_KEY_D;
    constexpr int down = GLFW_KEY_LEFT_SHIFT;
    constexpr int up = GLFW_KEY_SPACE;
    constexpr int toggleMouse = GLFW_KEY_E;
    constexpr int placeBlock = GLFW_KEY_RIGHT_ALT;

    //Write all of these manually so they can be changed however
    constexpr int selectItem[numOptions] = {
        GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9
    };
}