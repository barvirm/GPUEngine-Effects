
add_subdirectory(geometry)
add_subdirectory(laser)
add_subdirectory(shield)

install(FILES "VisualizationTechnique.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})