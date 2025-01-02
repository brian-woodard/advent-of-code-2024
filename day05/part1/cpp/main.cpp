#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

bool check_in_range(int Key, int Value, const std::unordered_multimap<int, int>& Map)
{
   bool result = false;
   auto range = Map.equal_range(Key);

   for (auto it = range.first; it != range.second; it++)
   {
      if (it->second == Value)
      {
         result = true;
         break;
      }
   }

   return result;
}

int check_order(const std::vector<int>& Pages,
                const std::unordered_multimap<int, int>& BeforeMap,
                const std::unordered_multimap<int, int>& AfterMap)
{
   int result = 0;
   bool found = true;

   for (size_t i = 0; i < Pages.size(); i++)
   {
      for (size_t j = 0; j < i; j++)
      {
         if (!check_in_range(Pages[i], Pages[j], AfterMap))
         {
            found = false;
         }

         if (!found)
            break;
      }

      for (size_t j = i+1; j < Pages.size(); j++)
      {
         if (!check_in_range(Pages[i], Pages[j], BeforeMap))
         {
            found = false;
         }

         if (!found)
            break;
      }

      if (!found)
         break;
   }

   if (found)
   {
      int mid = Pages.size() / 2;
      result = Pages[mid];
   }

   return result;
}

int main()
{
   std::ifstream file("../input.txt");

   if (file.is_open())
   {
      std::string line;
      bool store_rules = true;
      std::unordered_multimap<int, int> before_map;
      std::unordered_multimap<int, int> after_map;
      std::vector<std::vector<int>> print;
      int result = 0;

      while (1)
      {
         std::getline(file, line);
         if (file.eof())
            break;

         if (store_rules)
         {
            if (line.length() == 0)
            {
               store_rules = false;
            }
            else
            {
               std::stringstream str_stream(line);
               std::string split;
               int rule1 = -1;
               int rule2 = -1;

               std::getline(str_stream, split, '|');
               rule1 = std::atoi(split.c_str());
               std::getline(str_stream, split, '|');
               rule2 = std::atoi(split.c_str());

               if (rule1 != -1 && rule2 != -1)
               {
                  before_map.insert({rule1, rule2});
                  after_map.insert({rule2, rule1});
               }
            }
         }
         else
         {
            std::vector<int> pages;
            std::stringstream str_stream(line);
            std::string split;

            while (std::getline(str_stream, split, ','))
            {
               pages.push_back(std::atoi(split.c_str()));
            }

            print.push_back(pages);
         }
      }

      //auto range = before_map.equal_range(75);
      //printf("75 before --------------\n");
      //for (auto it = range.first; it != range.second; it++)
      //{
      //   printf("%d -> %d\n", it->first, it->second);
      //}

      //range = after_map.equal_range(75);
      //printf("75 after ---------------\n");
      //for (auto it = range.first; it != range.second; it++)
      //{
      //   printf("%d -> %d\n", it->first, it->second);
      //}

      for (auto& pages : print)
      {
         result += check_order(pages, before_map, after_map);
      }

      printf("Result: %d\n", result);
   }
}
