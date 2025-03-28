/******************************************************************************
 * random_matching.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/

#ifndef RANDOM_MATCHING_D5YDSMDW
#define RANDOM_MATCHING_D5YDSMDW

//#include "matching.h"

#include "extern/KaHIP/lib/partition/coarsening/matching/matching.h"

class random_matching : public matching {
        public:
                random_matching();
                virtual ~random_matching();

                void match(const KaHIP::PartitionConfig & config, 
                           KaHIP::graph_access & G, 
                           Matching & _matching, 
                           CoarseMapping & coarse_mapping, 
                           NodeID & no_of_coarse_vertices,
                           NodePermutationMap & permutation);
};

#endif /* end of include guard: RANDOM_MATCHING_D5YDSMDW */
