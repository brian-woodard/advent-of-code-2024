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

void calc_area_and_perimeter_recursive(TRegion& Region, TSubregion& Subregion, int Idx, char Value)
{
   int w = Region.Width;
   int x = Idx % Region.Width;
   int y = Idx / Region.Width;

   Region.Map[Idx] = 'x';
   Subregion.Area++;

   // Check left
   if (x - 1 >= 0)
   {
      if (Region.Map[Idx - 1] == Value)
      {
         calc_area_and_perimeter_recursive(Region, Subregion, Idx - 1, Value);
      }
      else if (Region.Map[Idx - 1] == '.')
      {
         Subregion.Perimeter++;
      }
   }
   else
   {
      Subregion.Perimeter++;
   }

   // Check right
   if (x + 1 < Region.Width)
   {
      if (Region.Map[Idx + 1] == Value)
      {
         calc_area_and_perimeter_recursive(Region, Subregion, Idx + 1, Value);
      }
      else if (Region.Map[Idx + 1] == '.')
      {
         Subregion.Perimeter++;
      }
   }
   else
   {
      Subregion.Perimeter++;
   }

   // Check up
   if (y - 1 >= 0)
   {
      if (Region.Map[Idx - w] == Value)
      {
         calc_area_and_perimeter_recursive(Region, Subregion, Idx - w, Value);
      }
      else if (Region.Map[Idx - w] == '.')
      {
         Subregion.Perimeter++;
      }
   }
   else
   {
      Subregion.Perimeter++;
   }

   // Check down
   if (y + 1 < Region.Height)
   {
      if (Region.Map[Idx + w] == Value)
      {
         calc_area_and_perimeter_recursive(Region, Subregion, Idx + w, Value);
      }
      else if (Region.Map[Idx + w] == '.')
      {
         Subregion.Perimeter++;
      }
   }
   else
   {
      Subregion.Perimeter++;
   }
}

void calc_area_and_perimeter(TRegion& Region)
{
   bool done = false;

   while (!done)
   {
      TSubregion subregion = {};
      int        idx = -1;

      for (int i = 0; i < (int)Region.Map.size(); i++)
      {
         if (Region.Map[i] != '.' && Region.Map[i] != 'x')
         {
            calc_area_and_perimeter_recursive(Region, subregion, i, Region.Map[i]);

            subregion.Cost = subregion.Area * subregion.Perimeter;
            Region.Subregions.push_back(subregion);

            idx = i;
            break;
         }
      }

      if (idx == -1)
         break;
   }
}

int main()
{
   //std::fstream file("../test1.txt"); // 140
   //std::fstream file("../test2.txt"); // 772
   //std::fstream file("../test3.txt"); // 1930
   //std::fstream file("../test4.txt"); // 132
   std::fstream file("../input.txt");

   if (file.is_open())
   {
      std::string line;
      std::unordered_map<char, TRegion> regions;
      TRegion map = {};
      int width = 0;
      int height = 0;
      uint64_t cost = 0;

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

      //print_region(map);

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
         //printf("%c: \n", key);
         //print_region(val);

         // Calculate are and perimeter for each sub-region
         calc_area_and_perimeter(val);

         //print_region(val);

         for (const auto& subregion : val.Subregions)
         {
            //printf("Subregion cost %d (%d x %d)\n",
            //       subregion.Cost,
            //       subregion.Area,
            //       subregion.Perimeter);

            cost += subregion.Cost;
         }
      }

      printf("Total cost: %ld\n", cost);
   }
}
