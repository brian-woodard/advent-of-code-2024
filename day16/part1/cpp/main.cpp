#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

struct Vector
{
   int X;
   int Y;
};

void print_map(const std::vector<char>& Map, int Width, int Height)
{
   printf("\n");
   for (int y = 0; y < Height; y++)
   {
      for (int x = 0; x < Width; x++)
      {
         int idx = (y * Width) + x;
         printf("%c", Map[idx]);
      }
      printf("\n");
   }
   printf("\n");
}

bool walk(std::vector<char>& Map, Vector Position, std::vector<bool>& Seen, std::vector<Vector>& Path, int Width, int Height)
{
   //printf("walk (%d, %d)\n", Position.X, Position.Y);
   if (Position.X < 0 || Position.X >= Width ||
       Position.Y < 0 || Position.Y >= Height)
   {
      return false;
   }

   int idx = (Position.Y * Width) + Position.X;

   if (Map[idx] == '#')
   {
      return false;
   }

   if (Map[idx] == 'E')
   {
      return true;
   }

   if (Seen[idx])
   {
      return false;
   }

   Seen[idx] = true;
   Path.push_back(Position);

   // try up
   if (walk(Map, { Position.X, Position.Y - 1 }, Seen, Path, Width, Height))
   {
      return true;
   }

   // try right 
   if (walk(Map, { Position.X + 1, Position.Y }, Seen, Path, Width, Height))
   {
      return true;
   }

   // try down 
   if (walk(Map, { Position.X, Position.Y + 1 }, Seen, Path, Width, Height))
   {
      return true;
   }

   // try left 
   if (walk(Map, { Position.X - 1, Position.Y }, Seen, Path, Width, Height))
   {
      return true;
   }

   Path.pop_back();

   return false;
}

int main()
{
   std::ifstream file("../test1.txt");

   if (file.is_open())
   {
      std::string line;
      std::vector<char> map;
      std::vector<bool> seen;
      std::vector<Vector> path;
      Vector start;
      int width = 0;
      int height = 0;
      int start_idx = 0;
      int depth = 0;

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

         for (auto c : line)
         {
            map.push_back(c);
            if (c == 'S')
               start_idx = (int)map.size() - 1;
         }

         height++;
      }

      print_map(map, width, height);
      printf("%d x %d\n", width, height);
      printf("Start: %d\n", start_idx);

      seen.reserve(map.size());
      for (size_t i = 0; i < map.size(); i++)
      {
         seen.push_back(false);
      }

      // solve the maze recursively
      start.X = start_idx % width;
      start.Y = start_idx / width;

      walk(map, start, seen, path, width, height);

      // add path to map
      printf("Path size: %ld\n", path.size());
      for (auto& loc : path)
      {
         int idx = (loc.Y * width) + loc.X;
         map[idx] = 'x';
      }

      print_map(map, width, height);
   }
}
