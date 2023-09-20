cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

set(CLI "${PROJECT_NAME}")

include(GoogleTest)

configure_file(version.h.in version.h)

add_executable(${CLI})
add_library(proj_info lib.cpp)
add_executable(test_version test_version.cpp)
target_sources(${CLI} PRIVATE
   "ip_address_filtering/ip_filter.cpp"
   "ip_address_filtering/ip_address.cpp"
)

set_target_properties(${CLI} proj_info test_version PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(proj_info
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_include_directories(${CLI}
    PRIVATE "${CMAKE_SOURCE_DIR}"
)

target_link_libraries(${CLI} PRIVATE
    proj_info
)
target_link_libraries(test_version
    proj_info
    gtest
)

if (MSVC)
    target_compile_options(${CLI} PRIVATE
        /W4
    )
    target_compile_options(proj_info PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(${CLI} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(proj_info PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS ${CLI} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_version test_version)
