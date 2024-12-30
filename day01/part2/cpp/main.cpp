#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <cstdint>
#include <unordered_map>

int main()
{
   std::vector<int> list1;
   std::vector<int> list2;
   uint64_t similarity;
   std::unordered_map<int, uint64_t> similarity_map;

   std::ifstream file("../input.txt");
   
   if (file.is_open())
   {
      while (1)
      {
         int num1;
         int num2;

         file >> num1 >> num2;

         if (file.eof())
            break;

         list1.push_back(num1);
         list2.push_back(num2);

         if (similarity_map.find(num2) == similarity_map.end())
         {
            similarity_map[num2] = 1;
         }
         else
         {
            similarity_map[num2]++;
         }
      }
   }

   assert(list1.size() == list2.size());

   for (auto& num1 : list1)
   {
      if (similarity_map.find(num1) != similarity_map.end())
      {
         similarity += (similarity_map[num1] * num1);
      }
   }

   printf("similarity: %d\n", similarity);
}
