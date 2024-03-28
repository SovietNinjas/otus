cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION
    "1"
    CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(matrix VERSION ${PROJECT_VESRION})

set(TARGET_CLIENT "${PROJECT_NAME}")
set(TARGET_TESTS "${TARGET_CLIENT}-test")

include(GoogleTest)

add_executable(${TARGET_CLIENT})
add_executable(${TARGET_TESTS} "test_version.cpp")
target_sources(${TARGET_CLIENT} PRIVATE "main.cpp")

set_target_properties(${TARGET_CLIENT} ${TARGET_TESTS}
                      PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)
target_include_directories(${TARGET_CLIENT} PRIVATE "${CMAKE_SOURCE_DIR}")

target_link_libraries(${TARGET_TESTS} gtest)

if(MSVC)
  target_compile_options(${TARGET_CLIENT} PRIVATE /W4)
  target_compile_options(proj-info PRIVATE /W4)
  target_compile_options(${TARGET_TESTS} PRIVATE /W4)
else()
  target_compile_options(${TARGET_CLIENT} PRIVATE -Wall -Wextra -pedantic
                                                  -Werror)
  target_compile_options(${TARGET_TESTS} PRIVATE -Wall -Wextra -pedantic
                                                 -Werror)
endif()

install(TARGETS ${TARGET_CLIENT} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(${TARGET_TESTS} ${TARGET_TESTS})
