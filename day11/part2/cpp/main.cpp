#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include "Stopwatch.h"

void print(const std::vector<uint32_t>& Input)
{
   printf("Stones: ");
   for (size_t i = 0; i < Input.size(); i++)
   {
      printf("%d ", Input[i]);
   }
   printf("\n");
}

uint32_t num_digits(uint32_t Value)
{
   uint32_t digits = 0;

   if (Value == 0)
      return 1;

   while (Value > 0)
   {
      digits++;
      Value /= 10;
   }

   return digits;
}

void split(std::vector<uint32_t>& Input, size_t Index)
{
   uint32_t digits = num_digits(Input[Index]);
   uint32_t value = Input[Index];
   uint32_t value_hi = 0;
   uint32_t value_lo = 0;

   uint32_t mag = 1;

   for (uint32_t i = 0; i < digits; i++)
   {
      if (i == digits / 2)
         mag = 1;

      if (i < digits / 2)
      {
         value_lo += (value % 10) * mag;
         mag *= 10;
         value /= 10;
      }
      else
      {
         value_hi += (value % 10) * mag;
         mag *= 10;
         value /= 10;
      }
   }

   Input[Index] = value_hi;
   Input.push_back(value_lo);
   //Input.insert(Input.begin() + Index + 1, value_lo);
}

void blink(std::vector<uint32_t>& Input)
{
   size_t size = Input.size();
   for (size_t i = 0; i < size; i++)
   {
      if (Input[i] == 0)
      {
         Input[i] = 1;
      }
      else if (num_digits(Input[i]) % 2 != 0)
      {
         Input[i] *= 2024;
      }
      else
      {
         // split into two stones
         split(Input, i);
         //i++;
      }
   }
}

void split(uint64_t& Num1, uint64_t& Num2, uint32_t Digits)
{
   uint64_t value = Num1;
   uint64_t value_hi = 0;
   uint64_t value_lo = 0;

   uint32_t mag = 1;

#if 1
   for (uint32_t i = 0; i < Digits; i++)
   {
      if (i == Digits / 2)
         mag = 1;

      if (i < Digits / 2)
      {
         value_lo += (value % 10) * mag;
         mag *= 10;
         value /= 10;
      }
      else
      {
         value_hi += (value % 10) * mag;
         mag *= 10;
         value /= 10;
      }
   }
#else
   uint32_t half_digits = Digits / 2;

   for (uint32_t i = 0; i < half_digits; i++)
   {
      value_lo += (value % 10) * mag;
      mag *= 10;
      value /= 10;
   }

   mag = 1;
   for (uint32_t i = half_digits; i < Digits; i++)
   {
      value_hi += (value % 10) * mag;
      mag *= 10;
      value /= 10;
   }
#endif

   Num1 = value_hi;
   Num2 = value_lo;
}

uint64_t blink_recursive(int Count, uint64_t Num)
{
   if (Count > 60)
   {
      return 1;
   }

   if (Num != 0)
   {
      uint32_t digits = num_digits(Num);

      if (digits % 2 != 0)
      {
         return blink_recursive(Count + 1, Num * 2024);
      }
      else
      {
         uint64_t result = 0;
         uint64_t num2 = 0;

         // split into two stones
         split(Num, num2, digits);

         result = blink_recursive(Count + 1, Num);
         result += blink_recursive(Count + 1, num2);
         return result;
      }
   }
   else
   {
      return blink_recursive(Count + 1, 1);
   }
}

int main()
{
   std::ifstream file("../small.txt");
   std::vector<uint32_t> input;

   input.reserve(100000000);

   if (file.is_open())
   {
      std::stringstream buffer;

      buffer << file.rdbuf();

      uint32_t num;
      while (buffer >> num)
      {
         input.push_back(num);
      }
   }

   print(input);

   uint64_t result = 0;

#if 0
   uint64_t prev_size = input.size();
   double prev_time = 0.0;
   for (int i = 0; i < 45; i++)
   {
      double curr_time;

      {
         CStopwatch timer(&curr_time);
         blink(input);
      }

      uint64_t size = input.size();

      double size_increase = ((double)size - (double)prev_size) / (double)prev_size;
      prev_size = size;

      double time_increase;
      if (i > 0)
      {
         time_increase = (curr_time - prev_time) / prev_time;
      }
      prev_time = curr_time;

      printf("blink %d %ld (%.1f) time %.2fs (%.1f)\n", i+1, input.size(), size_increase * 100.0, curr_time, time_increase * 100.0);

      //print(input);
   }

   result = input.size();
#else
   result = blink_recursive(1, input[0]);
#endif

   printf("Result: %ld\n", result);
}

// Single threaded 55 blinks with small.txt
// [sim_local@localhost cpp]$ time ./main 
// Stones: 0 
// Result: 5362947711
// 
// real	1m16.187s
// user	1m11.103s
// sys	0m0.004s

// Single threaded 56 blinks with small.txt
// [sim_local@localhost cpp]$ time ./main
// Stones: 0
// Result: 8161193535
// 
// real	1m36.532s
// user	1m31.443s
// sys	0m0.005s

// Single threaded 60 blinks with small.txt
// [sim_local@localhost cpp]$ time ./main
// Stones: 0
// Result: 43369895096
// 
// real	5m5.334s
// user	4m58.856s
// sys	0m0.001s

// TODO: Try multi-threading on input.txt, one thread for each starting stone
