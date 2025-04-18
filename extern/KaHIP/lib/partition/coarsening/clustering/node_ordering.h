/******************************************************************************
 * node_ordering.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/


#ifndef NODE_ORDERING_HM1YMLB1
#define NODE_ORDERING_HM1YMLB1

#include <algorithm>

//#include "definitions.h"
//#include "partition_config.h"
//#include "data_structure/graph_access.h"
//#include "tools/random_functions.h"

#include "lib/definitions.h"
#include "extern/KaHIP/lib/partition/partition_config.h"
#include "extern/KaHIP/lib/data_structure/graph_access.h"
#include "extern/KaHIP/lib/tools/random_functions.h"


class node_ordering {
public:
        node_ordering();
        virtual ~node_ordering();

        void order_nodes(const KaHIP::PartitionConfig & config, KaHIP::graph_access & G, std::vector< NodeID > & ordered_nodes) {
                forall_nodes(G, node) {
                        ordered_nodes[node] = node;
                } endfor

                switch( config.node_ordering ) {
                        case RANDOM_NODEORDERING:
                                order_nodes_random(config, G, ordered_nodes);
                             break;
                        case DEGREE_NODEORDERING:
                                order_nodes_degree(config, G, ordered_nodes);
                             break;
                 }
        }

        void order_nodes_random(const KaHIP::PartitionConfig & config, KaHIP::graph_access & G, std::vector< NodeID > & ordered_nodes) { 
                KaHIP::random_functions::permutate_vector_fast(ordered_nodes, false);
        }

        void order_nodes_degree(const KaHIP::PartitionConfig & config,KaHIP:: graph_access & G, std::vector< NodeID > & ordered_nodes) { 
                std::sort( ordered_nodes.begin(), ordered_nodes.end(), 
                           [&]( const NodeID & lhs, const NodeID & rhs) -> bool {
                                return (G.getNodeDegree(lhs) < G.getNodeDegree(rhs));
                           });
        }

 };


#endif /* end of include guard: NODE_ORDERING_HM1YMLB1 */
