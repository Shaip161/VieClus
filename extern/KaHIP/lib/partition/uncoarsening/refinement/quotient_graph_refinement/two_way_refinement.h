/******************************************************************************
 * two_way_refinement.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/

#ifndef TWO_WAY_REFINEMENT_INTERFACE_1ZWCSI0J
#define TWO_WAY_REFINEMENT_INTERFACE_1ZWCSI0J

//#include "definitions.h"

#include "lib/definitions.h"

class two_way_refinement{
        public:
                two_way_refinement( ) {};
                virtual ~two_way_refinement() {};

                virtual EdgeWeight perform_refinement(KaHIP::PartitionConfig & config, 
                                                      KaHIP::graph_access & G,
                                                      KaHIP::complete_boundary & boundary, 
                                                      std::vector<NodeID> & lhs_pq_start_nodes, 
                                                      std::vector<NodeID> & rhs_pq_start_nodes,
                                                      boundary_pair * refinement_pair,        
                                                      NodeWeight & lhs_part_weight,
                                                      NodeWeight & rhs_part_weight,
                                                      EdgeWeight & cut,
                                                      bool & something_changed) = 0;

};


#endif /* end of include guard: TWO_WAY_REFINEMENT_INTERFACE_1ZWCSI0J */
