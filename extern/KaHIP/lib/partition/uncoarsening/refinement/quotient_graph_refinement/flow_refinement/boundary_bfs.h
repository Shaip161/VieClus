/******************************************************************************
 * boundary_bfs.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *****************************************************************************/

#ifndef BOUNDARY_BFS_4AJLJJAB
#define BOUNDARY_BFS_4AJLJJAB

//#include "data_structure/graph_access.h"
//#include "partition_config.h"

#include "extern/KaHIP/lib/partition/partition_config.h"
#include "extern/KaHIP/lib/data_structure/graph_access.h"

class boundary_bfs {
        public:
                boundary_bfs( );
                virtual ~boundary_bfs();

                bool boundary_bfs_search(KaHIP::graph_access & G, 
                                         std::vector<NodeID> & start_nodes, 
                                         PartitionID partition, 
                                         NodeWeight upper_bound_no_nodes, 
                                         std::vector<NodeID> & reached_nodes,
                                         NodeWeight & stripe_weight, 
                                         bool flow_tiebreaking);
};


#endif /* end of include guard: BOUNDARY_BFS_4AJLJJAB */
