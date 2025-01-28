# automatically generated by the FlatBuffers compiler, do not modify

# namespace: VieClusInfo

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class MemoryConsumption(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAsMemoryConsumption(cls, buf, offset):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = MemoryConsumption()
        x.Init(buf, n + offset)
        return x

    # MemoryConsumption
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # MemoryConsumption
    def OverallMaxRss(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Int64Flags, o + self._tab.Pos)
        return 0

def MemoryConsumptionStart(builder): builder.StartObject(1)
def MemoryConsumptionAddOverallMaxRss(builder, overallMaxRss): builder.PrependInt64Slot(0, overallMaxRss, 0)
def MemoryConsumptionEnd(builder): return builder.EndObject()
