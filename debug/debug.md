---
title: debug
author: eranbu
date: 4.2025
marp: true
theme: gaia
paginate: true
---

![bg left width:500px](images/nobugs.jpg)


# Debugging

Some info and technics

---


## 🎯 What Is Debug Info?

- Maps binary addresses → source code/variables
- Needed for:
  - Breakpoints
  - Stack traces
  - Variable inspection


<!--
- Debug info connects machine instructions back to human-readable code.
- Without debug info, debugger only sees raw addresses and assembly.
- Breakpoints and step-debugging are impossible without it.
-->

---

## 🛠️ How Is It Stored?

- Formats:
  - Linux DWARF (Inside binary)
  - Windows PDB (Separate file)
- Can be stripped out after build


<!--
- Debug info is stored in non-executable sections (.debug_info, .pdb).
- Separate debug files are common on Linux (split DWARF).
- You can remove debug info to shrink size, but then you lose easy debugging.
-->

---

## 🛡️ Compiler Flags

- Linux (GCC/Clang):
  ```bash
  -g
  ```
- Windows (MSVC):
  ```bash
  /Zi
  ```

✅ Adds debug symbols to build


<!--
- Always compile with `-g` if you want debugging to be possible.
- On Windows, `/Zi` creates a .pdb file automatically.
- Debug info can be combined with Release optimizations too (`-g -O2`).
-->

---

## 🔥 What Happens Without Debug Info?

- No function names
- No source file names
- No variable names
- Only addresses and assembly


<!--
- You can still run the program — but debugging becomes extremely hard.
- GDB or Visual Studio will show unknown symbols or just raw addresses.
- Setting breakpoints by name will not work.
-->

---

## ✨ Demo

```bash
g++ main.cpp -g -omain_g
readelf --debug-dump=info main_g
strip main_g
readelf --debug-dump=info main_g
```

---


# 🔗 PDB and Executable Link

| What | Where it lives | Why it matters |
|:-----|:---------------|:--------------|
| PDB file name | Embedded in EXE/DLL | Tells debugger where to find PDB |
| GUID | Embedded in EXE/DLL and PDB | Must match to load symbols |
| Age  | Embedded in EXE/DLL and PDB | Tracks small rebuilds |


> **PDB must match executable by name, GUID, and Age — or symbols won't load!**

---

# 📋 Debugging without PDB

* Demo :
  * Windows simple program
  * Compile twice
  * Make debugger run with wrong pdb
  * Try debugging without pdb
    * No variables
    * Can fetch arguments
    * Breakpoints in API functions
    * Breakpoints in Win32/3rd party functions
    * Callstack

---


# 🎯 Breakpoints

✅ Pause execution at:

- A function
- A line
- A memory access (read/write)
- An exception

---

# 🛠️ Breakpoint on Function

- GDB:  
  ```bash
  break my_function
  ```
- MSVC:  
  ➔ Right-click function name → "Breakpoint → Insert Breakpoint"


<!--
- Function breakpoints let you pause as soon as a specific function is called.
- template - make many breakpoints
-->

---

# 🛠️ Breakpoint on Line

- GDB:  
  ```bash
  break myfile.cpp:42
  ```
- MSVC:  
  ➔ Click left margin next to code line

---

<!--
- Line breakpoints are the most common.
- In GDB you specify file and line explicitly.
- In Visual Studio just click left gutter or press F9.
-->

---

# 🛠️ Data Breakpoint (Watchpoint)

✅ Pause when memory changes!

- GDB:
  ```bash
  watch var_name
  ```
- MSVC:  
  ➔ Right-click variable → "Break when value changes"

✅ Read/write possible (`watch`, `rwatch`, `awatch`)

TODO: new MSVC can make read breakpoint ?

<!--
- Watchpoints track memory locations, not just instructions.
- In GDB:
  - `watch var` ➔ Break on write
  - `rwatch var` ➔ Break on read
  - `awatch var` ➔ Break on read or write
- MSVC makes it easy via context menu.
-->

---

# ⚡ Break on Win32 or glibc Functions

