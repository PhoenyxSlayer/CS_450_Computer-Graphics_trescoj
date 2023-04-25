cmake_minimum_required(VERSION 3.0)
project( BasicVulkan )

set(CMAKE_INSTALL_PREFIX .)

set(MASTER_DEPEND "E:/Dependencies" CACHE PATH "Set this if necessary to your directory for dependencies.")	
set(CMAKE_PREFIX_PATH ${MASTER_DEPEND} ${CMAKE_PREFIX_PATH})

#####################################
# Require C++17
#####################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#####################################
# Find necessary libraries
#####################################

find_package(Vulkan REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 3.3 REQUIRED) 
find_package(GLEW REQUIRED)	

add_definitions(-DGLEW_STATIC)

#####################################
# Get general sources
#####################################

include_directories("src/include" ${Vulkan_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

file(GLOB GENERAL_SOURCES    
    "src/lib/*.cpp"  
    "src/lib/*.hpp"  
)

#####################################
# Create executables 
# and install targets
#####################################

set(ALL_LIBRARIES ${Vulkan_LIBRARIES} ${ASSIMP_LIBRARIES} ${ASSIMP_ZLIB} glfw GLEW::glew_s)
 
# HelloWorld
add_executable(HelloWorld ${GENERAL_SOURCES} "./src/app/HelloWorld.cpp")
target_link_libraries(HelloWorld ${ALL_LIBRARIES})
install(TARGETS HelloWorld RUNTIME DESTINATION bin/HelloWorld)

# BasicOpenGL
add_executable(BasicOpenGL ${GENERAL_SOURCES} "./src/app/BasicOpenGL.cpp")
target_link_libraries(BasicOpenGL ${ALL_LIBRARIES})
install(TARGETS BasicOpenGL RUNTIME DESTINATION bin/BasicOpenGL)
install(DIRECTORY shaders/BasicOpenGL DESTINATION bin/BasicOpenGL/shaders)

# VerifyAssimp
#COMMENT THESE
add_executable(VerifyAssimp ${GENERAL_SOURCES} "./src/app/VerifyAssimp.cpp")
target_link_libraries(VerifyAssimp ${ALL_LIBRARIES})
install(TARGETS VerifyAssimp RUNTIME DESTINATION bin/VerifyAssimp)

# VerifyVulkan
add_executable(VerifyVulkan ${GENERAL_SOURCES} "./src/app/VerifyVulkan.cpp")
target_link_libraries(VerifyVulkan ${ALL_LIBRARIES})
install(TARGETS VerifyVulkan RUNTIME DESTINATION bin/VerifyVulkan)

#Assign01
add_executable(Assign01 ${GENERAL_SOURCES} "./src/app/Assign01.cpp")
target_link_libraries(Assign01 ${ALL_LIBRARIES})
install(TARGETS Assign01 RUNTIME DESTINATION bin/Assign01)
install(DIRECTORY shaders/Assign01 DESTINATION bin/Assign01/shaders)

#Assign02
add_executable(Assign02 ${GENERAL_SOURCES} "./src/app/Assign02.cpp")
target_link_libraries(Assign02 ${ALL_LIBRARIES})
install(TARGETS Assign02 RUNTIME DESTINATION bin/Assign02)
install(DIRECTORY shaders/Assign02 DESTINATION bin/Assign02/shaders)

#Assign03
add_executable(Assign03 ${GENERAL_SOURCES} "./src/app/Assign03.cpp")
target_link_libraries(Assign03 ${ALL_LIBRARIES})
install(TARGETS Assign03 RUNTIME DESTINATION bin/Assign03)
install(DIRECTORY shaders/Assign03 DESTINATION bin/Assign03/shaders)

#Assign04
add_executable(Assign04 ${GENERAL_SOURCES} "./src/app/Assign04.cpp")
target_link_libraries(Assign04 ${ALL_LIBRARIES})
install(TARGETS Assign04 RUNTIME DESTINATION bin/Assign04)
install(DIRECTORY shaders/Assign04 DESTINATION bin/Assign04/shaders)

#Assign05
add_executable(Assign05 ${GENERAL_SOURCES} "./src/app/Assign05.cpp")
target_link_libraries(Assign05 ${ALL_LIBRARIES})
install(TARGETS Assign05 RUNTIME DESTINATION bin/Assign05)
install(DIRECTORY shaders/Assign05 DESTINATION bin/Assign05/shaders)

# ProfExercisesMath
add_executable(ProfExercisesMath ${GENERAL_SOURCES} "./src/app/ProfExercisesMath.cpp")
target_link_libraries(ProfExercisesMath ${ALL_LIBRARIES})
install(TARGETS ProfExercisesMath RUNTIME DESTINATION bin/ProfExercisesMath)
install(DIRECTORY shaders/ProfExercisesMath DESTINATION bin/ProfExercisesMath/shaders)

# ProfExercises07
add_executable(ProfExercises07 ${GENERAL_SOURCES} "./src/app/ProfExercises07.cpp")
target_link_libraries(ProfExercises07 ${ALL_LIBRARIES})
install(TARGETS ProfExercises07 RUNTIME DESTINATION bin/ProfExercises07)
install(DIRECTORY shaders/ProfExercises07 DESTINATION bin/ProfExercises07/shaders)
#Exercises03
add_executable(Exercises03 ${GENERAL_SOURCES} "./src/app/Exercises03.cpp")
target_link_libraries(Exercises03 ${ALL_LIBRARIES})
install(TARGETS Exercises03 RUNTIME DESTINATION bin/Exercises03)
install(DIRECTORY shaders/Exercises03 DESTINATION bin/Exercises03/shaders)
