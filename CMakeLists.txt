cmake_minimum_required(VERSION 3.19)
project(ScreenTranslator LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

include_directories(${CMAKE_SOURCE_DIR}/libs/include)
link_directories(${CMAKE_SOURCE_DIR}/libs/lib)

qt_standard_project_setup()

qt6_add_resources(PROJECT_RESOURCES res.qrc)
qt_add_executable(ScreenTranslator
    WIN32 MACOSX_BUNDLE
    app.rc
    main.cpp
    ui/mainwindow.cpp
    ui/mainwindow.h
    ui/mainwindow.ui
    ui/CustomWidgets/draggableframe.h ui/CustomWidgets/draggableframe.cpp
    ${PROJECT_RESOURCES}
    ui/Overlay/OverlayWidget.h ui/Overlay/OverlayWidget.cpp

)


set(BUILD_DIR "${CMAKE_SOURCE_DIR}/build/6_5_3-Release")
set(DLL_DIR "${CMAKE_SOURCE_DIR}/libs/dlls")
file(GLOB DLL_FILES "${DLL_DIR}/*.dll")

add_custom_command(TARGET ScreenTranslator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL_FILES}
        ${BUILD_DIR}
    COMMENT "Copying DLLs to output directory"
)



#qt_add_translations(
#    TARGETS ScreenTranslator
#    TS_FILES ScreenTranslator_en_150.ts
#)

target_link_libraries(ScreenTranslator
    PRIVATE
        Qt::Core
        Qt::Widgets

        tesseract55.lib
        leptonica-1.85.0.lib

        zlib.lib
        libpng16.lib
        jpeg.lib
        tiff.lib
        gif.lib
        openjp2.lib
        libwebp.lib
        libwebpdecoder.lib
        libwebpdemux.lib
        libwebpmux.lib
        libsharpyuv.lib

        user32
        gdi32
        kernel32
        winspool
        shell32
        ole32
        oleaut32
        uuid
        comdlg32
        advapi32
        Dwmapi
)

include(GNUInstallDirs)

install(TARGETS ScreenTranslator
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ScreenTranslator
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