| Library    | Useful Breakpoints        |
|:-----------|:---------------------------|
| Windows    | `CreateFile`, `HeapAlloc`, `VirtualAlloc` |
| glibc/Linux| `malloc`, `free`, `open`, `pthread_create` |

✅ Find crashes, allocations, syscalls faster.


<!--
- You can break when the OS API is called.
- E.g., break on `malloc` to catch who is allocating memory.
- In Windows, breaking on `CreateFileA` shows file usage.
- Helpful for deep bugs or performance profiling.
-->

---

# 🛡️ Break on Exceptions

- **GDB**:
  ```bash
  catch throw
  ```
- **MSVC**:  
  ➔ Debug ➔ Windows ➔ Exception Settings ➔ Enable C++ exceptions

✅ Stops when exception is thrown, even before catch.


<!--
- In GDB, `catch throw` pauses at the point an exception is thrown (C++ exceptions).
- Visual Studio has a GUI for enabling break-on-throw.
- Super useful to catch bugs before they are swallowed by catch blocks.
-->

---

# 🛠️ Data Read Breakpoint (Advanced)

- GDB:
  ```bash
  rwatch var
  ```
- MSVC:  
  ➔ Memory breakpoint (set in Memory Window or Watch Window)

✅ Detect reads of uninitialized memory or sensitive data!


<!--
- Normally we break on writes, but sometimes reads are also critical.
- Example: reading stale, invalid, or uninitialized memory.
- GDB's `rwatch` lets you do that easily.
- MSVC allows memory access breakpoints too.
-->

---

# 🎯 Extra Tips

- **Conditional breakpoints**  
  ➔ Only break if condition is true
  ```bash
  break my_function if x == 42
  ```
- **Ignore count**  
  ➔ Skip N hits before stopping
  ```bash
  ignore 1 5
  ```

✅ Powerful for loops or sporadic bugs.


<!--
- Conditional breakpoints and ignore counts save a lot of time.
- Instead of breaking 1000 times inside a loop, break only when x == 42.
- Use `ignore` to skip the first N hits (e.g., the 1000th allocation).
-->

---


# 🧠 Core Dumps: Crash Afterlife

<!--
Core dumps are memory snapshots when programs crash.
They capture memory, registers, call stack at the crash moment.
Allow analyzing the problem later without re-running.
-->

---

# 📦 What is a Core Dump?

- Snapshot of memory + CPU state
- Captured at crash
- Used for post-mortem debugging

✅ Analyze offline without rerunning

<!--
Core dumps are saved binary files.
They allow seeing why a crash happened.
Cannot continue execution, only inspect.
-->

---

# 🛠️ Linux: Enable Core Dumps

- Allow dumps:

```bash
ulimit -c unlimited
```

- Set core file location:

```bash
echo "/tmp/core.%e.%p" | sudo tee /proc/sys/kernel/core_pattern
```

<!--
By default, Linux may block core files with small size limits.
Use ulimit to remove the limit.
core_pattern controls the filename and location.
-->

---

# 🛠️ Linux: Crash Programmatically

```cpp
#include <signal.h>

raise(SIGSEGV); // or abort();
```

✅ Forces core dump creation

<!--
raise(SIGSEGV) triggers a segmentation fault immediately.
abort() triggers abnormal termination.
Both will create a core file if enabled.
-->

---

# 📂 Where is the Core File? (Linux)

- Default: working directory
- Configured: `/tmp/`, `/var/core/`
- Naming: `core`, `core.PID`, `core.exename.PID`

<!--
If no core pattern is set, the core file lands where the program ran.
With pattern, it can be redirected (e.g., /tmp/core.myapp.12345).
-->

---

# 🛠️ Windows: MiniDump (DMP File)

- Use `MiniDumpWriteDump`
- External tool: **procdump**

✅ Creates `.dmp` file for WinDbg or Visual Studio

<!--
Windows doesn't make dumps automatically unless you configure it.
Programs can generate .dmp manually with MiniDumpWriteDump.
Or admins can catch crashes with procdump.
-->

---

