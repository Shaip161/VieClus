/******************************************************************************
 * matching.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/

#ifndef MATCHING_QL4RUO3D
#define MATCHING_QL4RUO3D

//#include "data_structure/graph_access.h"
//#include "partition_config.h"

#include "extern/KaHIP/lib/data_structure/graph_access.h"
#include "extern/KaHIP/lib/partition/partition_config.h"

class matching {
        public:
                matching();
                virtual ~matching();

                virtual void match(const KaHIP::PartitionConfig & partition_config, 
                                   KaHIP::graph_access & G, 
                                   Matching & _matching, 
                                   CoarseMapping & mapping, 
                                   NodeID & no_of_coarse_vertices,
                                   NodePermutationMap & permutation) = 0;

                void print_matching(FILE * out, Matching & edge_matching);
};

#endif /* end of include guard: MATCHING_QL4RUO3D */
