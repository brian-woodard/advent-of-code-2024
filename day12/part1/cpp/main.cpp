#include <fstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

struct TSubregion
{
   int Area;
   int Perimeter;
   int Cost;
};

struct TRegion
{
   std::vector<char>       Map;
   std::vector<TSubregion> Subregions;
   int                     Width;
   int                     Height;
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

void calc_area_and_perimeter(TRegion& Region)
{
   bool done = false;

   while (!done)
   {
      TSubregion subregion = {};
      int        idx = -1;
      int        w = Region.Width;
      char       region = 0;

      break;

      for (int i = 0; i < (int)Region.Map.size(); i++)
      {
         if (Region.Map[i] != '.' && Region.Map[i] != 'x')
         {
            region = Region.Map[i];

            subregion.Area++;

            if (i - 1 >= 0 && Region.Map[i - 1] != region)
               subregion.Perimeter++;

            if (i + 1 < (int)Region.Map.size() && Region.Map[i + 1] != region)
               subregion.Perimeter++;

            if (i - w >= 0 && Region.Map[i - w] != region)
               subregion.Perimeter++;

            if (i + w < (int)Region.Map.size() && Region.Map[i + w] != region)
               subregion.Perimeter++;

            idx = i;
            break;
         }
      }

      if (idx == -1)
         break;

      while (idx < (int)Region.Map.size())
      {
         // look right for next char
      }
   }
}

int main()
{
   std::fstream file("../test1.txt"); // 140
   //std::fstream file("../test2.txt"); // 772
   //std::fstream file("../test3.txt"); // 1930

   if (file.is_open())
   {
      std::string line;
      std::unordered_map<char, TRegion> regions;
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
            TRegion region = {};

            region.Map.reserve(map.Map.size());
            for (int i = 0; i < (int)map.Map.size(); i++)
               region.Map.push_back('.');

            region.Map[i] = map.Map[i];
            region.Width = width;
            region.Height = height;

            regions[map.Map[i]] = region;
         }
         else
         {
            regions[map.Map[i]].Map[i] = map.Map[i];
         }
      }

      printf("Regions: %ld\n", regions.size());

      for (auto& [key, val] : regions)
      {
         printf("%c: \n", key);
         print_region(val);

         // Calculate are and perimeter for each sub-region
         calc_area_and_perimeter(val);
      }
   }
}
