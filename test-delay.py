from src.utils import delayMicroseconds
from src.utils import delay

import time
import timeit

# n = time.time()
# delayMicroseconds(1000000)
# print(time.time() - n)

# n = time.time()
# delay(1000)
# print(time.time() - n)

# n = time.time()
# time.sleep(1)
# print(time.time() - n)

n = 5
r = timeit.timeit(stmt="delayMicroseconds(1000000)", globals=globals(), number=n)
print(r / n)