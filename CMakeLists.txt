cmake_minimum_required(VERSION 3.19)
project(HMI_Buceo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Widgets Charts)

qt_standard_project_setup()

set(BACK_SOURCES
    back/src/ConfigReader.cpp
    back/src/DataSimulator.cpp
    back/src/AlarmMonitor.cpp
)

set(FRONT_SOURCES
    front/src/MainWindow.cpp
    front/src/DiverWidget.cpp
)

qt_add_executable(HMI_Buceo
    WIN32 MACOSX_BUNDLE
    main.cpp
    ${BACK_SOURCES}
    ${FRONT_SOURCES}
)

target_include_directories(HMI_Buceo PRIVATE
    ${CMAKE_SOURCE_DIR}/back/include
    ${CMAKE_SOURCE_DIR}/front/include
)

target_link_libraries(HMI_Buceo PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::Charts
)

qt_generate_deploy_app_script(
    TARGET HMI_Buceo
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
