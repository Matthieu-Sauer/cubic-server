#ifndef CUBICSERVER_DEFAULT_OVERWORLD_HPP
#define CUBICSERVER_DEFAULT_OVERWORLD_HPP

#include <future>

#include "Dimension.hpp"
#include "default/DefaultWorld.hpp"
#include "world_storage/Persistence.hpp"

class Overworld : public Dimension {
public:
    Overworld(std::shared_ptr<DefaultWorld> world):
        Dimension(world, world_storage::DimensionType::OVERWORLD)
    {
    }
    ~Overworld() override = default;

    void tick() override;
    void initialize() override;
    void stop() override;
    void generateChunk(int x, int z, world_storage::GenerationState goalState = world_storage::GenerationState::READY) override;

private:
    std::future<void> _worldGenFuture;
};

#endif // CUBICSERVER_DEFAULT_OVERWORLD_HPP
