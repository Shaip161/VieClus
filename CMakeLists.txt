cmake_minimum_required(VERSION 3.10)
include(CheckCXXCompilerFlag)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_program(CCACHE_PROGRAM ccache)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(CCACHE_PROGRAM)
  message(STATUS "Using compiler cache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()
#project(KaHIP C CXX)
project(KaHIP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# if no build mode is specified build in release mode
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# tweak compiler flags
CHECK_CXX_COMPILER_FLAG(-funroll-loops COMPILER_SUPPORTS_FUNROLL_LOOPS)
if(COMPILER_SUPPORTS_FUNROLL_LOOPS)
  add_definitions(-funroll-loops)
endif()
CHECK_CXX_COMPILER_FLAG(-fno-stack-limit COMPILER_SUPPORTS_FNOSTACKLIMITS)
if(COMPILER_SUPPORTS_FNOSTACKLIMITS)
  add_definitions(-fno-stack-limit)
endif()
CHECK_CXX_COMPILER_FLAG(-Wall COMPILER_SUPPORTS_WALL)
if(COMPILER_SUPPORTS_WALL)
  add_definitions(-Wall)
endif()
CHECK_CXX_COMPILER_FLAG(-march=native COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
  add_definitions(-march=native)
endif()
CHECK_CXX_COMPILER_FLAG(-fpermissive COMPILER_SUPPORTS_FPERMISSIVE)
if(COMPILER_SUPPORTS_FPERMISSIVE)
  add_definitions(-fpermissive)
endif()

# check dependencies
find_package(MPI REQUIRED)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP support detected")
  add_definitions(${OpenMP_CXX_FLAGS})
else()
  message(WARNING "OpenMP not available, activating workaround")
  add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
  set_property(TARGET OpenMP::OpenMP_CXX PROPERTY INTERFACE_COMPILE_OPTIONS "")
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/misc)
endif()

# 64 Bit option
option(64BITMODE "64 bit mode" OFF)
if(64BITMODE)
  add_definitions("-DMODE64BITEDGES")
  add_definitions("-DPOINTER64=1")
endif()

# optimized output
option(OPTIMIZED_OUTPUT "optimized output" OFF)
if(OPTIMIZED_OUTPUT)
  add_definitions("-DKAFFPAOUTPUT")
endif()

# ParHIP

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/app)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/argtable3-3.0.3)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/app)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/io)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/tools)
include_directories(${cpi_SOURCE_DIR})  # Adjust if the actual path is different

set(LIBPADYGRCL_FILES 
lib/clustering/louvainmethod.cpp
lib/clustering/labelpropagation.cpp
lib/clustering/neighborhood.cpp
lib/clustering/coarsening/contractor.cpp
lib/clustering/coarsening/coarsening.cpp
lib/tools/modularitymetric.cpp)
add_library(libpadygrcl OBJECT ${LIBPADYGRCL_FILES})


set(LIBEVAL_FILES
extern/KaHIP/lib/data_structure/graph_hierarchy.cpp
extern/KaHIP/lib/io/graph_io.cpp
extern/KaHIP/lib/tools/quality_metrics.cpp
extern/KaHIP/lib/tools/random_functions.cpp
extern/KaHIP/lib/partition/coarsening/matching/matching.cpp
extern/KaHIP/lib/partition/coarsening/clustering/node_ordering.cpp
extern/KaHIP/lib/partition/coarsening/clustering/size_constraint_label_propagation.cpp
lib/logging/bexception.cpp )
#extern/argtable3-3.0.3/argtable3.c)
add_library(libeval OBJECT ${LIBEVAL_FILES})

