cmake_minimum_required(VERSION 3.10.0)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch Version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bulk_server VERSION ${PROJECT_VERSION} LANGUAGES C CXX)

option(WITH_GTEST_TEST "Whether to build GTEST test" ON)

configure_file(version.h.in version.h)

add_executable(bulk_server
    src/main.cpp
    src/server.cpp
    src/bulk.cpp
    src/printer.cpp
    src/parser.cpp
    src/async.cpp
    src/parsermanager.cpp
    src/printerdispatcher.cpp
)

target_include_directories(bulk_server PUBLIC include/)

if(WITH_GTEST_TEST) 
    find_package(GTest REQUIRED)
    
    add_executable(bulk_server_test 
        tests/bulk_test.cpp
        src/server.cpp
        src/bulk.cpp
        src/printer.cpp
        src/parser.cpp
        src/async.cpp
        src/parsermanager.cpp
        src/printerdispatcher.cpp
    )

    target_include_directories(bulk_server_test PUBLIC
        include/
    )

    set_target_properties(bulk_server_test PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    target_link_libraries(bulk_server_test PRIVATE GTest::GTest)
endif()

if (MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )

else ()
    target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

    if(WITH_GTEST_TEST)
    target_compile_options(bulk_server_test PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    endif()
endif()

install(TARGETS bulk_server RUNTIME DESTINATION usr/lib)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

if(WITH_GTEST_TEST)
    enable_testing()
    add_test(bulk_server_test bulk_server_test)
endif()