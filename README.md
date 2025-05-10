Created By:
Vatsal Mehta - vkm31
Tushar Cora Suresh - tc901

Test Plan Overview:

The test plan involves subjecting the two functions to stress tests using various argument scenarios. These scenarios include 1-byte objects, 7-byte objects, validation of assigned values for accuracy and accessibility, and verification of proper functionality in error cases.
Descriptions of Test Scenarios (including arguments):

memtest: Given memtest file.

Test 1: Evaluates the performance of repeatedly allocating and then immediately freeing a very small amount of memory (1 byte).

Test 2: Measures the overhead of allocating and freeing memory for a small amount of memory (1 byte) in a single loop iteration.

Test 3: Simulates dynamic memory usage by randomly choosing between allocating and freeing memory, to test the allocator's behavior under unpredictable patterns.

Test 4: Assesses the memory system's efficiency when handling large blocks of memory, close to the maximum size minus overhead.

Test 5: Checks how the system manages memory across a range of different sizes, to understand its performance with varied allocation sizes.

This comprehensive test plan aims to evaluate the robustness and correctness of the functions under various usage scenarios and potential error conditions.