#pragma once

// TODO: remove and replace with std:swap
void intSwap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

// TODO: remove and replace with std:swap
void floatSwap(float *a, float *b) {
  float temp = *a;
  *a = *b;
  *b = temp;
}
