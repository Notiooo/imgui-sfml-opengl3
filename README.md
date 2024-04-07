ImGui-SFML
==========
### This fork works only under Opengl3 and above. If you want to use it with Opengl2, please use the original repository!

Library which allows you to use [Dear ImGui](https://github.com/ocornut/imgui) with [SFML](https://github.com/SFML/SFML)

![screenshot](https://i2.wp.com/i.imgur.com/iQibpSk.gif)

Based on [this repository](https://github.com/Mischa-Alff/imgui-backends) with big improvements and changes.

State of Development
-----

-   The [`2.6.x`](https://github.com/SFML/imgui-sfml/tree/2.6.x) branch contains work targeting SFML 2 and is thus stable.

Dependencies
-----

* [SFML](https://github.com/SFML/SFML) >= 2.5.0
* [Dear ImGui](https://github.com/ocornut/imgui) >= 1.89

Contributing
-----

* The code is written in C++11 (stable SFML is still C++03, Dear ImGui has started using C++11 since 2022)
* The code should be formatted via [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) using `.clang-format` provided in the root of this repository

How-to
----

- [**CMake tutorial which also shows how to use ImGui-SFML with FetchContent**](https://edw.is/using-cmake/)
- [**Example project which sets up ImGui-SFML with FetchContent**](https://github.com/eliasdaler/cmake-fetchcontent-tutorial-code)
- [**Detailed tutorial on Elias Daler's blog**](https://edw.is/using-imgui-with-sfml-pt1)
- [**Using ImGui with modern C++ and STL**](https://edw.is/using-imgui-with-sfml-pt2/)
- [**Thread on SFML forums**](https://en.sfml-dev.org/forums/index.php?topic=20137.0). Feel free to ask your questions there.

Building and integrating into your CMake project
---
It's highly recommended to use FetchContent or git submodules to get SFML and Dear ImGui into your build.

See [this file](https://github.com/eliasdaler/imgui-sfml-fetchcontent/blob/master/dependencies/CMakeLists.txt) - if you do something similar, you can then just link to ImGui-SFML as simply as:

```cmake
target_link_libraries(game
  PUBLIC
    ImGui-SFML::ImGui-SFML
)
```

Integrating into your project manually
---
- Download [ImGui](https://github.com/ocornut/imgui)
- Add Dear ImGui folder to your include directories
- Add `imgui.cpp`, `imgui_widgets.cpp`, `imgui_draw.cpp` and `imgui_tables.cpp` to your build/project
- Copy the contents of `imconfig-SFML.h` to your `imconfig.h` file. (to be able to cast `ImVec2` to `sf::Vector2f` and vice versa)
- Add a folder which contains `imgui-SFML.h` to your include directories
- Add `imgui-SFML.cpp` to your build/project
- Link OpenGL if you get linking errors

Other ways to add to your project
---
Not recommended, as they're not maintained officially. Tend to lag behind and stay on older versions.

- [Conan](https://github.com/bincrafters/community/tree/main/recipes/imgui-sfml)
- [vcpkg](https://github.com/microsoft/vcpkg/tree/master/ports/imgui-sfml)
- [Bazel](https://github.com/zpervan/ImguiSFMLBazel)

Using ImGui-SFML in your code
---

- Call `ImGui::SFML::Init` and pass your `sf::Window` there. You can create your font atlas and pass the pointer in Init too, otherwise the default internal font atlas will be created for you. Do this for each window you want to draw ImGui on.
- For each iteration of a game loop:
    - Poll and process events:

        ```cpp
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            ...
        }
        ```

    - Call `ImGui::SFML::Update(window, deltaTime)` where `deltaTime` is `sf::Time`. You can also pass `mousePosition` and `displaySize` yourself instead of passing the window.
    - Call ImGui functions (`ImGui::Begin()`, `ImGui::Button()`, etc.)
    - Call `ImGui::EndFrame` after the last `ImGui::End` in your update function, if you update more than once before rendering. (e.g. fixed delta game loops)
    - Call `ImGui::SFML::Render(window)`

- Call `ImGui::SFML::Shutdown()` **after** `window.close()` has been called
    - Use `ImGui::SFML::Shutdown(window)` overload if you have multiple windows. After it's called one of the current windows will become a current "global" window. Call `SetCurrentWindow` to explicitly set which window will be used as default.

**If you only draw ImGui widgets without any SFML stuff, then you'll might need to call window.resetGLStates() before rendering anything. You only need to do it once.**

Example code
----

See example file [here](https://github.com/SFML/imgui-sfml/blob/master/examples/minimal/main.cpp)

```cpp
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Window/Window.hpp> // Core profile does not support Graphic module :)
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.attributeFlags = sf::ContextSettings::Core;
    
    sf::Window window(sf::VideoMode(640, 480), "ImGui + SFML + OpenGL3 = <3", settings);
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    // prepare something to draw :)

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowDemoWindow();

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        
        // draw your shape there c:
        
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
```

Glsl versions how-to
---
Default `#version 330` is loaded if you don't pass different one in `ImGui::SFML::Init`. Call `ImGui::SFML::Init(window, glsl_version);` if you want different one.

Multiple windows
----------------

See `examples/multiple_windows` to see how you can create multiple SFML and run different ImGui contexts in them.

- Don't forget to run `ImGui::SFML::Init(const sf::Window&)` for each window you create. Same goes for `ImGui::SFML::Shutdown(const sf::Window&)`
- Instead of calling `ImGui::SFML::ProcessEvent(sf::Event&)`, you need to call `ImGui::SFML::ProcessEvent(const sf::Window&, const sf::Event&)` overload for each window you create
- Call `ImGui::SFML::SetCurrentWindow` before calling any `ImGui` functions (e.g. `ImGui::Begin`, `ImGui::Button` etc.)
- Either call `ImGui::Render(sf::Window&)` overload for each window or manually do this:
    ```cpp
    SetCurrentWindow(window);
    ... // your custom rendering
    ImGui::Render();

    SetCurrentWindow(window2);
    ... // your custom rendering
    ImGui::Render();
    ```
- When closing everything: don't forget to close all windows using SFML's `sf::Window::Close` and then call `ImGui::SFML::Shutdown` to remote all ImGui-SFML window contexts and other data.

Mouse cursors
---
You can change your cursors in ImGui like this:

```cpp
ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
```

By default, your system cursor will change and will be rendered by your system. If you want SFML to draw your cursor with default ImGui cursors (the system cursor will be hidden), do this:

```cpp
ImGuiIO& io = ImGui::GetIO();
io.MouseDrawCursor = true;
```

Keyboard/Gamepad navigation
---
Starting with [ImGui 1.60](https://github.com/ocornut/imgui/releases/tag/v1.60), there's a feature to control ImGui with keyboard and gamepad. To use keyboard navigation, you just need to do this:

```cpp
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
```

Gamepad navigation requires more work, unless you have XInput gamepad, in which case the mapping is automatically set for you. But you can still set it up for your own gamepad easily, just take a look how it's done for the default mapping [here](https://github.com/SFML/imgui-sfml/blob/navigation/imgui-SFML.cpp#L697). And then you need to do this:

```cpp
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
```
By default, the first active joystick is used for navigation, but you can set joystick id explicitly like this:
```cpp
ImGui::SFML::SetActiveJoystickId(5);
```

High DPI screens
----

As SFML is not currently DPI aware, your GUI may show at the incorrect scale. This is particularly noticeable on systems with "Retina" / "4K" / "UHD" displays.

To work around this on macOS, you can create an app bundle (as opposed to just the exe) then modify the info.plist so that "High Resolution Capable" is set to "NO".

Another option to help ameliorate this, at least for getting started and for the common ImGui use-case of "developer/debug/building UI", is to explore `FontGlobalScale`:

```cpp
ImGuiIO& io = ImGui::GetIO();
io.FontGlobalScale = 2.0; // or any other value hardcoded or loaded from your config logic
```

License
---

This library is licensed under the MIT License, see LICENSE for more information.
