cmake_minimum_required(VERSION 3.10)
project(GammaAttenuation)

# Encontrar Geant4
find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

# Archivos fuente
file(GLOB sources src/*.cc)

add_executable(gammaAtt ${sources})
target_link_libraries(gammaAtt ${Geant4_LIBRARIES})
