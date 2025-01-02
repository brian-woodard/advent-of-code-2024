#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

int search(const std::vector<char>& Input, int X, int Y, int Width, int Height)
{
   if (X < 1 || X >= Width-1 ||
       Y < 1 || Y >= Height-1)
   {
      return 0;
   }

   int s_idx = (Y * Width) + X;
   int result = 0;

   if (Input[s_idx] != 'A')
      return 0;

   int idx1 = s_idx - Width - 1;
   int idx2 = s_idx - Width + 1;
   int idx3 = s_idx + Width + 1;
   int idx4 = s_idx + Width - 1;

   int count_m = 0;
   int count_s = 0;

   if (Input[idx1] == 'M' && Input[idx3] == 'M' &&
       Input[idx2] == 'S' && Input[idx4] == 'S')
      return 0;

   if (Input[idx1] == 'S' && Input[idx3] == 'S' &&
       Input[idx2] == 'M' && Input[idx4] == 'M')
      return 0;

   if (Input[idx1] == 'M')
      count_m++;

   if (Input[idx2] == 'M')
      count_m++;

   if (Input[idx3] == 'M')
      count_m++;

   if (Input[idx4] == 'M')
      count_m++;

   if (Input[idx1] == 'S')
      count_s++;

   if (Input[idx2] == 'S')
      count_s++;

   if (Input[idx3] == 'S')
      count_s++;

   if (Input[idx4] == 'S')
      count_s++;

   return (count_m == 2 && count_s == 2);
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
            words_found += search(input, x, y, width, height);
         }
      }

      printf("Number of words found: %d\n", words_found);
   }
}
