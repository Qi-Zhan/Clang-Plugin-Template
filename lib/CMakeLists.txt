add_library(
  UnusedForLoopVar
  SHARED
  UnusedForLoopVar.cpp
  )

target_include_directories(
  UnusedForLoopVar
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

target_link_libraries(
  UnusedForLoopVar
  "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>"
  )
