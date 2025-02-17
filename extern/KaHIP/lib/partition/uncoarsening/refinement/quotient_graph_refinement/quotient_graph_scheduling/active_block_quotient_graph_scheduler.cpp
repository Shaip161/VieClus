/******************************************************************************
 * active_block_quotient_graph_scheduler.cpp 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *****************************************************************************/

//#include "active_block_quotient_graph_scheduler.h"

#include "extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/active_block_quotient_graph_scheduler.h"


active_block_quotient_graph_scheduler::active_block_quotient_graph_scheduler( const KaHIP::PartitionConfig & config, 
                                                                              QuotientGraphEdges & qgraph_edges, 
                                                                              unsigned int bank_account) :  
                                                                              m_quotient_graph_edges(qgraph_edges) {

        m_is_block_active.resize(config.k);
        for( unsigned int i = 0; i < m_is_block_active.size(); i++) {
                m_is_block_active[i] = true;
        }
         
        m_no_of_active_blocks = config.k;     
        init(); 
}

active_block_quotient_graph_scheduler::~active_block_quotient_graph_scheduler() {
                
}

