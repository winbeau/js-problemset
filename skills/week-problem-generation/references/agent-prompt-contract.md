# Agent Prompt Contract

Use the same structure for both local work and delegated work.

Generator prompt must include:

- exact problem markdown path
- exact target path for `gen.cpp`
- accepted input range and output expectations
- deterministic seed requirement
- fixed 20-case distribution
- requirement to write `1.in..20.in` directly from `main()`
- requirement to avoid command-line parameters and external dependencies

Solution prompt must include:

- exact problem markdown path
- exact target path for `std.cpp`
- exact stdin/stdout behavior
- algorithm expectations from the statement or derived solution
- requirement to avoid extra spaces or missing newlines
- requirement to compile alone under C++17

Per-problem expectations:

- keep `gen.cpp` and `std.cpp` independent
- never let a worker edit the other file
- prefer legal, reproducible inputs over clever randomness
- ensure stress inputs are still valid according to the statement

Recommended worker reminder:

- "You are not alone in the codebase. Only edit the file you own. Do not revert other files. Adjust to existing repo state instead of assuming a clean tree."
