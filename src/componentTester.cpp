#include <component.hpp>
#include <initializer.hpp>

int main() {
    Initializer init;
    init.setWindowSize(800, 600);
    init.initGLFW();
    auto window = init.createWindow();
    init.initGLAD();

    std::cout << glGetString(GL_RENDERER) << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    ComponentSystem components;

    float vertices12[12] = {
        0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 
        1.0f, 0.0f, 0.0f, 
        -1.0f, 1.0f, 0.0f
    };

    float rearranged[12] = {
        1.0f, 0.0f, 0.0f, 
        -1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f
    };

    float vertices15[15] = {
        2.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 
        1.0f, 0.0f, 0.0f, 
        -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };

    components.addComponent(&vertices12[0], 12, LogicType::XOR);
    components.addComponent(&rearranged[0], 12, LogicType::TRAN);
    components.removeComponent(0);
    components.addComponent(&vertices15[0], 15, LogicType::NOT);
    components.removeComponent(1);
    components.addComponent(&vertices15[0], 15, LogicType::NOT);
    components.removeComponent(0);

    std::cout << "it worked" << std::endl;
}
