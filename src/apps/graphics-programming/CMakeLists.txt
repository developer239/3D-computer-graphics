set(APP_NAME GraphicsProgramming)
set(APP_FOLDER_NAME graphics-programming)

set(APP_NAME_TEST ${LIB_NAME}Test)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${APP_NAME} ${SOURCES})

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/src/apps/${APP_FOLDER_NAME})

target_link_libraries(${APP_NAME} Core)
target_link_libraries(${APP_NAME} CoreImgui)
target_link_libraries(${APP_NAME} Logger)
