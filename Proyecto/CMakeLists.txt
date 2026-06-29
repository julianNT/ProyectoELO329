cmake_minimum_required(VERSION 3.19)
project(HMI_Sensores LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Charts)

qt_standard_project_setup()

qt_add_executable(HMI_Sensores
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    sensorchannel.h sensorchannel.cpp
    datasimulator.h datasimulator.cpp
    idatasource.h idatasource.cpp
    serialsource.h serialsource.cpp
    sensorcard.h sensorcard.cpp
)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Charts SerialPort)

target_link_libraries(HMI_Sensores
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Charts
        Qt::SerialPort
)

install(TARGETS HMI_Sensores
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET HMI_Sensores
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})