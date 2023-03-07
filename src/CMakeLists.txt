set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(FLTK_BUILD_TEST OFF CACHE BOOL " " FORCE)
set(FLTK_BUILD_FLUID OFF CACHE BOOL " " FORCE)
set(FLTK_BUILD_FLTK_OPTIONS OFF CACHE BOOL " " FORCE)
if(UNIX AND NOT APPLE)
    set(OPTION_USE_PANGO ON CACHE BOOL " " FORCE)
endif()

FetchContent_Declare(FLTK
    GIT_REPOSITORY https://github.com/fltk/fltk
)
FetchContent_GetProperties(FLTK)
if(NOT FLTK_POPULATED)
  FetchContent_Populate(FLTK)
  add_subdirectory(${fltk_SOURCE_DIR} ${fltk_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG bc889afb4c5bf1c0d8ee29ef35eaaf4c8bef8a5d # v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

if(WIN32)
    set(GUI_TYPE WIN32)
elseif(APPLE)
    set(GUI_TYPE MACOSX_BUNDLE)
else()
    set(GUI_TYPE)
endif()

add_executable(imx-pins WIN32 MACOSX_BUNDLE imx-pins.cxx Fl_Button_Tree.cxx Fl_Button_Tree_Item.cxx PinSettings.cxx)
target_include_directories(imx-pins PRIVATE ${fltk_SOURCE_DIR} ${fltk_BINARY_DIR})
target_link_libraries(imx-pins PRIVATE fltk PRIVATE nlohmann_json::nlohmann_json)

if (APPLE)
  target_link_libraries(imx-pins "-framework cocoa")
endif (APPLE)
