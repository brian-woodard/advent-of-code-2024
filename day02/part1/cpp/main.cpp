#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>

int main()
{
   std::ifstream file ("../input.txt");
   std::vector<std::vector<int>> input;
   uint64_t num_safe = 0;

   if (file.is_open())
   {
      while (1)
      {
         std::string line;

         std::getline(file, line);
         if (file.eof())
            break;

         std::stringstream str_stream(line);

         int num;
         std::vector<int> report;
         while (str_stream >> num)
         {
            report.push_back(num);
         }
         input.push_back(report);
      }
   }

   for (auto& report : input)
   {
      bool decreasing = report[1] < report[0];
      bool safe = true;

      for (size_t i = 0; i < (report.size() - 1); i++)
      {
         int delta = report[i+1] - report[i];

         if (delta == 0)
         {
            safe = false;
            break;
         }

         if (decreasing)
         {
            if (delta > -1 || delta < -3)
            {
               safe = false;
               break;
            }
         }

         if (!decreasing)
         {
            if (delta < 1 || delta > 3)
            {
               safe = false;
               break;
            }
         }
      }

      if (safe)
         num_safe++;
   }

   printf("Number of safe reports: %d\n", num_safe);
}
