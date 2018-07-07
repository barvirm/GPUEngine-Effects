include(CMakeFindDependencyMacro)

if (NOT TARGET Effects)
    find_dependency(glm)
    find_dependency(GPUEngine)
    include(${CMAKE_CURRENT_LIST_DIR}/EffectsTargets.cmake)
endif()

