#include <component.hpp>
#include <initializer.hpp>
#include <chrono>

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
    for (uint32_t i = 0; i < 10000; i++) {
        components.addComponent(&rearranged[0], 12, LogicType::OR);
        components.addComponent(&vertices12[0], 12, LogicType::NAND);
    }
    auto start = std::chrono::system_clock::now();
    for (uint32_t i = 0; i < 1000000; i++) {
        components.addComponent(&rearranged[0], 12, LogicType::OR);
        components.addComponent(&vertices12[0], 12, LogicType::NAND);
        components.removeComponent(components.components[5]);
        components.removeComponent(components.components[3]);
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s"
              << std::endl;
}
