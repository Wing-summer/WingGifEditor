cmake_minimum_required(VERSION 3.16)

# project name
project(wing-gif-editor)

# uic/moc/rcc
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

# dependencies
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(ImageMagick REQUIRED COMPONENTS Magick++)
find_package(DtkCore REQUIRED)
find_package(DtkWidget REQUIRED)

list(APPEND INCLUDE_DIRS
		${CMAKE_CURRENT_SOURCE_DIR}/src
        ${ImageMagick_INCLUDE_DIRS}
		${ImageMagick_Magick++_INCLUDE_DIRS}
		)

list(APPEND LINK_LIBS
		${ImageMagick_DIRECTORIES}
		${ImageMagick_Magick++_DIRECTORIES}
		Qt5::Widgets)

# source
file(GLOB_RECURSE SOURCES src/*.cpp)

# resource
file(GLOB_RECURSE RESOURCES resource/*.qrc)

# build target
add_executable(${PROJECT_NAME} ${SOURCES} ${RESOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} SHARED ${LINK_LIBS})
