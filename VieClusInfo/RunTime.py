# automatically generated by the FlatBuffers compiler, do not modify

# namespace: VieClusInfo

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class RunTime(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAsRunTime(cls, buf, offset):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = RunTime()
        x.Init(buf, n + offset)
        return x

    # RunTime
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # RunTime
    def IoTime(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Float64Flags, o + self._tab.Pos)
        return 0.0

    # RunTime
    def MappingTime(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(6))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Float64Flags, o + self._tab.Pos)
        return 0.0

    # RunTime
    def TotalTime(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(8))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Float64Flags, o + self._tab.Pos)
        return 0.0

def RunTimeStart(builder): builder.StartObject(3)
def RunTimeAddIoTime(builder, ioTime): builder.PrependFloat64Slot(0, ioTime, 0.0)
def RunTimeAddMappingTime(builder, mappingTime): builder.PrependFloat64Slot(1, mappingTime, 0.0)
def RunTimeAddTotalTime(builder, totalTime): builder.PrependFloat64Slot(2, totalTime, 0.0)
def RunTimeEnd(builder): return builder.EndObject()
