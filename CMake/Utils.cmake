function(copy_core TARGET_NAME)
# After building the library, copy the core data folder to the output folder
add_custom_command(
  TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/Engine/CoreData"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data/Core"
  COMMENT "Copying data from ${CMAKE_SOURCE_DIR} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

  if (USE_FMOD)
  # After building the library, copy the fmod dlls to the output folder.
  add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/Engine/Includes/FMOD/win/studio/lib/x64/fmodstudio.dll"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmodstudio.dll"
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/Engine/Includes/FMOD/win/core/lib/x64/fmod.dll"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fmod.dll"
    COMMENT "Copying fmodstudio.dll and fmod.dll from ${CMAKE_SOURCE_DIR} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  )
  endif()
endfunction()