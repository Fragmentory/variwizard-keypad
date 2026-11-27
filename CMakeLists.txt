# SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.22)
project(wizard VERSION 1.0.0.0 DESCRIPTION "Custom composed HID device controller" LANGUAGES CXX)

include(GNUInstallDirs)
include(CTest)

set(GIT_HASH unknown)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    execute_process(COMMAND git rev-parse --short=4 HEAD
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)
endif()
configure_file(src/wizard_revision.h.in generated/wizard_revision.h @ONLY)

add_library(_varikey src/varikey_gadget_usb.cpp)
add_library(VariKey::host ALIAS _varikey)
set_target_properties(_varikey PROPERTIES
    OUTPUT_NAME varikey-host EXPORT_NAME host
    VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON)
target_compile_features(_varikey PUBLIC cxx_std_17)
target_compile_options(_varikey PRIVATE -Wall)
target_include_directories(_varikey PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/varikey>")

add_executable(wizard src/wizard.cpp src/wizard_args.cpp src/wizard_usb.cpp)
target_compile_options(wizard PRIVATE -Wall)
target_include_directories(wizard PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")
target_link_libraries(wizard PRIVATE VariKey::host)

install(TARGETS _varikey EXPORT VariKeyHostTargets)
install(TARGETS wizard RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES src/varikey_gadget_usb.hpp src/varikey_command.hpp
    src/varikey_device.hpp src/varikey_gadget.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/varikey)
install(EXPORT VariKeyHostTargets NAMESPACE VariKey:: FILE VariKeyHostConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VariKeyHost)

if(BUILD_TESTING AND NOT CMAKE_CROSSCOMPILING)
    add_test(NAME wizard-help COMMAND wizard --help)
    set_tests_properties(wizard-help PROPERTIES PASS_REGULAR_EXPRESSION "--list")
endif()
