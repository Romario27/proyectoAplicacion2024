
cmake_minimum_required(VERSION 3.10.0)
project(createExample)

add_executable(extractor.x)
target_sources(extractor.x PRIVATE main.cpp)


find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(extractor.x PRIVATE nlohmann_json::nlohmann_json)

set(HDF5DIR "/home/romario/lib/hdf5/1.12.0")
target_include_directories(extractor.x PRIVATE ${HDF5DIR}/include)
target_link_directories(extractor.x PRIVATE ${HDF5DIR}/bin)
target_link_libraries(extractor.x PRIVATE ${HDF5DIR}/lib/libhdf5.so ${HDF5DIR}/lib/libhdf5_cpp.so)


