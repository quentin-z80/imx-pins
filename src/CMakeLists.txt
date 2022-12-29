set(OpenGL_GL_PREFERENCE GLVND)

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(FLTK REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(imx-pins WIN32 MACOSX_BUNDLE imx-pins.cxx Fl_Button_Tree.cxx Fl_Button_Tree_Item.cxx PinSettings.cxx)
if (APPLE)
  target_link_libraries(imx-pins "-framework cocoa")
endif (APPLE)

target_include_directories(imx-pins PUBLIC ${FLTK_INCLUDE_DIRS})

target_link_libraries(imx-pins fltk nlohmann_json::nlohmann_json)