# 🛠️ Windows: Create Dump from Code

```cpp
#include <windows.h>
#include <dbghelp.h>

MiniDumpWriteDump(...);
```

✅ Needs linking with `dbghelp.lib`

<!--
MiniDumpWriteDump can capture process memory on request.
Useful for crash handlers or bug reporting systems.
-->

---

# 📚 Post-Mortem Debugging: What You Can Do

✅ View call stack  
✅ Inspect variables  
✅ Analyze CPU registers  
✅ Investigate crash reasons

<!--
You can fully navigate backtrace.
You can see values of variables (if debug symbols exist).
Registers like EIP, ESP can show crash cause.
-->

---

# 🚫 Post-Mortem Debugging: Limitations

❌ Cannot continue running  
❌ Cannot change memory  
❌ May miss some dynamic states

<!--
A dump is frozen in time.
Cannot resume program execution.
If debug info missing, some variables will be invisible.
-->

---

# 🛠️ Core Dump Tools

| Platform | Tool             |
|:---------|:-----------------|
| Linux    | GDB, LLDB         |
| Windows  | Visual Studio, WinDbg, procdump |

✅ Load dumps, inspect crash

<!--
All these tools can load core dumps.
They let you backtrace, inspect memory, variables.
Symbols (debug info) are critical for readable debugging.
-->

---

# 🎯 Summary

- Core dumps = crash snapshots
- Need enabling first
- Symbols make dumps useful
- Powerful tool for real-world debugging

<!--
Core dumps are vital in production systems.
Always compile with debug info, even in Release if you want meaningful crash dumps.
-->

---
---

# 🛠️ Remote Debugging with gdbserver

✅ Debug running programs on remote target  
✅ Control them from your local machine

---

# 🎯 Roles

| Machine | Role |
|:--------|:----|
| Target  | Runs `gdbserver` |
| Host    | Runs `gdb` locally, connects remotely |

✅ Target = small embedded / Linux device  
✅ Host = your laptop / PC

<!--
gdbserver runs on the target to control the program.
gdb runs on your host machine to give you a full UI, source code, etc.
-->

---

# 🛠️ On Target

- Install `gdbserver`
- Copy **binary** to target (not necessarily symbols)
- Start program under `gdbserver`:

```bash
gdbserver :1234 ./myapp
```

✅ Waits for host connection

<!--
gdbserver runs the program and pauses it until a debugger attaches.
The port (e.g., 1234) must be open and reachable.
The binary must match the one on the host.
-->

---

# 🛠️ On Host

- Install `gdb` locally
- Start gdb:

```bash
gdb ./myapp_with_symbols
```

- Connect:

```bash
target remote target_ip:1234
```

✅ Now you control remote program!

<!--
You need the exact same binary, but with full debug symbols.
Connect with 'target remote ip:port'.
Then you can set breakpoints, step, inspect memory.
-->

---

# 📦 Needed Files: Host vs Target

| File                | Target (device) | Host (your PC) |
|:--------------------|:----------------|:--------------|
| Stripped binary      | ✅              | ✅ (debug copy) |
| Full symbols         | ❌              | ✅             |
| Source code          | ❌              | ✅             |

✅ Target keeps it minimal.

<!--
The target has only what it needs to run.
All debug info and source files stay safely on the host.
Saves target disk space and keeps binaries smaller.
-->

---

# 🛠️ Source File Location Mapping

- Tell GDB where sources are:

```bash
set substitute-path /original/path /local/path
```

✅ Fixes mismatch if source was built elsewhere.

<!--
Useful when the binary was built on a different system.
GDB thinks sources are in /build/system/path/src/, but you have them locally elsewhere.
set substitute-path fixes that.
-->

---

# 🛠️ Load Symbols from Unstripped Shared Object

```bash
symbol-file /path/to/unstripped_binary
```

✅ Loads debug info manually

---

```bash
add-symbol-file /path/to/unstripped.so 0xADDRESS
```

✅ For shared objects (need base address)

