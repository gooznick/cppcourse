import numpy as np

i = np.int32(0x10000000)

while i > 0:
    i = np.int32(i + i)  # Ensure it remains within int32 range
    print(i)

"""
536870912
1073741824
/home/projects/cppcourse/compiler/examples/overflow/main.py:10: RuntimeWarning: overflow encountered in scalar add
  i = np.int32(i + i)  # Ensure it remains within int32 range
-2147483648
"""