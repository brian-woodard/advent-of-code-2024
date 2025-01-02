#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

int search(const std::vector<char>& Input, int X, int Y, int Width, int Height)
{
   int s_idx = (Y * Width) + X;
   int result = 0;

   if (Input[s_idx] != 'X')
      return 0;

   // check left
   if (X >= 3)
   {
      if (Input[s_idx-1] == 'M' &&
          Input[s_idx-2] == 'A' &&
          Input[s_idx-3] == 'S')
      {
         result++;
      }
   }

   // check right
   if (X <= Width-4)
   {
      if (Input[s_idx+1] == 'M' &&
          Input[s_idx+2] == 'A' &&
          Input[s_idx+3] == 'S')
      {
         result++;
      }
   }

   // check up
   if (Y >= 3)
   {
      int idx1 = s_idx - Width;
      int idx2 = idx1 - Width;
      int idx3 = idx2 - Width;
      if (Input[idx1] == 'M' &&
          Input[idx2] == 'A' &&
          Input[idx3] == 'S')
      {
         result++;
      }
   }

   // check down
   if (Y <= Height-4)
   {
      int idx1 = s_idx + Width;
      int idx2 = idx1 + Width;
      int idx3 = idx2 + Width;
      if (Input[idx1] == 'M' &&
          Input[idx2] == 'A' &&
          Input[idx3] == 'S')
      {
         result++;
      }
   }

   // check upper left
   if (X >= 3 && Y >= 3)
   {
      int idx1 = s_idx - Width - 1;
      int idx2 = idx1 - Width - 1;
      int idx3 = idx2 - Width - 1;
      if (Input[idx1] == 'M' &&
          Input[idx2] == 'A' &&
          Input[idx3] == 'S')
      {
         result++;
      }
   }

   // check upper right
   if (X <= Width-4 && Y >= 3)
   {
      int idx1 = s_idx - Width + 1;
      int idx2 = idx1 - Width + 1;
      int idx3 = idx2 - Width + 1;
      if (Input[idx1] == 'M' &&
          Input[idx2] == 'A' &&
          Input[idx3] == 'S')
      {
         result++;
      }
   }

   // check lower left
   if (X >= 3 && Y <= Height-4)
   {
      int idx1 = s_idx + Width - 1;
      int idx2 = idx1 + Width - 1;
      int idx3 = idx2 + Width - 1;
      if (Input[idx1] == 'M' &&
          Input[idx2] == 'A' &&
          Input[idx3] == 'S')
      {
         result++;
      }
   }

   // check lower right
   if (X <= Width-4 && Y <= Height-4)
   {
      int idx1 = s_idx + Width + 1;
      int idx2 = idx1 + Width + 1;
      int idx3 = idx2 + Width + 1;
      if (Input[idx1] == 'M' &&
          Input[idx2] == 'A' &&
          Input[idx3] == 'S')
      {
         result++;
      }
   }

   return result;
}

int main()
{
   std::ifstream file("../input.txt");
   std::vector<char> input;
   int width = 0;
   int height = 0;
   uint64_t words_found = 0;

   if (file.is_open())
   {
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
         else if (width != line.length())
         {
            printf("Error: mismatched line lengths!\n");
            exit(1);
         }

         for (size_t i = 0; i < line.length(); i++)
         {
            input.push_back(line[i]);
         }
         height++;
      }

      printf("%d x %d\n", width, height);

      for (int y = 0; y < height; y++)
      {
         for (int x = 0; x < width; x++)
         {
            int idx = (y * width) + x;
            words_found += search(input, x, y, width, height);
         }
      }

      printf("Number of words found: %d\n", words_found);
   }
}
