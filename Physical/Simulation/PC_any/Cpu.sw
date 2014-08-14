<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio Version=4.0.17.85 SP?>
<SwConfiguration CpuAddress="SL1" xmlns="http://br-automation.co.at/AS/SwConfiguration">
  <TaskClass Name="Cyclic#1">
    <Task Name="Reject" Source="Reject.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="main" Source="main.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="Entry" Source="Entry.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="CheckRejec" Source="CheckReject.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="Accumulato" Source="Accumulator.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="Camera1" Source="Camera1.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="Printer" Source="Printer.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="Conveyor" Source="Conveyor.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#2" />
  <TaskClass Name="Cyclic#3" />
  <TaskClass Name="Cyclic#4">
    <Task Name="SerialCode" Source="SerialCodes.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="IHM" Source="IHM.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#5" />
  <TaskClass Name="Cyclic#6" />
  <TaskClass Name="Cyclic#7" />
  <TaskClass Name="Cyclic#8" />
  <DataObjects />
  <NcDataObjects />
  <VcDataObjects>
    <VcDataObject Name="Visu" Source="Visu.dob" Memory="UserROM" Language="Vc" WarningLevel="2" Compress="false" />
  </VcDataObjects>
  <Binaries>
    <BinaryObject Name="sysconf" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="ashwd" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="asfw" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="iomap" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arconfig" Source="" Memory="SystemROM" Language="Binary" />
  </Binaries>
  <Libraries>
    <LibraryObject Name="CPP" Source="CPP.lby" Memory="None" Language="ANSIC" Debugging="true" />
    <LibraryObject Name="standard" Source="standard.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="AsHttp" Source="AsHttp.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="runtime" Source="runtime.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="AsIecCon" Source="AsIecCon.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="astime" Source="astime.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="FileIO" Source="FileIO.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="sys_lib" Source="sys_lib.lby" Memory="UserROM" Language="Binary" Debugging="true" />
  </Libraries>
</SwConfiguration>