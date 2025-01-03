#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

enum TMoveState
{
   MOVE_UP,
   MOVE_RIGHT,
   MOVE_DOWN,
   MOVE_LEFT,
   FINISHED,
};

void print_room(const std::vector<char>& Room, int Width, int Height)
{
   printf("\nRoom:\n");
   for (int y = 0; y < Height; y++)
   {
      for (int x = 0; x < Width; x++)
      {
         int idx = (y * Width) + x;
         printf("%c", Room[idx]);
      }
      printf("\n");
   }
}

bool move(std::vector<char>& Room, int Width, int Height, int& Idx, TMoveState& State)
{
   int x = Idx % Width;
   int y = Idx / Width;
   bool result = (Room[Idx] != 'X');

   switch (State)
   {
      case MOVE_UP:
         Room[Idx] = 'X';

         // move up
         y--;

         // new index
         Idx = (y * Width) + x;

         // test square
         if (y < 0)
         {
            State = FINISHED;
            return result;
         }
         else if (Room[Idx] == '#')
         {
            y++;
            Idx = (y * Width) + x;
            State = MOVE_RIGHT;
            return result;
         }
         break;
      case MOVE_RIGHT:
         Room[Idx] = 'X';

         // move right
         x++;

         // new index
         Idx = (y * Width) + x;

         // test square
         if (x >= Width)
         {
            State = FINISHED;
            return result;
         }
         else if (Room[Idx] == '#')
         {
            x--;
            Idx = (y * Width) + x;
            State = MOVE_DOWN;
            return result;
         }
         break;
      case MOVE_DOWN:
         Room[Idx] = 'X';

         // move down
         y++;

         // new index
         Idx = (y * Width) + x;

         // test square
         if (y >= Height)
         {
            State = FINISHED;
            return result;
         }
         else if (Room[Idx] == '#')
         {
            y--;
            Idx = (y * Width) + x;
            State = MOVE_LEFT;
            return result;
         }
         break;
      case MOVE_LEFT:
         Room[Idx] = 'X';

         // move left
         x--;

         // new index
         Idx = (y * Width) + x;

         // test square
         if (x < 0)
         {
            State = FINISHED;
            return result;
         }
         else if (Room[Idx] == '#')
         {
            x++;
            Idx = (y * Width) + x;
            State = MOVE_UP;
            return result;
         }
         break;
      default:
         break;
   }

   return result;
}

int main()
{
   std::ifstream file("../input.txt");
   std::vector<char> room;
   int width = 0;
   int height = 0;
   int start_idx = 0;
   int unique_squares = 0;
   TMoveState state = FINISHED;

   if (file.is_open())
   {
      std::string line;
      int idx = 0;

      while (1)
      {
         std::getline(file, line);
         if (file.eof())
            break;

         if (width == 0)
         {
            width = line.length();
         }
         else if (line.length() != width)
         {
            printf("Error: mismatched line widths\n");
            return 1;
         }

         for (auto& c : line)
         {
            if (c == '^' && state == FINISHED)
            {
               start_idx = idx;
               state = MOVE_UP;
            }

            room.push_back(c);
            idx++;
         }

         height++;
      }

      printf("%d x %d\n", width, height);
      printf("start_idx = %d\n", start_idx);
      assert(room.size() == (width*height));
      //print_room(room, width, height);

      while (state != FINISHED)
      {
         if (move(room, width, height, start_idx, state))
            unique_squares++;
         //print_room(room, width, height);
      }

      printf("Result: %d\n", unique_squares);
   }
}
