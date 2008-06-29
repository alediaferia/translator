project(plasma-translator)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
find_package(Plasma REQUIRED)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories( 
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )

set(translator_SRCS plasma-translator.cpp)

kde4_add_plugin(plasma_applet_translator ${translator_SRCS})
target_link_libraries(plasma_applet_translator
                      ${PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${QT_QTSCRIPT_LIBRARY} )

install(TARGETS plasma_applet_translator
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-translator.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
