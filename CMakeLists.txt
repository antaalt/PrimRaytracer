cmake_minimum_required (VERSION 3.12)

project(AkaPrim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Aka
add_subdirectory(lib/Aka)

add_executable(AkaPrim
	"src/main.cpp"

	"src/Viewer.cpp"

	"src/Prim/BoundingBox.cpp"
	"src/Prim/BSDF.cpp"
	"src/Prim/Camera.cpp"
	"src/Prim/CheckerTexture.cpp"
	"src/Prim/Config.cpp"
	"src/Prim/ConstantTexture.cpp"
	"src/Prim/Glass.cpp"
	"src/Prim/GLTFLoader.cpp"
	"src/Prim/Hitable.cpp"
	"src/Prim/ImageTexture.cpp"
	"src/Prim/Intersection.cpp"
	"src/Prim/Light.cpp"
	"src/Prim/LightDistribution.cpp"
	"src/Prim/Loader.cpp"
	"src/Prim/Matte.cpp"
	"src/Prim/Mesh.cpp"
	"src/Prim/MeshBVH.cpp"
	"src/Prim/MeshOctree.cpp"
	"src/Prim/Metal.cpp"
	"src/Prim/OBJLoader.cpp"
	"src/Prim/OrthographicCamera.cpp"
	"src/Prim/PathTracer.cpp"
	"src/Prim/PerspectiveCamera.cpp"
	"src/Prim/PointLight.cpp"
	"src/Prim/Random.cpp"
	"src/Prim/Ray.cpp"
	"src/Prim/Renderer.cpp"
	"src/Prim/Scene.cpp"
	"src/Prim/ShapeDisk.cpp"
	"src/Prim/ShapeLight.cpp"
	"src/Prim/ShapeSphere.cpp"
	"src/Prim/Sphere.cpp"
	"src/Prim/SunLight.cpp"
	"src/Prim/ThreadPool.cpp"
	"src/Prim/Transform.cpp"
	"src/Prim/TransformNode.cpp"
	"src/Prim/Triangle.cpp"
	"src/Prim/WhittedTracer.cpp"
)

# Add third party libs
#target_include_directories(AkaPrim PUBLIC lib/imgui)
#target_include_directories(AkaPrim PUBLIC lib/Aka/lib/glfw/include)
#target_include_directories(AkaPrim PUBLIC lib/Aka/lib/glew/include)
#target_include_directories(AkaPrim PUBLIC lib/IconFontCppHeaders)
#target_include_directories(AkaPrim PUBLIC lib/json/include)
target_include_directories(AkaPrim PUBLIC lib/Aka/lib/stb)

target_link_libraries(AkaPrim Aka)

add_custom_command(
	TARGET AkaPrim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/lib/Aka/lib/glew/bin/glew32.dll"
    $<TARGET_FILE_DIR:AkaPrim>
)