set(LIBKAFFPA_SOURCE_FILES
extern/KaHIP/lib/data_structure/graph_hierarchy.cpp
extern/KaHIP/lib/algorithms/strongly_connected_components.cpp
extern/KaHIP/lib/algorithms/topological_sort.cpp
extern/KaHIP/lib/algorithms/push_relabel.cpp
extern/KaHIP/lib/io/graph_io.cpp
extern/KaHIP/lib/tools/quality_metrics.cpp
extern/KaHIP/lib/tools/random_functions.cpp
extern/KaHIP/lib/tools/graph_extractor.cpp
extern/KaHIP/lib/tools/misc.cpp
extern/KaHIP/lib/tools/partition_snapshooter.cpp
extern/KaHIP/lib/partition/graph_partitioner.cpp
extern/KaHIP/lib/partition/w_cycles/wcycle_partitioner.cpp
extern/KaHIP/lib/partition/coarsening/coarsening.cpp
extern/KaHIP/lib/partition/coarsening/contraction.cpp
extern/KaHIP/lib/partition/coarsening/edge_rating/edge_ratings.cpp
extern/KaHIP/lib/partition/coarsening/matching/matching.cpp
extern/KaHIP/lib/partition/coarsening/matching/random_matching.cpp
extern/KaHIP/lib/partition/coarsening/matching/gpa/path.cpp
extern/KaHIP/lib/partition/coarsening/matching/gpa/gpa_matching.cpp
extern/KaHIP/lib/partition/coarsening/matching/gpa/path_set.cpp
extern/KaHIP/lib/partition/coarsening/clustering/node_ordering.cpp
extern/KaHIP/lib/partition/coarsening/clustering/size_constraint_label_propagation.cpp
extern/KaHIP/lib/partition/initial_partitioning/initial_partitioning.cpp
extern/KaHIP/lib/partition/initial_partitioning/initial_partitioner.cpp
extern/KaHIP/lib/partition/initial_partitioning/initial_partition_bipartition.cpp
extern/KaHIP/lib/partition/initial_partitioning/initial_refinement/initial_refinement.cpp
extern/KaHIP/lib/partition/initial_partitioning/bipartition.cpp
extern/KaHIP/lib/partition/initial_partitioning/initial_node_separator.cpp
extern/KaHIP/lib/partition/uncoarsening/uncoarsening.cpp
extern/KaHIP/lib/partition/uncoarsening/separator/area_bfs.cpp
extern/KaHIP/lib/partition/uncoarsening/separator/vertex_separator_algorithm.cpp
extern/KaHIP/lib/partition/uncoarsening/separator/vertex_separator_flow_solver.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/greedy_neg_cycle.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/problem_factory.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/augmented_Qgraph.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/mixed_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/label_propagation_refinement/label_propagation_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/2way_fm_refinement/two_way_fm.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/two_way_flow_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/boundary_bfs.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/flow_solving_kernel/cut_flow_problem_solver.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/most_balanced_minimum_cuts/most_balanced_minimum_cuts.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/complete_boundary.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/partial_boundary.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/quotient_graph_scheduling.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/simple_quotient_graph_scheduler.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/active_block_quotient_graph_scheduler.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_core.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_commons.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/augmented_Qgraph_fabric.cpp 
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/advanced_models.cpp 
extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/multitry_kway_fm.cpp 
extern/KaHIP/lib/partition/uncoarsening/refinement/node_separators/greedy_ns_local_search.cpp 
extern/KaHIP/lib/partition/uncoarsening/refinement/node_separators/fm_ns_local_search.cpp 
extern/KaHIP/lib/partition/uncoarsening/refinement/node_separators/localized_fm_ns_local_search.cpp 
extern/KaHIP/lib/algorithms/cycle_search.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/cycle_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/tabu_search/tabu_search.cpp )
#extern/argtable3-3.0.3/argtable3.c)
add_library(libkaffpa_kahip OBJECT ${LIBKAFFPA_SOURCE_FILES})
#target_link_libraries(libkaffpa_kahip PRIVATE absl::flat_hash_set)

set(ARGTABLE_SOURCE_FILES
  extern/argtable3-3.0.3/argtable3.c)
add_library(argtab_kahip STATIC ${ARGTABLE_SOURCE_FILES})

