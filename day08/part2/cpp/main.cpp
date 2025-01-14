#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>

void find_antinodes(std::unordered_map<int, bool>& antinodes, int idx1, int idx2, int width, int height)
{
   int x1 = idx1 % width;
   int y1 = idx1 / width;
   int x2 = idx2 % width;
   int y2 = idx2 / width;

   int dist_x = x2 - x1;
   int dist_y = y2 - y1;

   int new_x1 = x1 - dist_x;
   int new_y1 = y1 - dist_y;
   int new_x2 = x2 + dist_x;
   int new_y2 = y2 + dist_y;

   antinodes[idx1] = true;
   antinodes[idx2] = true;

   while (new_x1 >= 0 && new_x1 < width &&
          new_y1 >= 0 && new_y1 < height)
   {
      int idx = (new_y1 * width) + new_x1;
      antinodes[idx] = true;

      new_x1 -= dist_x;
      new_y1 -= dist_y;
   }

   while (new_x2 >= 0 && new_x2 < width &&
          new_y2 >= 0 && new_y2 < height)
   {
      int idx = (new_y2 * width) + new_x2;
      antinodes[idx] = true;

      new_x2 += dist_x;
      new_y2 += dist_y;
   }
}

int main()
{
   std::ifstream file("../input.txt");
   int width = 0;
   int height = 0;
   std::vector<char> input;
   std::unordered_map<char, std::vector<int>> nodes;
   std::unordered_map<int, bool> total_antinodes_on_map;

   if (file.is_open())
   {
      while (1)
      {
         std::string line;

         std::getline(file, line);
         if (file.eof())
            break;

         printf("%s\n", line.c_str());

         if (width == 0)
         {
            width = line.length();
         }
         else
         {
            assert(width == line.length());
         }

         for (auto& c : line)
         {
            input.push_back(c);
         }

         height++;
      }

      printf("%d x %d\n", width, height);

      for (int y = 0; y < height; y++)
      {
         for (int x = 0; x < width; x++)
         {
            int idx = (y * width) + x;
            if (input[idx] != '.')
            {
               nodes[input[idx]].push_back(idx);

               if (nodes[input[idx]].size() > 1)
               {
                  int n = nodes[input[idx]].size() - 1;
                  for (size_t i = 0; i < n; i++)
                  {
                     find_antinodes(total_antinodes_on_map,
                                    nodes[input[idx]][i],
                                    nodes[input[idx]][n],
                                    width,
                                    height);
                  }
               }
            }
         }
      }

      printf("Result: %d\n", total_antinodes_on_map.size());
   }
}
