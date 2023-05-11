#include "StrippedDarkOakWood.hpp"
namespace Blocks {
namespace StrippedDarkOakWood {
BlockId toProtocol(Properties::Axis axis)
{
    switch (axis) {
    case Properties::Axis::X:
        return 208;
    case Properties::Axis::Y:
        return 209;
    case Properties::Axis::Z:
        return 210;
    default:
        return 0;
    }
    return 0;
}
}

}
