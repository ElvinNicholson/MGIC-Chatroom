cmake_minimum_required(VERSION 3.14)
project(Chatroom++)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

# set(CMAKE_PREFIX_PATH C:/)
find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

set(SOURCE_FILES src/main.cpp src/Game.cpp src/Game.h src/Networking/Server.cpp src/Networking/Server.h src/Networking/Client.cpp src/Networking/Client.h src/UI/ClientUI.cpp src/UI/ClientUI.h src/UI/Objects/GameObject.cpp src/UI/Objects/GameObject.h src/Networking/MessagePacket.cpp src/Networking/MessagePacket.h)
add_executable(Chatroom++ ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

target_link_libraries (Chatroom++ sfml-graphics sfml-window sfml-system sfml-network sfml-audio)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data/)