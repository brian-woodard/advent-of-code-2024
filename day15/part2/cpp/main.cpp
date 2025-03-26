#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

// To get the wider warehouse's map, start with your original map and, for each tile, make the following changes:
// 
// If the tile is #, the new map contains ## instead.
// If the tile is O, the new map contains [] instead.
// If the tile is ., the new map contains .. instead.
// If the tile is @, the new map contains @. instead.

void print_map(std::vector<char>& Map, int Width)
{
   printf("\n");
   for (size_t i = 0; i < Map.size(); i++)
   {
      if (i > 0 && i % Width == 0)
         printf("\n");
      printf("%c", Map[i]);
   }
   printf("\n");
}

bool check(std::vector<char>& Map, char Move, int Position, int Width, int Height, bool CheckNeighbor)
{
   int x = Position % Width;
   int y = Position / Width;

   int pos_left = Position - 1;
   int pos_right = Position + 1;
   bool check_pos = true;

   if (Map[Position] == '.')
   {
      return true;
   }
   else if (Map[Position] == '[')
   {
      if (CheckNeighbor)
         check_pos = check(Map, Move, pos_right, Width, Height, false);
   }
   else if (Map[Position] == ']')
   {
      if (CheckNeighbor)
         check_pos = check(Map, Move, pos_left, Width, Height, false);
   }

   switch (Move)
   {
      case '^':
      {
         if (y > 1 && check_pos)
         {
            int idx = ((y - 1) * Width) + x;
            int idx_left = idx - 1;
            int idx_right = idx + 1;

            if (Map[idx] == '.')
            {
               return true;
            }
            else if (Map[idx] == '[')
            {
               return (check(Map, Move, idx, Width, Height, true));
            }
            else if (Map[idx] == ']')
            {
               return (check(Map, Move, idx, Width, Height, true));
            }
         }
         break;
      }
      case 'v':
      {
         if (y < Height - 1 && check_pos)
         {
            int idx = ((y + 1) * Width) + x;
            int idx_left = idx - 1;
            int idx_right = idx + 1;

            if (Map[idx] == '.')
            {
               return true;
            }
            else if (Map[idx] == '[')
            {
               return (check(Map, Move, idx, Width, Height, true));
            }
            else if (Map[idx] == ']')
            {
               return (check(Map, Move, idx, Width, Height, true));
            }
         }
         break;
      }
      default:
         break;
   }

   return false;
}

bool move(std::vector<char>& Map, char Move, int& Position, int Width, int Height)
{
   int x = Position % Width;
   int y = Position / Width;

   switch (Move)
   {
      case '<':
      {
         if (x > 1)
         {
            int idx = (y * Width) + x - 1;
            if (Map[idx] == '.')
            {
               bool update_position = false;
               if (Map[Position] == '@')
                  update_position = true; 
               Map[idx] = Map[Position];
               Map[Position] = '.';
               if (update_position)
                  Position = idx;
               return true;
            }
            else if (Map[idx] == '[' || Map[idx] == ']')
            {
               if (move(Map, Move, idx, Width, Height))
               {
                  bool update_position = false;
                  if (Map[Position] == '@')
                     update_position = true; 
                  Map[idx] = Map[Position];
                  Map[Position] = '.';
                  if (update_position)
                     Position = idx;
                  return true;
               }
               else
               {
                  return false;
               }
            }
         }
         else
         {
            return false;
         }
         break;
      }
      case '>':
      {
         if (x < Width - 1)
         {
            int idx = (y * Width) + x + 1;
            if (Map[idx] == '.')
            {
               bool update_position = false;
               if (Map[Position] == '@')
                  update_position = true; 
               Map[idx] = Map[Position];
               Map[Position] = '.';
               if (update_position)
                  Position = idx;
               return true;
            }
            else if (Map[idx] == '[' || Map[idx] == ']')
            {
               if (move(Map, Move, idx, Width, Height))
               {
                  bool update_position = false;
                  if (Map[Position] == '@')
                     update_position = true; 
                  Map[idx] = Map[Position];
                  Map[Position] = '.';
                  if (update_position)
                     Position = idx;
                  return true;
               }
               else
               {
                  return false;
               }
            }
         }
         else
         {
            return false;
         }
         break;
      }
      case '^':
      {
         if (y > 1)
         {
            int idx = ((y - 1) * Width) + x;
            int idx_left = idx - 1;
            int idx_right = idx + 1;

            if (!check(Map, Move, idx, Width, Height, true))
            {
               return false;
            }

            if (Map[idx] == '.')
            {
               bool update_position = false;
               if (Map[Position] == '@')
                  update_position = true; 
               Map[idx] = Map[Position];
               Map[Position] = '.';
               if (update_position)
                  Position = idx;
               return true;
            }
            else if (Map[idx] == '[')
            {
               if (move(Map, Move, idx, Width, Height) && move(Map, Move, idx_right, Width, Height))
               {
                  bool update_position = false;
                  if (Map[Position] == '@')
                     update_position = true; 
                  Map[idx] = Map[Position];
                  Map[Position] = '.';
                  if (update_position)
                     Position = idx;
                  return true;
               }
               else
               {
                  return false;
               }
            }
            else if (Map[idx] == ']')
            {
               if (move(Map, Move, idx, Width, Height) && move(Map, Move, idx_left, Width, Height))
               {
                  bool update_position = false;
                  if (Map[Position] == '@')
                     update_position = true; 
                  Map[idx] = Map[Position];
                  Map[Position] = '.';
                  if (update_position)
                     Position = idx;
                  return true;
               }
               else
               {
                  return false;
               }
            }
            else
            {
               return false;
            }
         }
         else
         {
            return false;
         }
         break;
      }
      case 'v':
      {
         if (y < Height - 1)
         {
            int idx = ((y + 1) * Width) + x;
            int idx_left = idx - 1;
            int idx_right = idx + 1;

            if (!check(Map, Move, idx, Width, Height, true))
            {
               return false;
            }

            if (Map[idx] == '.')
            {
               bool update_position = false;
               if (Map[Position] == '@')
                  update_position = true; 
               Map[idx] = Map[Position];
               Map[Position] = '.';
               if (update_position)
                  Position = idx;
               return true;
            }
            else if (Map[idx] == '[')
            {
               if (move(Map, Move, idx, Width, Height) && move(Map, Move, idx_right, Width, Height))
               {
                  bool update_position = false;
                  if (Map[Position] == '@')
                     update_position = true; 
                  Map[idx] = Map[Position];
                  Map[Position] = '.';
                  if (update_position)
                     Position = idx;
                  return true;
               }
               else
               {
                  return false;
               }
            }
            else if (Map[idx] == ']')
            {
               if (move(Map, Move, idx, Width, Height) && move(Map, Move, idx_left, Width, Height))
               {
                  bool update_position = false;
                  if (Map[Position] == '@')
                     update_position = true; 
                  Map[idx] = Map[Position];
                  Map[Position] = '.';
                  if (update_position)
                     Position = idx;
                  return true;
               }
               else
               {
                  return false;
               }
            }
         }
         else
         {
            return false;
         }
         break;
      }
      default:
         break;
   }

   return false;
}

