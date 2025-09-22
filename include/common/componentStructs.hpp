#pragma once
#include <shaderType.hpp>
#include <vector>

enum class LogicType {NUL, TRAN, NOT, AND, OR, NAND, NOR, XOR, XNOR};

struct InstanceAttribute {
    glm::vec4 color;
    glm::mat4 model;
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
            case LogicType::OR: return a || b;
            case LogicType::NAND: return !(a && b);
            case LogicType::NOR: return !(a || b);
            case LogicType::XOR: return a != b;
            case LogicType::XNOR: return a==b;
            case LogicType::NUL: return false;
        }
    }
};

struct UniqueComponent : Component {
    shaderType shaderID;
    std::vector<uint16_t>* indicesPointer;
    uint32_t firstIndex;
    uint32_t numIndices;
};


