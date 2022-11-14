#include "DefaultWorldGroup.hpp"

#include <utility>
#include "DefaultWorld.hpp"

DefaultWorldGroup::DefaultWorldGroup(std::shared_ptr<Chat> chat)
    : WorldGroup(std::move(chat))
{

}

void DefaultWorldGroup::initialize()
{
    _worlds.emplace("default", std::make_shared<DefaultWorld>());
    _worlds.at("default")->initialize();
}
