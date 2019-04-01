#include <iostream>

// Unit Tests
void TestFilter()
{
  float testData[] = {1.2, 1.3, 0.9, 0.7, 1.5, 1.7, 2.0, 1.5, 1.2, 0.9, 0.9, 1.2};
  float answer = (1.2 + 1.3 + 0.9 + 0.7 + 1.5 + 1.7 + 2.0 + 1.5 + 1.2 + 0.9+ 0.9+ 1.2) / testData.length;
  float testAnswer = answer[0];

  for (int i = 1; i < testData.length; i++)
  {
     testAnswer = preflightFilter(i, testAnswer, testData[i])
  }


  return;
}

