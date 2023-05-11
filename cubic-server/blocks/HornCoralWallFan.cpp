#include "HornCoralWallFan.hpp"
namespace Blocks {
namespace HornCoralWallFan {
BlockId toProtocol(Properties::Facing facing, Properties::Waterlogged waterlogged)
{
    switch (facing) {
    case Properties::Facing::NORTH:
        switch (waterlogged) {
        case Properties::Waterlogged::TRUE:
            return 12297;
        case Properties::Waterlogged::FALSE:
            return 12298;
        default:
            return 0;
        }
    case Properties::Facing::SOUTH:
        switch (waterlogged) {
        case Properties::Waterlogged::TRUE:
            return 12299;
        case Properties::Waterlogged::FALSE:
            return 12300;
        default:
            return 0;
        }
    case Properties::Facing::WEST:
        switch (waterlogged) {
        case Properties::Waterlogged::TRUE:
            return 12301;
        case Properties::Waterlogged::FALSE:
            return 12302;
        default:
            return 0;
        }
    case Properties::Facing::EAST:
        switch (waterlogged) {
        case Properties::Waterlogged::TRUE:
            return 12303;
        case Properties::Waterlogged::FALSE:
            return 12304;
        default:
            return 0;
        }
    default:
        return 0;
    }
    return 0;
}
}

}
