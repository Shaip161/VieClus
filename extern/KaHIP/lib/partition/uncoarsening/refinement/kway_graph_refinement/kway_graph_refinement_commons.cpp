/******************************************************************************
 * kway_graph_refinement_commons.cpp 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/

#include <omp.h>

//#include "kway_graph_refinement_commons.h"

#include "extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_commons.h"


std::vector<KaHIP::kway_graph_refinement_commons*>* KaHIP::kway_graph_refinement_commons::m_instances = NULL;

KaHIP::kway_graph_refinement_commons::kway_graph_refinement_commons() {

}

KaHIP::kway_graph_refinement_commons::~kway_graph_refinement_commons() {
}


KaHIP::kway_graph_refinement_commons* KaHIP::kway_graph_refinement_commons::getInstance( KaHIP::PartitionConfig & config ) {

        bool created = false;
        #pragma omp critical 
        {
                if( m_instances == NULL ) {
                        m_instances = new std::vector< KaHIP::kway_graph_refinement_commons*>(omp_get_max_threads(), reinterpret_cast<KaHIP::kway_graph_refinement_commons*>(NULL));
                }
        } 

        int id = omp_get_thread_num();
        if((*m_instances)[id] == NULL) {
                (*m_instances)[id] = new KaHIP::kway_graph_refinement_commons();
                (*m_instances)[id]->init(config);
                created = true;
        }

        if(created == false) {
                if(config.k != (*m_instances)[id]->getUnderlyingK()) {
                        //should be a very rare case 
                        (*m_instances)[id]->init(config); 
                }
        }

        return  (*m_instances)[id];
}
