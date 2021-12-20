# ImGui_RAII lightweigth C++20 library

## Author
Dominic Koepke  
Mail: [DNKpp2011@gmail.com](mailto:dnkpp2011@gmail.com)

## License
[BSL-1.0](LICENSE_1_0.txt) (free, open source)

```text
          Copyright Dominic Koepke 2021 - 2021.
 Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
          https://www.boost.org/LICENSE_1_0.txt)
```

## Description
You're using [Dear ImGui](https://github.com/ocornut/imgui) and getting tired of explicitly calling ``endXYZ()``? Then this library might solve your issues.

This library consists mainly of one header, which one can simply put into their projects and include them. Otherwise you can add the whole repository to your project and add it via cmake.
In either case ``ImGui_RAII`` must be able to include the header ``imgui.h`` and you have to compile the imgui sources or at least link to a library which does so.

If you are already using ``Dear ImGui`` than to include this library just include the ``ImGui_RAII``'s header file.

``ImGui_RAII``doesn't force you to anything. Users can combine ``ImGui_RAII`` classes with traditional ``Dear ImGui`` functional approach.

## Example
Be aware ``Dear ImGui`` isn't runable by its own, thus the examples below are not actually working.

Below the straight forward way is shown.
```cpp
#include <imgui_raii/imgui_raii.hpp>

void my_render()
{
	imgui_raii::NextFrame frame{};		// when this object gets destroyed, it calls ImGui::Render()

	// Begin and many more classes are usable in any boolean context
	if (imgui_raii::Begin win{ "Hello, World!" })
	{
		// calling a original ImGui function
		if (ImGui::Button("Test Button"))
		{
			// do something
		}
		
		if (ImGui::Button("Exit"))
		{
			// this approach enables your the possibility to return early
			return;
		}
	}

	// no end operations needed!
}

void main()
{
	imgui_raii::BeginContext context{};	// this is the only class which is movable
	
	while (true) my_render();
}
```

Another nice syntax is to use the lambda chaining syntax.
```cpp
#include <imgui_raii/imgui_raii.hpp>

void my_render()
{
	/*
	As you can see anonymous objects for chaining are completely fine!
	ImGui::NextFrame() doesn't return a bool, thus the chained lambda will always be executed.
	*/
	imgui_raii::NextFrame{} /
		[]
		{
			// ImGui::Begin does return a bool, thus the chained lambda will only be executed if true has been returned.
			imgui_raii::Begin{ "Hello, World!" } /
				[]
				{
					// calling the original ImGui function
					if (ImGui::Button("Test Button"))
					{
						// do something
					}
					
					if (ImGui::Button("Exit"))
					{
						// early return is still possible, but of course will only return from the local lambda
						return;
					}
				}
				
		}

	// no end operations needed!
}

void main()
{
	imgui_raii::BeginContext context{};	// this is the only class which is movable
	
	while (true) my_render();
}
```

## Installation with CMake
This library can easily be integrated into your project via CMake target_link_libraries command.

```cmake
target_link_libraries(
	<your_target_name>
	PRIVATE
	ImGui_RAII::ImGui_RAII
)
```

### FetchContent
It is possible fetching this library via CMakes FetchContent module.

```cmake
include(FetchContent)

FetchContent_Declare(
	ImGui_RAII
	GIT_REPOSITORY	https://github.com/DNKpp/ImGui_RAII
	GIT_TAG			"master"
)
FetchContent_MakeAvailable(ImGui_RAII)

```

### CPM
The [CPM](https://github.com/cpm-cmake/CPM.cmake) CMake module is a featureful wrapper around the FetchContent feature. To use it, simply add ``CPM.cmake`` into your project folder. 

```cmake
include(CPM)

CPMAddLibrary("gh:DNKpp/ImGui_RAII#master")
```
