file(GLOB source_files ./../../../source/semantics_tests/*.cpp)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

add_executable(semantics_tests ${source_files})
add_custom_command(TARGET semantics_tests POST_BUILD COMMAND semantics_tests)
install(TARGETS semantics_tests CONFIGURATIONS Debug
  DESTINATION $ENV{TEST_INSTALL_DIRECTORY}/debug)
install(TARGETS semantics_tests CONFIGURATIONS Release RelWithDebInfo
  DESTINATION $ENV{TEST_INSTALL_DIRECTORY}/release)
