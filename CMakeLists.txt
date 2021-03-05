cmake_minimum_required (VERSION 3.12)

project(Prim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Aka
add_subdirectory(lib/Aka)

set(SRC
	"src/main.cpp"
	"src/Viewer.cpp"
	"src/Editor/EditorUI.cpp"
	"src/Editor/SceneUI.cpp"

	"src/Prim/ThreadPool.cpp"

	"src/Prim/Core/BoundingBox.cpp"
	"src/Prim/Core/BSDF.cpp"
	"src/Prim/Core/Camera.cpp"
	"src/Prim/Core/Hitable.cpp"
	"src/Prim/Core/Intersection.cpp"
	"src/Prim/Core/Light.cpp"
	"src/Prim/Core/LightDistribution.cpp"
	"src/Prim/Core/Loader.cpp"
	"src/Prim/Core/Ray.cpp"
	"src/Prim/Core/Scene.cpp"
	"src/Prim/Core/Transform.cpp"
	
	"src/Prim/Scene/Camera/OrthographicCamera.cpp"
	"src/Prim/Scene/Camera/PerspectiveCamera.cpp"

	"src/Prim/Scene/Hitable/Mesh.cpp"
	"src/Prim/Scene/Hitable/MeshBVH.cpp"
	"src/Prim/Scene/Hitable/MeshOctree.cpp"
	"src/Prim/Scene/Hitable/Sphere.cpp"
	"src/Prim/Scene/Hitable/Triangle.cpp"
	"src/Prim/Scene/Hitable/TransformNode.cpp"

	"src/Prim/Scene/Light/PointLight.cpp"
	"src/Prim/Scene/Light/ShapeLight.cpp"
	"src/Prim/Scene/Light/SunLight.cpp"
	
	"src/Prim/Scene/Loader/OBJLoader.cpp"
	"src/Prim/Scene/Loader/GLTFLoader.cpp"

	"src/Prim/Scene/Material/Matte.cpp"
	"src/Prim/Scene/Material/Metal.cpp"
	"src/Prim/Scene/Material/Glass.cpp"
	
	"src/Prim/Scene/Texture/CheckerTexture.cpp"
	"src/Prim/Scene/Texture/ConstantTexture.cpp"
	"src/Prim/Scene/Texture/ImageTexture.cpp"

	"src/Prim/Shape/ShapeDisk.cpp"
	"src/Prim/Shape/ShapeSphere.cpp"

	"src/Prim/Tracer/PathTracer.cpp"
	"src/Prim/Tracer/WhittedTracer.cpp"
)
if(GRAPHIC_OPENGL)
	set(SRC
		${SRC}
		"lib/imgui/imgui.cpp"
		"lib/imgui/imgui_demo.cpp"
		"lib/imgui/imgui_draw.cpp"
		"lib/imgui/imgui_widgets.cpp"
		"lib/imgui/imgui_tables.cpp"
		"lib/imgui/backends/imgui_impl_opengl3.cpp"
		"lib/imgui/backends/imgui_impl_glfw.cpp"
	)
elseif(GRAPHIC_D3D11)
	set(SRC
		${SRC}
		"lib/imgui/imgui.cpp"
		"lib/imgui/imgui_demo.cpp"
		"lib/imgui/imgui_draw.cpp"
		"lib/imgui/imgui_widgets.cpp"
		"lib/imgui/imgui_tables.cpp"
		"lib/imgui/backends/imgui_impl_dx11.cpp"
		"lib/imgui/backends/imgui_impl_glfw.cpp"
	)
endif()
add_executable(Prim
	${SRC}
)

# Add third party libs
target_include_directories(Prim PUBLIC lib/imgui)
target_include_directories(Prim PUBLIC lib/Aka/lib/glfw/include)
target_include_directories(Prim PUBLIC lib/Aka/lib/glew/include)
target_include_directories(Prim PUBLIC lib/Aka/lib/stb)

target_link_libraries(Prim Aka)

add_custom_command(
	TARGET Prim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/lib/Aka/lib/glew/bin/glew32.dll"
    $<TARGET_FILE_DIR:Prim>
)

add_custom_command(
	TARGET Prim POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/asset
	$<TARGET_FILE_DIR:Prim>/asset
)
