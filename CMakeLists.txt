CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(OCR_PROGRAM)

#CXX STANDARTLARI VE COMPILE MODLARINI YAPILANDIRMA
SET(CMAKE_CXX_STANDARD 17)


#compiler
#set(CMAKE_CXX_COMPILER "clang++" CACHE STRING "C++ compiler" FORCE)

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wall -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} \
-Wextra \
-Wall \
-O3 \
-march=native \
-mtune=native \
-funroll-loops \
-ffast-math \
-fomit-frame-pointer \
-flto \
-fprefetch-loop-arrays \
-fopenmp \
")

#-fno-fast-math
#-ffast-math

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()


#DIZINLERIN YAPILANDIRILMASI
SET(DIZIN_SRC ${CMAKE_SOURCE_DIR}/src)
SET(DIZIN_INCLUDE ${CMAKE_SOURCE_DIR}/include)

SET(PROGRAM_SRC
   
    ${DIZIN_SRC}/main.cpp
    ${DIZIN_SRC}/opRandomMath.cpp

    ${DIZIN_SRC}/model/neuralNetwork.cpp
    ${DIZIN_SRC}/model/loadDataset.cpp
    ${DIZIN_SRC}/model/segment.cpp 

    
    ${DIZIN_SRC}/opPoint/bmpReader.cpp
    ${DIZIN_SRC}/opPoint/brightnessOperation.cpp
    ${DIZIN_SRC}/opPoint/thresholdOperation.cpp
    



)

SET(PROGRAM_INCLUDE

    ${DIZIN_INCLUDE}/debugOCR.hpp 
    ${DIZIN_INCLUDE}/opRandomMath.hpp
    ${DIZIN_INCLUDE}/modelProp.hpp
    

    ${DIZIN_INCLUDE}/model/neuralNetwork.hpp
    ${DIZIN_INCLUDE}/model/loadDataset.hpp
    ${DIZIN_INCLUDE}/model/segment.hpp


    ${DIZIN_INCLUDE}/opPoint/bmpReader.hpp
    ${DIZIN_INCLUDE}/opPoint/brightnessOperation.hpp
    ${DIZIN_INCLUDE}/opPoint/thresholdOperation.hpp



)


#COMPILE ADIMININ YAPILANDIRILMASI
if(WIN32)
    add_executable(
        ${PROJECT_NAME} 
        ${PROGRAM_SRC}
        )

    target_link_libraries(
        ${PROJECT_NAME}
      
    )

elseif(UNIX)
    include_directories(
        ${PROGRAM_INCLUDE}
    )

    add_executable(
            ${PROJECT_NAME} 
            ${PROGRAM_SRC}
)
endif()