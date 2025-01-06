#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>

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
   bool result = true;

   switch (State)
   {
      case MOVE_UP:
         // move up
         y--;

         // new index
         Idx = (y * Width) + x;

         // test square
         if (y < 0)
         {
            State = FINISHED;
         }
         else if (Room[Idx] == '#')
         {
            y++;
            Idx = (y * Width) + x;
            State = MOVE_RIGHT;
            result = false;
         }
         break;
      case MOVE_RIGHT:
         // move right
         x++;

         // new index
         Idx = (y * Width) + x;

         // test square
         if (x >= Width)
         {
            State = FINISHED;
         }
         else if (Room[Idx] == '#')
         {
            x--;
            Idx = (y * Width) + x;
            State = MOVE_DOWN;
            result = false;
         }
         break;
      case MOVE_DOWN:
         // move down
         y++;

         // new index
         Idx = (y * Width) + x;

         // test square
         if (y >= Height)
         {
            State = FINISHED;
         }
         else if (Room[Idx] == '#')
         {
            y--;
            Idx = (y * Width) + x;
            State = MOVE_LEFT;
            return false;
         }
         break;
      case MOVE_LEFT:
         // move left
         x--;

         // new index
         Idx = (y * Width) + x;

         // test square
         if (x < 0)
         {
            State = FINISHED;
         }
         else if (Room[Idx] == '#')
         {
            x++;
            Idx = (y * Width) + x;
            State = MOVE_UP;
            return false;
         }
         break;
      default:
         break;
   }

   return result;
}

bool check_for_loop(std::vector<char>& Room, int Width, int Height, int CurrIdx, int StartIdx)
{
   int x = CurrIdx % Width;
   int y = CurrIdx / Width;

   //printf("\ncheck for loop at (%d, %d)\n", x, y);

   // add a barrier at current location
   char prev = Room[CurrIdx];
   Room[CurrIdx] = '#';

   // see if we're in a loop
   int moves = 0;
   TMoveState state = MOVE_UP;
   static int max_moves = 0;

   int start_idx = StartIdx;
   std::unordered_map<int, bool> visited;

   visited[start_idx] = true;

   while (state != FINISHED)
   {
      bool able_to_move = move(Room, Width, Height, StartIdx, state);
      moves++;

      //if (able_to_move && state != FINISHED)
      //{
      //   // check if we haven't visited this spot before
      //   if (visited.find(StartIdx) == visited.end())
      //   {
      //      int x = StartIdx % Width;
      //      int y = StartIdx / Width;
      //      //printf(">>> (%d, %d) visited false size %d move %d\n", x, y, visited.size(), moves);
      //      // update this as our new start index
      //      start_idx = StartIdx;
      //      visited[start_idx] = true;
      //   }
      //   else if (start_idx == StartIdx)
      //   {
      //      //printf(">>> loop at move %d\n", moves);
      //      break;
      //   }
      //}

      if (moves > (Width * Height * 10))
         break;
   }

   //printf("(%d, %d) moves %d\n", x, y, moves);

   if (state == FINISHED && moves > max_moves)
   {
      max_moves = moves;
      //printf("Max is now: %d\n", max_moves);
   }

   // remove barrier at current location
   Room[CurrIdx] = prev;

   return (state != FINISHED);
}

int main()
{
   std::ifstream file("../input.txt");
   std::vector<char> room;
   std::unordered_map<int, bool> loop_locations;
   int num_loop_locations = 0;
   int width = 0;
   int height = 0;
   int start_idx = 0;
   int curr_idx = 0;
   int moves = 0;
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
      print_room(room, width, height);

      bool first = true;
      curr_idx = start_idx;
      while (state != FINISHED)
      {
         if (move(room, width, height, curr_idx, state))
         {
            room[curr_idx] = 'o';
            moves++;
         }

         if (state != FINISHED)
         {
            if (loop_locations.find(curr_idx) == loop_locations.end())
            {
               int x = curr_idx % width;
               int y = curr_idx / width;
               //printf(">>> %d check for loop at (%d, %d)\n", moves, x, y);
               loop_locations[curr_idx] = check_for_loop(room, width, height, curr_idx, start_idx);
               //if (loop_locations[curr_idx])
               //   printf("found loop at (%d, %d) loop %d\n", x, y, loop_locations[curr_idx]);
            }
         }

         first = false;
      }

      room[start_idx] = '*';
      for (auto idx : loop_locations)
      {
         if (idx.second)
         {
            room[idx.first] = 'O';
            num_loop_locations++;
         }
      }
      print_room(room, width, height);

      printf("Result: %d %d\n", moves, num_loop_locations);
   }
}
