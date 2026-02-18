cmake_minimum_required(VERSION 3.16)

project(gpio_hal_hello VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appgpio_hal_hello
    main.cpp
)

qt_add_qml_module(appgpio_hal_hello
    URI gpio_hal_hello
    QML_FILES
        Main.qml
        SOURCES GpioHal/igpiopin.h
        SOURCES
        SOURCES GpioHal/mockgpiopin.h GpioHal/mockgpiopin.cpp
        SOURCES GpioHal/realgpiopin.h GpioHal/realgpiopin.cpp
        SOURCES GpioHal/gpiofactory.h GpioHal/gpiofactory.cpp
)

# Optional: real GPIO (libgpiod) - off by default for easy desktop build
option(USE_REAL_GPIO "Use real libgpiod (requires libgpiod-dev on target/desktop)" OFF)

if(USE_REAL_GPIO)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBGPIOD REQUIRED libgpiod)
    target_link_libraries(appgpio_hal_hello PRIVATE ${LIBGPIOD_LIBRARIES})
    target_include_directories(appgpio_hal_hello PRIVATE ${LIBGPIOD_INCLUDE_DIRS})
    target_compile_definitions(appgpio_hal_hello PRIVATE USE_REAL_GPIO)
    message(STATUS "Building with REAL GPIO (libgpiod)")
else()
    message(STATUS "Building with MOCK GPIO (perfect for Windows/WSL desktop)")
endif()


target_link_libraries(appgpio_hal_hello
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appgpio_hal_hello
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
