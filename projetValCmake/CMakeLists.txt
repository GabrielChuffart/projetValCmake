cmake_minimum_required(VERSION 3.0)

project(projetValCmake)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release) # Si le mode de compilation n'est pas défini, on utilise "Release" par défaut
endif()
# Ajout de l'exécutable
add_executable(projetValCmake projet.cpp)
target_link_libraries(projetValCmake ${CMAKE_SOURCE_DIR}/library/sfml-graphics.lib)
target_link_libraries(projetValCmake ${CMAKE_SOURCE_DIR}/library/sfml-audio.lib)
target_link_libraries(projetValCmake ${CMAKE_SOURCE_DIR}/library/sfml-network.lib)
target_link_libraries(projetValCmake ${CMAKE_SOURCE_DIR}/library/sfml-system.lib)
target_link_libraries(projetValCmake ${CMAKE_SOURCE_DIR}/library/sfml-window.lib)
target_link_libraries(projetValCmake ${CMAKE_SOURCE_DIR}/library/sfml-graphics-d.lib)

include_directories(${CMAKE_SOURCE_DIR}/include)
set_property(TARGET projetValCmake PROPERTY CXX_STANDARD 17)
set_property(TARGET projetValCmake PROPERTY CXX_STANDARD_REQUIRED ON)
message(STATUS "Mode de compilation : ${CMAKE_BUILD_TYPE}")