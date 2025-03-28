/******************************************************************************
 * uncoarsening.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/

#ifndef UNCOARSENING_XSN847F2
#define UNCOARSENING_XSN847F2

//#include "data_structure/graph_hierarchy.h"
//#include "partition_config.h"

#include "extern/KaHIP/lib/data_structure/graph_hierarchy.h"
#include "extern/KaHIP/lib/partition/partition_config.h"

class uncoarsening {
public:
        uncoarsening( );
        virtual ~uncoarsening();
        
        int perform_uncoarsening(const KaHIP::PartitionConfig & config, KaHIP::graph_hierarchy & hierarchy);
        int perform_uncoarsening_cut(const KaHIP::PartitionConfig & config, KaHIP::graph_hierarchy & hierarchy);
        int perform_uncoarsening_nodeseparator(const KaHIP::PartitionConfig & config, KaHIP::graph_hierarchy & hierarchy);
        int perform_uncoarsening_nodeseparator_fast(const KaHIP::PartitionConfig & config, KaHIP::graph_hierarchy & hierarchy);
};


#endif /* end of include guard: UNCOARSENING_XSN847F2 */
