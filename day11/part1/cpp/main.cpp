#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

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
   Input.insert(Input.begin() + Index + 1, value_lo);
}

void blink(std::vector<uint32_t>& Input)
{
   for (size_t i = 0; i < Input.size(); i++)
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
         i++;
      }
   }
}

int main()
{
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
      }
   }

   print(input);

   for (int i = 0; i < 25; i++)
   {
      blink(input);
      //print(input);
   }

   // 204469 - too high
   printf("Result: %ld\n", input.size());
}
