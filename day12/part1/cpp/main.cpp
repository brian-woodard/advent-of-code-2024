#include <fstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

struct TRegion
{
   std::vector<char> Map;
   int               Width;
   int               Height;
};

void print_region(const TRegion& Region)
{
   printf("\n");
   for (int y = 0; y < Region.Height; y++)
   {
      for (int x = 0; x < Region.Width; x++)
      {
         int idx = (y * Region.Width) + x;

         assert(idx >= 0);
         assert(idx < Region.Map.size());

         printf("%c", Region.Map[idx]);
      }
      printf("\n");
   }
   printf("\n");
}

int main()
{
   std::fstream file("../test1.txt");

   if (file.is_open())
   {
      std::string line;
      std::unordered_map<char, std::vector<TRegion>> regions;
      TRegion map = {};
      int width = 0;
      int height = 0;

      while (!file.eof())
      {
         std::getline(file, line);

         if (file.eof())
            break;

         if (width == 0)
         {
            width = line.length();
         }
         else
         {
            assert(width == line.length());
         }

         for (auto& c : line)
            map.Map.push_back(c);

         height++;
      }

      map.Width = width;
      map.Height = height;

      printf("%d x %d\n", width, height);

      print_region(map);

      for (int i = 0; i < (int)map.Map.size(); i++)
      {
         if (regions.count(map.Map[i]) == 0)
         {
            TRegion region;

            region.Map.reserve(map.Map.size());
            for (int i = 0; i < (int)map.Map.size(); i++)
               region.Map.push_back('.');

            region.Map[i] = map.Map[i];
            region.Width = width;
            region.Height = height;

            regions[map.Map[i]].emplace_back(region);
         }
         else
         {
            regions[map.Map[i]][0].Map[i] = map.Map[i];
         }
      }

      printf("Regions: %ld\n", regions.size());

      for (auto& [key, val] : regions)
      {
         printf("%c: \n", key);
         print_region(val[0]);
      }
   }
}