int main()
{
   //std::ifstream file("../test1.txt"); //
   //std::ifstream file("../test2.txt"); // 9021
   //std::ifstream file("../test3.txt"); //
   std::ifstream file("../input.txt"); //

   if (file.is_open())
   {
      std::string line;
      std::vector<char> map;
      std::vector<char> big_map;
      std::vector<char> moves;
      int width = 0;
      int height = 0;
      int start = 0;
      bool read_map = true;
      uint64_t gps_sum = 0;

      while (!file.eof())
      {
         if (file.eof())
            break;

         std::getline(file, line);

         if (read_map)
         {
            if (line.length() > 0)
            {
               if (width == 0)
               {
                  width = line.length();
               }
               else
               {
                  assert(width == line.length());
               }

               for (size_t i = 0; i < line.length(); i++)
               {
                  map.push_back(line[i]);
                  if (line[i] == '@')
                     start = map.size() - 1;
               }

               height++;
            }
            else
            {
               read_map = false;
            }
         }
         else
         {
            for (size_t i = 0; i < line.length(); i++)
               moves.push_back(line[i]);
         }
      }

      printf("%d x %d\n", width, height);
      printf("Number of moves: %ld\n", moves.size());

      print_map(map, width);

      // resize map
      width *= 2;
      printf("%d x %d\n", width, height);
      big_map.reserve(width * height);
      for (size_t i = 0; i < map.size(); i++)
      {
         if (map[i] == '#')
         {
            big_map.push_back('#');
            big_map.push_back('#');
         }
         else if (map[i] == '.')
         {
            big_map.push_back('.');
            big_map.push_back('.');
         }
         else if (map[i] == '@')
         {
            big_map.push_back('@');
            start = big_map.size() - 1;
            big_map.push_back('.');
         }
         else if (map[i] == 'O')
         {
            big_map.push_back('[');
            big_map.push_back(']');
         }
      }

      print_map(big_map, width);
      for (size_t i = 0; i < moves.size(); i++)
      {
         move(big_map, moves[i], start, width, height);
         //print_map(big_map, width);
      }

      print_map(big_map, width);

      // calculate gps sum
      for (size_t i = 0; i < big_map.size(); i++)
      {
         if (big_map[i] == '[')
         {
            int x = i % width;
            int y = i / width;

            gps_sum += (100 * y) + x;
         }
      }

      printf("GPS sum: %ld\n", gps_sum);
   }
}
