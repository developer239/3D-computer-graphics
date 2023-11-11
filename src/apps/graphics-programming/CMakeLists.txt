set(APP_NAME GraphicsProgramming)
set(APP_FOLDER_NAME graphics-programming)

set(APP_NAME_TEST ${LIB_NAME}Test)

set(SOURCES
        src/main.cpp
        src/geometry/Cube.h
        src/geometry/Mesh.h
        src/LoopStrategy.h
        src/math/Faces.h
        src/math/Triangle.h
        src/math/Vec.h
        src/rendering/Camera.h
        src/rendering/Painter.h
        src/rendering/Projector.h
        src/rendering/WavefrontLoader.h
        src/utils.h src/gui/Controls.h)

add_executable(${APP_NAME} ${SOURCES})

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/src/apps/${APP_FOLDER_NAME})

target_link_libraries(${APP_NAME} Core)
target_link_libraries(${APP_NAME} CoreImgui)
target_link_libraries(${APP_NAME} Logger)
