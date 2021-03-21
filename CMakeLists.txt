cmake_minimum_required(VERSION 3.0)

SET(KOMPAS_SDK "C:/Program Files/ASCON/KOMPAS-3D v18 Home/SDK")

SET(KOMPAS3DPRINT_VERSION 0.1.0)
add_definitions(-DKOMPAS3DPRINT_VERSION="${KOMPAS3DPRINT_VERSION}")
add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-D_AFXDLL)
add_definitions(-D_WIN32_WINNT=0x0502)

project(Kompas3DPrint VERSION ${KOMPAS3DPRINT_VERSION})

SET(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")

include_directories(${KOMPAS_SDK}/Include)
include_directories(${KOMPAS_SDK}/lib)
include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
  StdAfx.cpp
  BaseEvent.cpp
  DocumentEvent.cpp
  ApplicationEvent.cpp
  Kompas3DPrint.cpp
  Kompas3DPrint.def
  Kompas3DPrint.rc
)

SET (CMAKE_LINK_DEF_FILE_FLAG "/DEF:Kompas3DPrint.def")
add_library(Kompas3DPrint SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${KOMPAS_SDK}/lib64/kAPI2D5.lib)
target_link_libraries(${PROJECT_NAME} ${KOMPAS_SDK}/lib64/kAPI3D5.lib)
target_link_libraries(${PROJECT_NAME} ${KOMPAS_SDK}/lib64/kAPI5.lib)
target_link_libraries(${PROJECT_NAME} ${KOMPAS_SDK}/lib64/kAPI7.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".rtw")

# include(build/cmake/libraries.cmake)
# target_compile_options(Kompas3DPrint PRIVATE -Werror=return-type)
