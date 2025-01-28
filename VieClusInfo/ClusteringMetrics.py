# automatically generated by the FlatBuffers compiler, do not modify

# namespace: VieClusInfo

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class ClusteringMetrics(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAsClusteringMetrics(cls, buf, offset):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = ClusteringMetrics()
        x.Init(buf, n + offset)
        return x

    # ClusteringMetrics
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # ClusteringMetrics
    def Score(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Float64Flags, o + self._tab.Pos)
        return 0.0

    # ClusteringMetrics
    def ClusteringAmount(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(6))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Uint64Flags, o + self._tab.Pos)
        return 0

def ClusteringMetricsStart(builder): builder.StartObject(2)
def ClusteringMetricsAddScore(builder, score): builder.PrependFloat64Slot(0, score, 0.0)
def ClusteringMetricsAddClusteringAmount(builder, clusteringAmount): builder.PrependUint64Slot(1, clusteringAmount, 0)
def ClusteringMetricsEnd(builder): return builder.EndObject()
