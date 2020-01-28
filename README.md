# `libhoststat`

`libhoststat` is a library intended for use in [Event Store][es] server for collecting statistics from hosts. Since we
use performance counters on Windows, only Linux and macOS are supported by `libhoststat`. The statistics which can be
collected currently are:

- 1, 5 and 15 minute load averages - `get_host_load_averages`
- Free memory - `get_host_free_memory`
- Total memory - `get_host_total_memory`

All other statistics in Event Store are handled by the .NET Core's event tracing mechanism. It is intended that PInvoke
be used to invoke this library, although there is no reason it cannot be used from C or Rust instead if necessary.

All operations return `HOSTSTAT_SUCCESS` if they are successful, `HOSTSTAT_UNAVAILABLE` if the requested statistic is
not available on the given host (e.g. if `/proc` is not readable), and `HOSTSTAT_ERROR` in case of all other errors.

## PInvoke Signatures

```csharp
const int HostStatSuccess = 0;
const int HostStatError = 1;
const int HostStatUnavailable = 2;

[DllImport("hoststat")]
static extern int get_host_total_memory(out ulong totalMemory);

[DllImport("hoststat")]
static extern int get_host_free_memory(out ulong freeMemory);

[StructLayout(LayoutKind.Sequential)]
public struct LoadAverages {
    public double Average1m;
    public double Average5m;
    public double Average15m;
}

[DllImport("hoststat")]
static extern int get_host_load_averages(out LoadAverages memory_size);
```

### Usage Example

```csharp
if (LibHostStat.get_host_total_memory(out var totalMemory) != LibHostStat.HostStatSuccess) {
	throw new InvalidOperationException();
}
Console.WriteLine($"Memory: {memory}");

if (LibHostStat.get_host_free_memory(out var freeMemory) != LibHostStat.HostStatSuccess) {
	throw new InvalidOperationException();
}
Console.WriteLine($"Free Memory: {freeMemory}");

if (LibHostStat.get_host_load_averages(out var lAvg) != LibHostStat.HostStatSuccess) {
	throw new InvalidOperationException();
}
Console.WriteLine($"Load Averages: {lAvg.Average1m} {lAvg.Average5m} {lAvg.Average15m}");
```

[es]: https://github.com/EventStore/EventStore
