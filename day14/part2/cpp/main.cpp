#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

struct TVector
{
   int X;
   int Y;
};

struct TRobot
{
   TVector Position;
   TVector Velocity;
};

void print_map(unsigned char* Map, std::unordered_map<int, int>& RobotCounts, int Width, int Height)
{
   printf("\n");
   for (int y = 0; y < Height; y++)
   {
      for (int x = 0; x < Width; x++)
      {
         int idx = (y * Width) + x;

         if (RobotCounts.find(idx) != RobotCounts.end())
         {
            printf("%d", RobotCounts[idx]);
         }
         else
         {
            printf(".");
         }
      }
      printf("\n");
   }
   printf("\n");
}

int main()
{
   //std::ifstream file("../test.txt"); // 12
   std::ifstream file("../input.txt"); // 225552000

   if (file.is_open())
   {
      std::vector<TRobot> robots;
      //const int width = 11;
      //const int height = 7;
      const int width = 101;
      const int height = 103;
      unsigned char map[width * height] = {};
      std::unordered_map<int, int> robot_counts;

      while (!file.eof())
      {
         std::string line;
         TRobot robot;

         std::getline(file, line);

         if (file.eof())
            break;

         int value_count = 0;
         for (size_t i = 0; i < line.length(); i++)
         {
            if (value_count == 0)
            {
               if (line[i] == '=')
               {
                  robot.Position.X = std::atoi(&line.c_str()[i+1]);
                  value_count++;
               }
            }
            else if (value_count == 1)
            {
               if (line[i] == ',')
               {
                  robot.Position.Y = std::atoi(&line.c_str()[i+1]);
                  value_count++;
               }
            }
            else if (value_count == 2)
            {
               if (line[i] == '=')
               {
                  robot.Velocity.X = std::atoi(&line.c_str()[i+1]);
                  value_count++;
               }
            }
            else if (value_count == 3)
            {
               if (line[i] == ',')
               {
                  robot.Velocity.Y = std::atoi(&line.c_str()[i+1]);
                  value_count++;
               }
            }
         }

         robots.push_back(robot);
      }

      printf("Number of robots: %ld\n", robots.size());

      // generate initial map of robot counts
      robot_counts.clear();
      for (const auto& robot : robots)
      {
         int idx = (robot.Position.Y * width) + robot.Position.X;

         if (robot_counts.find(idx) == robot_counts.end())
         {
            robot_counts[idx] = 1;
         }
         else
         {
            robot_counts[idx]++;
         }
      }

      print_map(map, robot_counts, width, height);

      // 2118 vertical pattern every 101 seconds or so

      // simulate robot movements for 100 seconds
      int seconds = 0;
      while (1)
      {
         for (auto& robot : robots)
         {
            robot.Position.X += robot.Velocity.X;
            robot.Position.Y += robot.Velocity.Y;

            if (robot.Position.X < 0)
               robot.Position.X += width;

            if (robot.Position.X >= width)
               robot.Position.X -= width;

            if (robot.Position.Y < 0)
               robot.Position.Y += height;

            if (robot.Position.Y >= height)
               robot.Position.Y -= height;
         }

         seconds++;

         int check = seconds - 7371;

         if (check < 0)
            continue;

         if (check % 101 == 0)
         {
            // generate map of robot counts after simulation
            printf("Seconds: %d\n", seconds);
            robot_counts.clear();
            for (const auto& robot : robots)
            {
               int idx = (robot.Position.Y * width) + robot.Position.X;

               if (robot_counts.find(idx) == robot_counts.end())
               {
                  robot_counts[idx] = 1;
               }
               else
               {
                  robot_counts[idx]++;
               }
            }
            print_map(map, robot_counts, width, height);

            std::cin.get();
         }
      }
   }
}