set(LIBCLUSTERING_SOURCE_FILES
lib/parallel_mh_clustering/parallel_mh_async_clustering.cpp
lib/parallel_mh_clustering/population_clustering.cpp
lib/parallel_mh_clustering/exchange/exchanger_clustering.cpp
lib/tools/graph_communication.cpp
lib/clustering/louvainmethod.cpp
lib/clustering/labelpropagation.cpp
lib/clustering/neighborhood.cpp
lib/clustering/coarsening/coarsening.cpp
lib/clustering/coarsening/contractor.cpp
lib/logging/bexception.cpp
lib/tools/modularitymetric.cpp
lib/tools/mpi_tools.cpp)
add_library(libclustering OBJECT ${LIBCLUSTERING_SOURCE_FILES})
target_include_directories(libclustering PUBLIC ${MPI_CXX_INCLUDE_PATH})

# Link cpi with the required libraries in VieClus
target_link_libraries(libpadygrcl PRIVATE cpi)
target_link_libraries(libkaffpa_kahip PRIVATE cpi)
target_link_libraries(libeval PRIVATE cpi)
target_link_libraries(libclustering PRIVATE cpi)

target_link_libraries(libeval PRIVATE argtab_kahip)
target_link_libraries(libkaffpa_kahip PRIVATE argtab_kahip)

target_include_directories(libpadygrcl PRIVATE ${STXXL_INCLUDE_DIRS})
target_include_directories(libkaffpa_kahip PRIVATE ${STXXL_INCLUDE_DIRS})

# Example: Explicitly setting PUBLIC for visibility to external projects
#target_link_libraries(libkaffpa_kahip PUBLIC ${MPI_CXX_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
#target_link_libraries(libpadygrcl PUBLIC ${MPI_CXX_LIBRARIES} ${OpenMP_CXX_LIBRARIES})

# After defining the OBJECT libraries, create a static library target
add_library(VieClus STATIC
    $<TARGET_OBJECTS:libclustering>
    $<TARGET_OBJECTS:libkaffpa_kahip>
    $<TARGET_OBJECTS:libpadygrcl>
    $<TARGET_OBJECTS:libeval>
)

# Ensure the library is position-independent (if needed)
set_target_properties(VieClus PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Set the C++ standard (if required)
#set_target_properties(VieClus PROPERTIES CXX_STANDARD 20)

target_link_libraries(VieClus
    PUBLIC
    cpi
    argtab_kahip
    ${MPI_CXX_LIBRARIES}
    ${OpenMP_CXX_LIBRARIES}
)

target_include_directories(VieClus
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/app
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/argtable3-3.0.3
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/io
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/tools
    ${cpi_SOURCE_DIR}
)


add_executable(graphchecker extern/KaHIP/app/graphchecker.cpp $<TARGET_OBJECTS:libkaffpa_kahip>)
target_compile_definitions(graphchecker PRIVATE "-DMODE_GRAPHCHECKER")
target_link_libraries(graphchecker flatbuffers ${OpenMP_CXX_LIBRARIES})
install(TARGETS graphchecker DESTINATION bin)

add_executable(evaluator app/evaluator.cpp $<TARGET_OBJECTS:libeval> $<TARGET_OBJECTS:libpadygrcl> )
target_compile_definitions(evaluator PRIVATE "-DMODE_EVALUATOR")
target_include_directories(evaluator PUBLIC ${MPI_CXX_INCLUDE_PATH})
target_link_libraries(evaluator argtab_kahip flatbuffers ${MPI_CXX_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
install(TARGETS evaluator DESTINATION bin)

add_executable(evolutionary_clustering app/evo_clustering.cpp $<TARGET_OBJECTS:libkaffpa_kahip> $<TARGET_OBJECTS:libclustering> )
target_compile_definitions(evolutionary_clustering PRIVATE "-DMODE_KAFFPAE")
target_include_directories(evolutionary_clustering PUBLIC ${MPI_CXX_INCLUDE_PATH})
target_link_libraries(evolutionary_clustering argtab_kahip flatbuffers ${MPI_CXX_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
install(TARGETS evolutionary_clustering DESTINATION bin)

