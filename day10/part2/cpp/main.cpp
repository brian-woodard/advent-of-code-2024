#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>

void print_map(const std::vector<int>& Map, int Width, int Height)
{
   for (int y = 0; y < Height; y++)
   {
      for (int x = 0; x < Width; x++)
      {
         int idx = (y * Width) + x;
         std::cout << Map[idx];
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}

void check_trailhead(const std::vector<int>& Map, int Idx, int PrevScore, int Width, int Height, std::unordered_map<int, int>& TrailheadScore)
{
   int x = Idx % Width;
   int y = Idx / Width;
   int score = 0;

   if (x < 0 || x >= Width ||
       y < 0 || y >= Height)
   {
      return;
   }

   score = Map[Idx];

   if (score != PrevScore + 1)
   {
      return;
   }

   if (score == 9)
   {
      // Add to hash map
      if (TrailheadScore.find(Idx) == TrailheadScore.end())
      {
         TrailheadScore[Idx] = 1;
      }
      else
      {
         TrailheadScore[Idx]++;
      }
      return;
   }

   // Check up
   if (y > 0)
      check_trailhead(Map, Idx - Width, score, Width, Height, TrailheadScore);

   // Check down 
   if (y < Height-1)
      check_trailhead(Map, Idx + Width, score, Width, Height, TrailheadScore);

   // Check right 
   if (x < Width-1)
      check_trailhead(Map, Idx + 1, score, Width, Height, TrailheadScore);

   // Check left 
   if (x > 0)
      check_trailhead(Map, Idx - 1, score, Width, Height, TrailheadScore);
}

int main()
{
   std::ifstream file("../test.txt");

   if (file.is_open())
   {
      std::vector<int> input;
      std::unordered_map<int, std::unordered_map<int, int>> trailheads;
      int width = 0;
      int height = 0;
      uint64_t result = 0;

      while (1)
      {
         std::string line;

         std::getline(file, line);
         if (file.eof())
            break;

         if (width == 0)
         {
            width = line.length();
         }
         else
         {
            assert(width == (int)line.length());
         }

         for (const auto& c : line)
         {
            input.push_back((int)(c - '0'));
         }

         height++;
      }

      printf("%d x %d\n", width, height);
      print_map(input, width, height);

      for (size_t i = 0; i < input.size(); i++)
      {
         if (input[i] == 0)
         {
            check_trailhead(input, i, -1, width, height, trailheads[i]);
            //printf("Result: (%d, %d) %d\n", i % width, i / width, trailheads[i].size());

            for (const auto& count : trailheads[i])
            {
               //printf("%d ", count.second);
               result += count.second;
            }
            //printf("\n");
         }
      }

      printf("Result: %d\n", result);
   }
}
