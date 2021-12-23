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

### Boolean checkable
Below the straight forward way is shown.
```cpp
#include <imgui_raii/imgui_raii.hpp>

void my_render()
{
	imgui_raii::NewFrame frame{};		// when this object gets destroyed, it calls ImGui::Render()

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

### Chainable with lambdas
Another nice syntax is to use the lambda chaining.

There are two different operators overloaded for chaining. As ``/`` often indicates the next level of a hirarchy (e.g in a file system)
there is usually no precondition. Objects which are not boolean checkable (like ``imgui_raii::NewFrame``) use the ``operator /`` for chaining and the attached lambda
will always executed. On the other hand ``ImGui`` heavily relys on functions which return a ``bool``, which indicates if code should be executed or not. For this reason
the ``operator >`` comes into play. At first I thought about the ``operator =>``, because it will be used in the next c++ version for pattern matching and would fit very good here.
Unfortunatly there is currently no such operator available in c++, thus I decided to use the most similar one: ``operator >``

When this operator is used the attached lambda will only be executed if the object has a ``true`` state.

Note that only one of these operators are available in each class.

```cpp
#include <imgui_raii/imgui_raii.hpp>

void my_render()
{
	/*
	As you can see anonymous objects for chaining are completely fine!
	ImGui::NextFrame() doesn't return a bool, thus the chained lambda will always be executed.
	*/
	imgui_raii::NewFrame{} /
		[]
		{
			// ImGui::Begin does return a bool, thus the chained lambda will only be executed if true has been returned.
			imgui_raii::Begin{ "Hello, World!" } >
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
				};
		};

	// no end operations needed!
}

void main()
{
	imgui_raii::BeginContext context{};	// this is the only class which is movable
	
	while (true) my_render();
}
```

## Installation with CMake
This library can easily be integrated into your project via CMake target_link_libraries command. ``ImGui_RAII`` must know the source directory of ``Dear ImGui``. To solve that
the variable ``IMGUI_DIR`` must be set.

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

set(IMGUI_DIR <path to imgui>)
FetchContent_MakeAvailable(ImGui_RAII)

```

### CPM
The [CPM](https://github.com/cpm-cmake/CPM.cmake) CMake module is a featureful wrapper around the ``FetchContent`` feature. To use it simply add ``CPM.cmake`` or ``get_cmp.make`` (which will pull ``CPM.cmake``)
from the latest release into your project folder and include it into your ``CMakeLists.txt``. 

```cmake
include(CPM)

set(IMGUI_DIR <path to imgui>)
CPMAddPackage("gh:DNKpp/ImGui_RAII#master")
```
