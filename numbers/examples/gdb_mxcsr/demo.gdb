# Compile with: g++ -g main.cpp
# Run with: gdb -x demo.gdb ./a.out

start

# Step over the first print
next
echo \n--- Default State ---\n
# 'info float' shows the status of x87 FPU and SSE (MXCSR)
info float

# Step over fesetround(FE_DOWNWARD)
next
echo \n--- After fesetround(FE_DOWNWARD) ---\n
info float
# Look for "RC: Down" in MXCSR

quit
