#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdint>

int main()
{
   std::vector<int> list1;
   std::vector<int> list2;
   uint64_t total_distance = 0;

   std::ifstream file("../input.txt");
   if (file.is_open())
   {
      int num1;
      int num2;

      while (1)
      {
         file >> num1 >> num2;

         if (file.eof())
            break;

         list1.push_back(num1);
         list2.push_back(num2);
      }
   }

   assert(list1.size() == list2.size());

   std::sort(list1.begin(), list1.end());
   std::sort(list2.begin(), list2.end());

   for (size_t i = 0; i < list1.size(); i++)
   {
      total_distance += abs(list1[i] - list2[i]);
   }

   std::cout << "total distance: " << total_distance << std::endl;
}
