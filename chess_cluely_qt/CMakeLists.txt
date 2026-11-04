cmake_minimum_required(VERSION 3.19)
project(chess_cluely_qt LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(chess_cluely_qt #define the application and its source files
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    chessboard.cpp #compile the board implementation
    chessboard.h #include the board declaration in the project
)

qt_add_resources(chess_cluely_qt "app_resources" #bundle files into the application
    PREFIX "/" #place them under the resource root
    FILES
        assets/icon.png #include the icon image
)

qt_add_translations(
    TARGETS chess_cluely_qt
    TS_FILES chess_cluely_qt_en_US.ts
)

target_link_libraries(chess_cluely_qt
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS chess_cluely_qt
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET chess_cluely_qt
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
