project(PropertyTest)

# Libraries
#if(USE_QT4)
#    find_package(Qt4 REQUIRED)
#    set(LIBS Qt4::QtTest)
#    find_package(Qt4Sql REQUIRED)
#    list(APPEND LIBS Qt4::Sql)
#else()
#    find_package(Qt5Test REQUIRED)
#    set(LIBS Qt5::Test)
#    find_package(Qt5Sql REQUIRED)
#    list(APPEND LIBS Qt5::Sql)
#endif(USE_QT4)

#find_package(EnergoKodTools REQUIRED)
#list(APPEND LIBS EnergoKodTools)
#find_package(EnergoKodDb REQUIRED)
#list(APPEND LIBS EnergoKodDb)

find_package(EnergoKodProperty)
list(APPEND LIBS EnergoKodProperty)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.h)
file(GLOB RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${RESOURCE_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE Src/)

#set(TEST_NAME_PROJECT TestDb)
#add_custom_target(${TEST_NAME_PROJECT}7z COMMAND 7z a -t7z -mx=9 ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME_PROJECT}.7z ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME_PROJECT})
#add_dependencies(${PROJECT_NAME} ${TEST_NAME_PROJECT}7z)

# This is too late for our needs (on Linux it runs after build just before linking)
#add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
#                   COMMAND 7z a -t7z -mx=9 TestProject.7z ./TestProject)
