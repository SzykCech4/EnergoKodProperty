cmake_minimum_required(VERSION 3.9.0)

project(EnergoKodProperty VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

################## OPTIONS BEGIN ###################################z

option(STATIC_LIB "Build as static lib or as shared lib." OFF)
option(BUILD_TESTS "Build tests." OFF)
option(WARNINGS_NOT_ALLOWED "Warnings not allowed when compile." OFF)

# Project subdirs
list(APPEND HEADER_SUBDIRS Include Src)
list(APPEND SOURCE_SUBDIRS Src)
list(APPEND UI_SUBDIRS Src)
list(APPEND RESOURCE_SUBDIRS . Src)

# Definitions
list(APPEND DEFINITIONS -DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
list(APPEND DEFINITIONS ENERGO_KOD_EDITOR2_LIBRARY)

# Include dirs
list(APPEND INCLUDE_DIRS "." "./Src" "./Include")
list(APPEND INCLUDE_DIRS ${QtQml_INCLUDE_DIRS})

# Unwanted sources
#list(APPEND EXCLUDE_FILES "Database-1[.]0[.].*")

# Used libraries
#find_package(Qt5Core REQUIRED)
#list(APPEND LIBS Qt5::Core)
#find_package(Qt5Gui REQUIRED)
#list(APPEND LIBS Qt5::Gui)
#find_package(Qt5Widgets REQUIRED)
#list(APPEND LIBS Qt5::Widgets)
##find_package(Qt5Sql REQUIRED)
##list(APPEND LIBS Qt5::Sql)
##find_package(Qt5Network REQUIRED)
##list(APPEND LIBS Qt5::Network)
##find_package(Qt5Qml REQUIRED)
##list(APPEND LIBS Qt5::Qml)
##find_package(Qt5Quick REQUIRED)
##list(APPEND LIBS Qt5::Quick)
##find_package(Qt5QuickControls2 REQUIRED)
##list(APPEND LIBS Qt5::QuickControls2)
##find_package(Qt5WebEngine REQUIRED)
##list(APPEND LIBS Qt5::WebEngine)
##find_package(Qt5WebEngineCore REQUIRED)
##list(APPEND LIBS Qt5::WebEngineCore)
##find_package(Qt5WebEngineWidgets REQUIRED)
##list(APPEND LIBS Qt5::WebEngineWidgets)
#find_package(Qt5TextToSpeech REQUIRED)
#list(APPEND LIBS Qt5::TextToSpeech)
#find_package(Qt5Test REQUIRED)
#list(APPEND LIBS Qt5::Test)

#find_package(KF5SyntaxHighlighting REQUIRED CONFIG)
#list(APPEND LIBS KF5SyntaxHighlighting)

#find_package(EnergoKodTools REQUIRED)
#list(APPEND LIBS EnergoKodTools)

################## OPTIONS END ###################################z

# Enable Qt files support
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)

# Remove unwanted platform files
if(NOT WIN32)
    list(APPEND EXCLUDE_FILES ".*[.]Windows[.]cpp")
endif (NOT WIN32)
if(NOT UNIX)
    list(APPEND EXCLUDE_FILES ".*[.]Linux[.]cpp")
endif (NOT UNIX)

set(CMAKE_DEBUG_POSTFIX .debug)

# List headers
foreach(DIR IN LISTS HEADER_SUBDIRS)
    list(APPEND HEADER_EXPANDED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.h)
    list(APPEND HEADER_EXPANDED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.hpp)
    list(APPEND HEADER_EXPANDED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.hxx)
    list(APPEND HEADER_EXPANDED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.h++)
endforeach()
file(GLOB_RECURSE HEADER_FILES FOLLOW_SYMLINKS ${HEADER_EXPANDED_DIRS})
#message("HEADER_FILES: " ${HEADER_FILES})

# List sources
foreach(DIR IN LISTS SOURCE_SUBDIRS)
    list(APPEND SOURCE_EXPANDED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.c)
    list(APPEND SOURCE_EXPANDED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.cpp)
    list(APPEND SOURCE_EXPANDED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.cxx)
    list(APPEND SOURCE_EXPANDED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.c++)
endforeach()
file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS ${SOURCE_EXPANDED_DIRS})
#message("Source files:" ${SOURCE_FILES})

# List ui files
foreach(DIR IN LISTS UI_SUBDIRS)
    list(APPEND UI_EXPANDED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.ui)
endforeach()
file(GLOB_RECURSE UI_FILES FOLLOW_SYMLINKS ${UI_EXPANDED_DIRS})

# List resource files
foreach(DIR IN LISTS RESOURCE_SUBDIRS)
    list(APPEND RESOURCE_EXPANDED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.qrc)
endforeach()
file(GLOB RESOURCE_FILES ${RESOURCE_EXPANDED_DIRS})

# Remove unwanted files
foreach(EXCLUDE_FILE IN LISTS EXCLUDE_FILES)
    message("EXCLUDE_FILE: ${EXCLUDE_FILE}")
    list(FILTER HEADER_FILES EXCLUDE REGEX ${EXCLUDE_FILE})
    list(FILTER SOURCE_FILES EXCLUDE REGEX ${EXCLUDE_FILE})
    list(FILTER UI_FILES EXCLUDE REGEX ${EXCLUDE_FILE})
    list(FILTER RESOURCE_FILES EXCLUDE REGEX ${EXCLUDE_FILE})
endforeach()

if(STATIC_LIB)
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES} ${RESOURCE_FILES})
    # Propogate the STATIC_LIB define for all consuming stuff
    # Necessary for correct ${PROJECT_NAME}SHARED_EXPORT handling, espacially for windows (cross compilation) builds
    target_compile_definitions(${PROJECT_NAME} PUBLIC -D${PROJECT_NAME}_STATIC_LIB)
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES} ${RESOURCE_FILES})
    set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
endif(STATIC_LIB)


# Add definitions
foreach(DEFINITION IN LISTS DEFINITIONS)
    message("DEFINITION: ${DEFINITION}")
    target_compile_definitions(${PROJECT_NAME} PRIVATE ${DEFINITION})
endforeach()

# Add include dir
foreach(INCLUDE_DIR IN LISTS INCLUDE_DIRS)
    message("INCLUDE_DIR: ${INCLUDE_DIR}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
endforeach()
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME}> $<INSTALL_INTERFACE:include>)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

# g++ compiler flags
if (CMAKE_COMPILER_IS_GNUCXX)
    if(WARNINGS_NOT_ALLOWED)
        message("Warnings not allowed.")
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wformat=2
            -Wuninitialized -Winit-self -Wswitch-enum -Wundef
            -Wpointer-arith -Wdisabled-optimization -Wcast-align -Wcast-qual)
    else()
        message("Warnings allowed.")
        target_compile_options(${PROJECT_NAME} PRIVATE -Wformat=2 -Wall
            -Wuninitialized -Winit-self -Wswitch-enum -Wundef
            -Wpointer-arith -Wdisabled-optimization -Wcast-align -Wcast-qual)
    endif()
endif()

if(BUILD_TESTS)
    add_subdirectory(Tests)
endif(BUILD_TESTS)

# Install
include(GNUInstallDirs)

# Install headers
install(DIRECTORY Include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install exe, library, and archive files - we can't install headers here because we can have directory structure (not just flat subdir with headers)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${CMAKE_LIBRARY_ARCHITECTURE}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE})

# Create and install Cmake Config file
install(EXPORT ${PROJECT_NAME}Config DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/${PROJECT_NAME})

# Create and install Cmake version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/${PROJECT_NAME})

# Create and install pc file
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}/pkgconfig)
