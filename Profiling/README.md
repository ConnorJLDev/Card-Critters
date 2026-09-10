# Profiling
- These are external tools used to profile performance of the executable.

## Windows Performance Analyzer
- To use Windows Performance Analyzer (WPA), the [RunWPR](./RunWPR.cmd) file must be run in administrator mode, as Windows Performance Recorder (WPR) (the Windows executable being called) requires elevated permissions to properly track hardware statistics such as cache misses and context switches.
- The [profiles available](./Profiles.wprp) include:
	- "Misses": Measures cache misses/references and branch mispredictions.
	- "Instructions": Measures aggregate number of instructions issued, instructions retired, and cycles.
	- "Usage": Measures the amount of time (and number of context switches) that the executable was actually running, and includes a "cycles per instruction" metric to measure total average efficiency of the CPU.
- To use a profile, run [RunWPR](./RunWPR.cmd) with the first argument being the desired profile, followed by any arguments to pass to the executable being tested.
- The functionality of some or all profiles may or may not work depending on the hardware being tested, the operating system, and any required programs.
- The generated file (if successful) contains result data that pertains to your system and any programs that were running during the profiling process. As such, *DO NOT COMMIT THESE FILES TO THE REPOSITORY*! Their file type (.etl) has been included in the gitignore as a preventative measure.
- It is strongly recommend to *NOT* share these generated files, and by using these provided files, the end user agrees that any issues, problems, or other situations that arise from sharing provided or generated files are the sole responsibility of the user who shared them.
- To properly view the resulting trace data, the PDB file for the executable must be included in order to view meaningful locations of where measurements were made.
- The [RunWPR](./RunWPR.cmd) and [Profiles](./Profiles.wprp) files were written using the guide on [Authoring Recording Profiles](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/authoring-recording-profiles), the [XML reference](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/recording-profile-xml-reference), and the guide on [Recording PMU Events](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/recording-pmu-events), as well as related pages within the [Windows Hardware Developer/Windows Performance Toolkit](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/) section at the official Microsoft Learn website.
