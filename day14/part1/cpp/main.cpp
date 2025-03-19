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

      // simulate robot movements for 100 seconds
      for (int i = 0; i < 100; i++)
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
      }

      // generate map of robot counts after simulation
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

      // quad 1 count
      int count_1 = 0;
      for (int y = 0; y < height / 2; y++)
      {
         for (int x = 0; x < width / 2; x++)
         {
            int idx = (y * width) + x;

            if (robot_counts.find(idx) != robot_counts.end())
               count_1 += robot_counts[idx];
         }
      }
      printf("Quad 1: %d\n", count_1);

      // quad 2 count
      int count_2 = 0;
      for (int y = 0; y < height / 2; y++)
      {
         for (int x = (width / 2) + 1; x < width; x++)
         {
            int idx = (y * width) + x;

            if (robot_counts.find(idx) != robot_counts.end())
               count_2 += robot_counts[idx];
         }
      }
      printf("Quad 2: %d\n", count_2);

      // quad 3 count
      int count_3 = 0;
      for (int y = (height / 2) + 1; y < height; y++)
      {
         for (int x = 0; x < width / 2; x++)
         {
            int idx = (y * width) + x;

            if (robot_counts.find(idx) != robot_counts.end())
               count_3 += robot_counts[idx];
         }
      }
      printf("Quad 3: %d\n", count_3);

      // quad 4 count
      int count_4 = 0;
      for (int y = (height / 2) + 1; y < height; y++)
      {
         for (int x = (width / 2) + 1; x < width; x++)
         {
            int idx = (y * width) + x;

            if (robot_counts.find(idx) != robot_counts.end())
               count_4 += robot_counts[idx];
         }
      }
      printf("Quad 4: %d\n", count_4);

      printf("Safety score: %d\n", (count_1 * count_2 * count_3 * count_4));
   }
}
