#include "SpecialMapExtending.hpp"

#include "Server.hpp"

namespace Recipe {
    SpecialMapExtending::SpecialMapExtending(const nlohmann::json &recipe):
        Recipe(recipe)
    {
        this->setValidity(false);
    }

    void SpecialMapExtending::dump(void) const
    {
        LINFO("recipe special suspicious stew");
    }

    std::unique_ptr<Recipe> SpecialMapExtending::create(const nlohmann::json &recipe)
    {
        return (std::make_unique<SpecialMapExtending>(SpecialMapExtending(recipe)));
    }
};