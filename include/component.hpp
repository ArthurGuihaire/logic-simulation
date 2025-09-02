#ifndef COMPONENT
#define COMPONENT
#include <vector>
#include <utility>
enum class LogicType {AND, NAND, OR, NOR, XOR, NOT, TRAN};

struct Component {
    bool active;
    LogicType type;
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

#endif