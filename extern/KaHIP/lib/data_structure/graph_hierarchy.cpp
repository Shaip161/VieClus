/******************************************************************************
 * graph_hierarchy.cpp 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *****************************************************************************/

//#include "graph_hierarchy.h"
#include "extern/KaHIP/lib/data_structure/graph_hierarchy.h"

namespace KaHIP {

graph_hierarchy::graph_hierarchy() : m_current_coarser_graph(NULL), 
                                     m_current_coarse_mapping(NULL){

}

graph_hierarchy::~graph_hierarchy() {
        for( unsigned i = 0; i < m_to_delete_mappings.size(); i++) {
                if(m_to_delete_mappings[i] != NULL)
                        delete m_to_delete_mappings[i];
        }

        for( unsigned i = 0; i+1 < m_to_delete_hierachies.size(); i++) {
                if(m_to_delete_hierachies[i] != NULL)
                delete m_to_delete_hierachies[i];
        }
}

void graph_hierarchy::push_back(KaHIP::graph_access * G, CoarseMapping * coarse_mapping) {
        m_the_graph_hierarchy.push(G);
        m_the_mappings.push(coarse_mapping);
	m_to_delete_mappings.push_back(coarse_mapping);
        m_coarsest_graph = G;
}

KaHIP::graph_access* graph_hierarchy::pop_finer_and_project() {
        KaHIP::graph_access* finer = pop_coarsest();

        CoarseMapping* coarse_mapping = m_the_mappings.top(); // mapps finer to coarser nodes
        m_the_mappings.pop(); 

        if(finer == m_coarsest_graph) {
                m_current_coarser_graph = finer;
                finer = pop_coarsest();
                finer->set_partition_count(m_current_coarser_graph->get_partition_count());
               
                coarse_mapping = m_the_mappings.top(); 
                m_the_mappings.pop();
        }
        
        ASSERT_EQ(m_the_graph_hierarchy.size(), m_the_mappings.size());

        //perform projection
        KaHIP::graph_access& fRef = *finer;
        KaHIP::graph_access& cRef = *m_current_coarser_graph;
        forall_nodes(fRef, n) {
                NodeID coarser_node              = (*coarse_mapping)[n];
                PartitionID coarser_partition_id = cRef.getPartitionIndex(coarser_node);
                fRef.setPartitionIndex(n, coarser_partition_id);
        } endfor

        m_current_coarse_mapping = coarse_mapping;
        finer->set_partition_count(m_current_coarser_graph->get_partition_count());
        m_current_coarser_graph = finer;

        return finer;                
}

KaHIP::graph_access* graph_hierarchy::pop_finer_and_project_ns( PartialBoundary & separator ) {
        KaHIP::graph_access* finer = pop_coarsest();

        CoarseMapping* coarse_mapping = m_the_mappings.top(); // mapps finer to coarser nodes
        m_the_mappings.pop(); 

        if(finer == m_coarsest_graph) {
                m_current_coarser_graph = finer;
                finer = pop_coarsest();
                finer->set_partition_count(m_current_coarser_graph->get_partition_count());
               
                coarse_mapping = m_the_mappings.top(); 
                m_the_mappings.pop();
        }
        
        ASSERT_EQ(m_the_graph_hierarchy.size(), m_the_mappings.size());

	separator.clear();
        //perform projection
        KaHIP::graph_access& fRef = *finer;
        KaHIP::graph_access& cRef = *m_current_coarser_graph;
        forall_nodes(fRef, n) {
                NodeID coarser_node              = (*coarse_mapping)[n];
                PartitionID coarser_partition_id = cRef.getPartitionIndex(coarser_node);
                fRef.setPartitionIndex(n, coarser_partition_id);
		if( coarser_partition_id == 2 ) separator.insert(n);
        } endfor

        m_current_coarse_mapping = coarse_mapping;
        finer->set_partition_count(m_current_coarser_graph->get_partition_count());
        m_current_coarser_graph = finer;

        return finer;                
}

CoarseMapping * graph_hierarchy::get_mapping_of_current_finer() {
        return m_current_coarse_mapping; 
}

KaHIP::graph_access* graph_hierarchy::get_coarsest( ) {
        return m_coarsest_graph;                
}

KaHIP::graph_access* graph_hierarchy::pop_coarsest( ) {
        KaHIP::graph_access* current_coarsest = m_the_graph_hierarchy.top(); 
        m_the_graph_hierarchy.pop();
        return current_coarsest;                
}

bool graph_hierarchy::isEmpty( ) {
        ASSERT_EQ(m_the_graph_hierarchy.size(), m_the_mappings.size());
        return m_the_graph_hierarchy.empty();        
}

unsigned int graph_hierarchy::size() {
        return m_the_graph_hierarchy.size();        
}

}