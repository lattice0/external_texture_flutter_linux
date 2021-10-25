cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "external_texture")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "external_texture_plugin")

add_library(${PLUGIN_NAME} SHARED
  "external_texture_plugin.cc"
  "fl_my_texture_gl.cc"
  "opengl_renderer.cpp"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter)
target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::GTK)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package( OpenGL REQUIRED )
target_link_libraries(${PLUGIN_NAME} PRIVATE OpenGL::GL)

find_package(GLEW REQUIRED)
target_link_libraries(${PLUGIN_NAME} PRIVATE GLEW::GLEW )

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(${PLUGIN_NAME} PRIVATE glfw )

# List of absolute paths to libraries that should be bundled with the plugin
set(external_texture_bundled_libraries
  ""
  PARENT_SCOPE
)
