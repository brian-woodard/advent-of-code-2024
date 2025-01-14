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

void check_trailhead(int StartIdx, int Score, int Width, int Height)
{
   int x = StartIdx % Width;
   int y = StartIdx / Width;

   printf("check trail at (%d, %d) score %d\n", x, y, Score);
}

int main()
{
   std::ifstream file("../test.txt");

   if (file.is_open())
   {
      std::vector<int> input;
      std::unordered_map<int, std::unordered_map<int, bool>> trailheads;
      int width = 0;
      int height = 0;

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
            assert(width == line.length());
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
            check_trailhead(i, input[i], width, height);
            break;
         }
      }
   }
}
