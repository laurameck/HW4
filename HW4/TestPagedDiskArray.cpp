//
//  TestPagedDiskArray.cpp
//  COMP2355 Winter 2015 HW4
//
#include "PagedDiskArray.h"
#include "TestPagedDiskArray.h"
#include <iostream>

// TestPagedDiskArrray - run tests on the PagedDiskArray class with specified
//    page size and number of pages.
//
// Print error messages when errors are detected.  Note that uint8_t values to
// be printed are wrapped with "unsigned()" so that they will be interpreted by
// the output stream as unisgned numbers rather than arbitrary character values.

void TestPagedDiskArray(size_t pageSize, size_t numPages) {
  std::cout << "Testing page size " << pageSize << ", num pages = " << numPages << "\n";
  srand(1);  // reset pseudo-random number generator
  
  // Create a disk array
  const char fileName[] = "disk_array.dat";
  PagedDiskArray d(pageSize, numPages, fileName);
  
  // Create some test data to write and read (don't run this test with a really big array,
  // since the test data is all in memory)
  size_t arraySize = pageSize * numPages;
  uint8_t *testData = new uint8_t[arraySize];
  for (size_t i = 0; i < arraySize; ++i) {
    testData[i] = rand() & 0xFF;  // set test data to some semi-random value
  }
  
  // Set and read back every array entry
  for (size_t i = 0; i < arraySize; ++i) {
    d.set(i, testData[i]);
    uint8_t valueRead = d[i];
    if (valueRead != testData[i]) {
      std::cerr << "ERROR: during writing, d[" << i << "] = " << unsigned(valueRead)
      << ", expected " << unsigned(testData[i]) << "\n";
    }
  }
  
  // Read back entries sequentially, then change each entry
  for (size_t i = 0; i < arraySize; ++i) {
    uint64_t valueRead = d[i];
    if (valueRead != testData[i]) {
      std::cerr << "ERROR: during read back, d[" << i << "] = " << unsigned(valueRead)
      << ", expected " << unsigned(testData[i]) << "\n";
    }
    testData[i] = rand() & 0xFF;  // set test data to different semi-random value
    d.set(i, testData[i]);
  }

  // Generate 100 pseudo-random locations in file
  const size_t numLocations = 100;
  size_t locations[numLocations];
  for (size_t i = 0; i < numLocations; ++i) {
    locations[i] = (rand() % numPages) * pageSize + (rand() % pageSize);
  }

  // Write the 100 locations in the file, then read them back
  for (size_t i = 0; i < numLocations; ++i ) {
    testData[locations[i]] = rand() & 0xFF;  // set test data to different semi-random value
    d.set(locations[i], testData[locations[i]]);
  }
  for (size_t i = 0; i < numLocations; ++i ) {
    uint8_t valueRead = d[locations[i]];
    uint8_t valueExpected = testData[locations[i]];
    if (valueRead != testData[locations[i]]) {
      std::cerr << "ERROR: during random write/read at location " << locations[i] << ", read "
                << unsigned(valueRead) << ", expected " << unsigned(valueExpected) << "\n";
    }
  }

  std::cout << "Test finished for page size " << pageSize << ", num pages = " << numPages << "\n";
}
