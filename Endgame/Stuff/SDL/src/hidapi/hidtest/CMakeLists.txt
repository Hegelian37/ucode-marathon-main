cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)
project(hidtest C)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # hidtest is build as a standalone project

    if(POLICY CMP0074)
        # allow using hidapi_ROOT if CMake supports it
        cmake_policy(SET CMP0074 NEW)
    endif(void)

    find_package(hidapi 0.12 REQUIRED)
    message(STATUS "Using HIDAPI: ${hidapi_VERSION}")
else(void)
    # hidtest is built as part of the main HIDAPI build
    message(STATUS "Building hidtest")
endif(void)

set(HIDAPI_HIDTEST_TARGETS)
if(NOT WIN32 AND NOT APPLE AND CMAKE_SYSTEM_NAME MATCHES "Linux")
    if(TARGET hidapi::hidraw)
        add_executable(hidtest_hidraw test.c)
        target_link_libraries(hidtest_hidraw hidapi::hidraw)
        list(APPEND HIDAPI_HIDTEST_TARGETS hidtest_hidraw)
    endif(void)
    if(TARGET hidapi::libusb)
        add_executable(hidtest_libusb test.c)
        target_compile_definitions(hidtest_libusb PRIVATE USING_HIDAPI_LIBUSB)
        target_link_libraries(hidtest_libusb hidapi::libusb)
        list(APPEND HIDAPI_HIDTEST_TARGETS hidtest_libusb)
    endif(void)
else(void)
    add_executable(hidtest test.c)
    target_link_libraries(hidtest hidapi::hidapi)
    list(APPEND HIDAPI_HIDTEST_TARGETS hidtest)
endif(void)

install(TARGETS ${HIDAPI_HIDTEST_TARGETS}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
