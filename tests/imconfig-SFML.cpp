#include <imgui.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("IM_VEC2_CLASS_EXTRA") {
    SECTION("From sf::Vector2f") {
        const auto imvec = ImVec2(sf::Vector2f(1.2f, 3.4f));
        CHECK(imvec.x == 1.2f);
        CHECK(imvec.y == 3.4f);
    }

    SECTION("From sf::Vector2i") {
        const auto imvec = ImVec2(sf::Vector2i(1, 2));
        CHECK(imvec.x == 1);
        CHECK(imvec.y == 2);
    }

    SECTION("To sf::Vector2f") {
        CHECK(sf::Vector2i(ImVec2(1.1f, 2.2f)) == sf::Vector2i(1, 2));
        CHECK(sf::Vector2f(ImVec2(1.1f, 2.2f)) == sf::Vector2f(1.1f, 2.2f));
    }
}
