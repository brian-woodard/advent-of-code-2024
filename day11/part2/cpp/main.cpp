#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <cstdint>

uint64_t splits = 0;

void print(const std::vector<uint64_t>& Input)
{
   printf("Stones: ");
   for (const auto& num : Input)
   {
      printf("%d ", num);
   }
   printf("\n");
}

uint64_t num_digits(uint64_t Value)
{
   uint64_t digits = 0;

   if (Value == 0)
      return 1;

   while (Value > 0)
   {
      digits++;
      Value /= 10;
   }

   return digits;
}

void split(std::vector<uint64_t>& Input, uint64_t& Num, size_t Index)
{
   uint64_t digits = num_digits(Num);
   uint64_t value = Num;
   uint64_t value_hi = 0;
   uint64_t value_lo = 0;

   uint64_t mag = 1;

   for (uint64_t i = 0; i < digits; i++)
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

   Num = value_hi;
   //Input.insert(std::next(Input.begin(), Index), value_lo); // std::list
   Input.insert(Input.begin() + Index + 1, value_lo); // std::vector
   splits++;
}

void blink(std::vector<uint64_t>& Input)
{
   //size_t i = 0;
   //for (auto& num : Input)
   for (size_t i = 0; i < Input.size(); i++)
   {
      uint64_t& num = Input[i];
      if (num == 0)
      {
         num = 1;
      }
      else if (num_digits(num) % 2 != 0)
      {
         num *= 2024;
      }
      else
      {
         // split into two stones
         split(Input, num, i);
         i++;
      }
   }
}

int main()
{
   std::ifstream file("../input.txt");
   std::vector<uint64_t> input;

   if (file.is_open())
   {
      std::stringstream buffer;

      buffer << file.rdbuf();

      uint64_t num;
      while (buffer >> num)
      {
         input.push_back(num);
      }
   }

   print(input);

   for (int i = 0; i < 27; i++)
   {
      blink(input);
      printf("blink %d (size %d splits %d)\n", i+1, input.size(), splits);
      splits = 0;
      //print(input);
   }

   printf("Result: %ld\n", input.size());
}
