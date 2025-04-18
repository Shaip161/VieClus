/******************************************************************************
 * initial_refinement.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/

#ifndef INITIAL_REFINEMENT_LDIIF5CG
#define INITIAL_REFINEMENT_LDIIF5CG

//#include "data_structure/graph_access.h"
//#include "partition_config.h"

#include "extern/KaHIP/lib/data_structure/graph_access.h"
#include "extern/KaHIP/lib/partition/partition_config.h"

class initial_refinement {
public:
        initial_refinement( );
        virtual ~initial_refinement();

        int optimize( const KaHIP::PartitionConfig & config, 
                      KaHIP::graph_access & G, 
                      EdgeWeight & initial_cut); 
};


#endif /* end of include guard: INITIAL_REFINEMENT_LDIIF5CG */
