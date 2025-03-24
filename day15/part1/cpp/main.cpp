#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

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
            else if (Map[idx] == 'O')
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
            else if (Map[idx] == 'O')
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
            else if (Map[idx] == 'O')
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
      case 'v':
      {
         if (y < Height - 1)
         {
            int idx = ((y + 1) * Width) + x;
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
            else if (Map[idx] == 'O')
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
      default:
         break;
   }

   return false;
}

int main()
{
   std::ifstream file("../test1.txt");
   //std::ifstream file("../test2.txt");

   if (file.is_open())
   {
      std::string line;
      std::vector<char> map;
      std::vector<char> moves;
      int width = 0;
      int height = 0;
      int start = 0;
      bool read_map = true;

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
      for (size_t i = 0; i < moves.size(); i++)
      {
         move(map, moves[i], start, width, height);
         print_map(map, width);
      }
   }
}
