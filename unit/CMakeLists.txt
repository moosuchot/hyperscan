set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")

set(gtest_SOURCES gtest/gtest-all.cc gtest/gtest.h)
if(NOT XCODE)
    include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})
else()
    set(CMAKE_CXX_FLAGS "-isystem ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CXX_FLAGS}")
endif()
include_directories(${CMAKE_SOURCE_DIR}/util)

# remove some warnings
# cmake's scope means these only apply here

if (CXX_MISSING_DECLARATIONS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations")
endif()

if(CXX_WEAK_VTABLES)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")
endif()

if(CXX_WUNUSED_VARIABLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
endif()

add_library(gtest ${gtest_SOURCES})

add_definitions(-DGTEST_HAS_PTHREAD=0 -DSRCDIR=${CMAKE_SOURCE_DIR})

if (NOT RELEASE_BUILD)
set(unit_internal_SOURCES
    internal/bitfield.cpp
    internal/bitutils.cpp
    internal/charreach.cpp
    internal/compare.cpp
    internal/database.cpp
    internal/depth.cpp
    internal/fdr.cpp
    internal/fdr_flood.cpp
    internal/fdr_loadval.cpp
    internal/flat_set.cpp
    internal/flat_map.cpp
    internal/graph.cpp
    internal/lbr.cpp
    internal/limex_nfa.cpp
    internal/masked_move.cpp
    internal/multi_bit.cpp
    internal/nfagraph_common.h
    internal/nfagraph_comp.cpp
    internal/nfagraph_equivalence.cpp
    internal/nfagraph_find_matches.cpp
    internal/nfagraph_literal_analysis.cpp
    internal/nfagraph_redundancy.cpp
    internal/nfagraph_repeat.cpp
    internal/nfagraph_util.cpp
    internal/nfagraph_width.cpp
    internal/noodle.cpp
    internal/pack_bits.cpp
    internal/parser.cpp
    internal/partial.cpp
    internal/pqueue.cpp
    internal/repeat.cpp
    internal/rose_build_merge.cpp
    internal/rvermicelli.cpp
    internal/sidecar.cpp
    internal/simd_utils.cpp
    internal/shuffle.cpp
    internal/shufti.cpp
    internal/state_compress.cpp
    internal/truffle.cpp
    internal/unaligned.cpp
    internal/unicode_set.cpp
    internal/uniform_ops.cpp
    internal/utf8_validate.cpp
    internal/util_string.cpp
    internal/vermicelli.cpp
    internal/main.cpp
)

add_executable(unit-internal ${unit_internal_SOURCES})
target_link_libraries(unit-internal hs gtest corpusomatic)
endif(NOT RELEASE_BUILD)

set(unit_hyperscan_SOURCES
    hyperscan/allocators.cpp
    hyperscan/arg_checks.cpp
    hyperscan/bad_patterns.cpp
    hyperscan/bad_patterns.txt
    hyperscan/behaviour.cpp
    hyperscan/expr_info.cpp
    hyperscan/extparam.cpp
    hyperscan/identical.cpp
    hyperscan/main.cpp
    hyperscan/multi.cpp
    hyperscan/order.cpp
    hyperscan/scratch_op.cpp
    hyperscan/serialize.cpp
    hyperscan/single.cpp
    hyperscan/som.cpp
    hyperscan/stream_op.cpp
    hyperscan/test_util.cpp
    hyperscan/test_util.h
    )
add_executable(unit-hyperscan ${unit_hyperscan_SOURCES})
if (BUILD_STATIC_AND_SHARED OR BUILD_SHARED_LIBS)
target_link_libraries(unit-hyperscan hs_shared gtest expressionutil)
else()
target_link_libraries(unit-hyperscan hs gtest expressionutil)
endif()

#
# build target to run unit tests
#
add_custom_target(
    unit
    COMMAND bin/unit-internal
    COMMAND bin/unit-hyperscan
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS unit-internal unit-hyperscan
)
