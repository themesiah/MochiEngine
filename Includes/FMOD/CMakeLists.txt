project(fmod)

add_library(fmod SHARED IMPORTED GLOBAL)
add_library(fmodstudio SHARED IMPORTED GLOBAL)


# Core
set_property(TARGET fmod PROPERTY
             IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/win/core/lib/x64/fmod.dll")
set_property(TARGET fmod PROPERTY
             IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/win/core/lib/x64/fmod_vc.lib")
set_property(TARGET fmod PROPERTY
             INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/win/core/inc")
             

# Studio
set_property(TARGET fmodstudio PROPERTY
             IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/win/studio/lib/x64/fmodstudio.dll")
set_property(TARGET fmodstudio PROPERTY
             IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/win/studio/lib/x64/fmodstudio_vc.lib")
set_property(TARGET fmodstudio PROPERTY
             INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/win/studio/inc")