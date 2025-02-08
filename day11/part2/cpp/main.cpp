#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cmath>
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
#if 0
   uint32_t digits = 0;

   if (Value == 0)
      return 1;

   while (Value > 0)
   {
      digits++;
      Value /= 10;
   }

   return digits;
#else
   if (Value < 10)
      return 1;
   else if (Value < 100)
      return 2;
   else if (Value < 1000)
      return 3;
   else if (Value < 10000)
      return 4;
   else if (Value < 100000)
      return 5;
   else if (Value < 1000000)
      return 6;
   else if (Value < 10000000)
      return 7;
   else if (Value < 100000000)
      return 8;
   else if (Value < 1000000000)
      return 9;
   else if (Value < 10000000000)
      return 10;
#endif
}

void split(uint64_t& Num1, uint64_t& Num2, uint32_t Digits)
{
   uint64_t value = Num1;
   uint64_t value_hi = 0;
   uint64_t value_lo = 0;

   uint32_t mag = 1;

#if 0
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
   //uint32_t half_digits = Digits / 2;
   uint32_t half_digits = Digits >> 1;

   for (uint32_t i = 0; i < half_digits; i++)
   {
      mag *= 10;
   }

   value_hi = value / mag;
   value_lo = value % mag;
#endif

   Num1 = value_hi;
   Num2 = value_lo;
}

uint64_t max_blinks;
std::unordered_map<uint64_t, uint64_t> stones[2];
int in = 0;
int out = 1;

void blink(uint64_t Num)
{
   uint64_t new_value1;
   uint64_t new_value2;

   if (Num != 0)
   {
      uint32_t digits = num_digits(Num);

      if (digits & 0x1 != 0)
      {
         new_value1 = Num * 2024;
         if (stones[out].count(new_value1) == 0)
            stones[out][new_value1] = stones[in][Num];
         else
            stones[out][new_value1] += stones[in][Num];
      }
      else
      {
         // split into two stones
         new_value1 = Num;
         split(new_value1, new_value2, digits);

         if (stones[out].count(new_value1) == 0)
            stones[out][new_value1] = stones[in][Num];
         else
            stones[out][new_value1] += stones[in][Num];

         if (stones[out].count(new_value2) == 0)
            stones[out][new_value2] = stones[in][Num];
         else
            stones[out][new_value2] += stones[in][Num];
      }
      stones[in][Num] = 0;
   }
   else
   {
      if (stones[out].count(1) == 0)
         stones[out][1] = stones[in][0];
      else
         stones[out][1] += stones[in][0];
      stones[in][0] = 0;
   }
}

int main()
{
   printf("Start blinking...\n");

   double time_sec = 0.0;
   uint64_t result = 0;
   max_blinks = 75;
   std::ifstream file("../input.txt");
   std::vector<uint32_t> input;

   if (file.is_open())
   {
      std::stringstream buffer;

      buffer << file.rdbuf();

      uint32_t num;
      while (buffer >> num)
      {
         input.push_back(num);
         stones[in][num] = 1;
      }
   }

   print(input);

   {
      CStopwatch timer(&time_sec);

      for (size_t i = 0; i < max_blinks; i++)
      {
         for (auto& stone : stones[in])
            blink(stone.first);

         in = 1 - in;
         out = 1 - out;
      }

      for (auto& stone : stones[in])
         result += stone.second;
   }

   // 233510865551799 - too high
   printf("Result: %ld (%ld blinks) took %d minutes %.3f secs\n",
      result,
      max_blinks,
      (int)time_sec / 60,
      fmod(time_sec, 60.0));
}
