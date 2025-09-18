#pragma once
#include <shaderType.hpp>
#include <vector>

enum class LogicType {AND, NAND, OR, NOR, XOR, NOT, TRAN};

struct InstanceAttribute {
    glm::mat4 model;
    glm::vec3 color;
};

struct Component {
    bool active;
    LogicType type;
    uint32_t meshType;
    bool evaluate(bool a, bool b) {
        switch (type) {
            case LogicType::TRAN: return a;
            case LogicType::NOT: return !a;
            case LogicType::AND: return a && b;
            case LogicType::NAND: return !(a && b);
            case LogicType::OR: return a || b;
            case LogicType::NOR: return !(a || b);
            case LogicType::XOR: return a != b;
        }
    }
};

struct UniqueComponent : Component {
    shaderType shaderID;
    std::vector<uint16_t>* indicesPointer;
    uint32_t firstIndex;
    uint32_t numIndices;
};