<!--
symbol-file replaces all symbols at once.
add-symbol-file adds additional symbols without replacing everything.
For shared libraries (SO files), you must specify the load address.
Use info proc mappings to find it.
-->

---

# 🔥 Useful Commands After Connect

| Command                | Purpose                  |
|:------------------------|:-------------------------|
| `info sharedlibrary`    | See loaded .so files      |
| `info files`            | Check symbol file loaded  |
| `info source`           | Show source file paths    |
| `info proc mappings`    | Show memory layout        |

✅ Helps when debugging remote programs.

<!--
info sharedlibrary: See which shared libraries are loaded.
info files: Confirm which symbols are loaded.
info source: Check if GDB knows where sources are.
info proc mappings: Needed to find load addresses for SOs.
-->

---

# 🎯 Summary

- `gdbserver` on target, `gdb` on host
- Host has full symbols and sources
- `symbol-file` or `add-symbol-file` to reload symbols
- `set substitute-path` to fix sources

✅ Full remote control for debugging anywhere!

<!--
Remote debugging lets you debug small systems, embedded devices, even Docker containers.
You don't need heavy setup on the target.
Only gdbserver + minimal binary are needed.
-->

---

# Some more points

## Windbg
## GDB tui
## GDB + Visual studio
## vscode debugging
## rr - Time travel
## Debug child processes
## Disassembly (ABI, registers)
## MS pdbs



---

# 🚀 Advanced Debugging Topics

---

# 🛠️ WinDbg (Windows)

- Microsoft’s low-level debugger
- Analyze `.dmp` files, kernel dumps
- Powerful scripting (`!analyze -v`)

<!--
WinDbg is used for crash analysis, even at OS level (blue screens).
More complex but very powerful.
Common for production crash dumps.
Symbol servers supported automatically.
-->

---

# 🛠️ GDB TUI Mode

- Text User Interface inside GDB
- Enable:

```bash
gdb -tui
Ctrl+X Ctrl+A
```

✅ Source + disassembly side by side

<!--
TUI splits the screen: top shows source, bottom shows GDB command line.
You can step, set breakpoints visually without leaving terminal.
Useful on servers without GUIs.
-->

---

# 🛠️ GDB + Visual Studio

- Visual Studio 2022+ can remote debug via GDB
- Cross-compile on Windows, debug on Linux
- Needs SSH setup

✅ Nice GUI + breakpoints

<!--
Visual Studio now supports connecting to remote Linux systems over SSH.
Compiles C++ projects remotely.
GDB runs on target, Visual Studio acts as the front-end.
-->

---

# 🛠️ VSCode Remote Debugging

- Install **Remote - SSH** extension
- Configure `launch.json`
- GDB runs on target, VSCode controls

✅ Lightweight, flexible, free

<!--
VSCode remote extensions let you edit, compile, and debug entirely over SSH.
Launch.json lets you control GDB arguments.
Very good for embedded and IoT targets.
-->

---

# 🕰️ rr — Time Travel Debugging

- Record program execution:

```bash
rr record ./app
```

- Replay with:

```bash
rr replay
```

✅ Step **backwards** in time

<!--
rr records every memory access and system call.
During replay, you can reverse-step and inspect bugs happening before a crash.
Only works on Linux x86-64 for now.
Revolutionary for heisenbugs and race conditions.
-->

---

# 🧠 Debugging Child Processes

- GDB:

```bash
set follow-fork-mode child
```

- Attach to new PID manually

✅ Control forked processes
✅ MSVC has addon

<!--
By default, GDB follows parent after fork.
set follow-fork-mode child lets you automatically attach to child instead.
Useful for server processes spawning workers.
-->

---

# 🔍 Disassembly + ABI Inspection

- Disassemble:

```bash
disassemble
```

- Show registers:

```bash
info registers
```

✅ See low-level CPU state

<!--
Sometimes source is missing or wrong — disassembly lets you debug anyway.
Registers like RIP, RSP show where the program is.
Essential for post-crash analysis, performance tuning, or malware analysis.
-->

---

# 🛠️ Debugging MS PDBs

- Download Microsoft's PDB


<!--

-->

