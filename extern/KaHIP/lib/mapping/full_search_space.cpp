/******************************************************************************
 * full_search_space.cpp
 *
 * Source of KaHIP -- Karlsruhe High Quality Graph Partitioning 
 *****************************************************************************/

//#include "full_search_space.h"

#include "extern/KaHIP/lib/mapping/full_search_space.h"

full_search_space::full_search_space(KaHIP::PartitionConfig & config, NodeID number_of_nodes) {
        m_unsucc_tries = 0;
        m_swap_lhs = 0;
        m_swap_rhs = 1;
        m_ub  = number_of_nodes*(number_of_nodes-1);
        m_ub /= 2;
        m_number_of_nodes = number_of_nodes;
}

full_search_space::~full_search_space() {
                
}

