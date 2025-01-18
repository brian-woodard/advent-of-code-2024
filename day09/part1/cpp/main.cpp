#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct TDiskNode
{
   int FileId;
};

void print_disk(const std::vector<TDiskNode>& Disk)
{
   for (const auto& node : Disk)
   {
      if (node.FileId == -1)
         std::cout << (char)'.';
      else
         std::cout << node.FileId;
   }
   std::cout << std::endl;
}

int main()
{
   std::ifstream file("../input.txt");
   std::vector<TDiskNode> input;

   if (file.is_open())
   {
      std::stringstream buffer;

      buffer << file.rdbuf();

      input.reserve(buffer.str().length());

      bool is_file = true;
      int file_id = 0;
      for (size_t i = 0; i < buffer.str().length(); i++)
      {
         int count = buffer.str().c_str()[i] - '0';

         for (int j = 0; j < count; j++)
         {
            if (is_file)
               input.push_back({file_id});
            else
               input.push_back({-1});
         }

         if (is_file)
            file_id++;

         is_file = !is_file;
      }

      //print_disk(input);

      // Defragment disk
      size_t empty_idx = 0;
      size_t node_idx = input.size() - 1;

      int count = 0;
      uint64_t checksum = 0;
      while (1)
      {
         count++;

         // Find first empty index
         for (size_t i = empty_idx; i < input.size(); i++)
         {
            if (input[i].FileId == -1)
            {
               empty_idx = i;
               break;
            }
            else
               checksum += i * input[i].FileId;
         }

         // Find right most file node
         for (size_t i = node_idx; i >= 0; i--)
         {
            if (input[i].FileId != -1)
            {
               node_idx = i;
               break;
            }
         }

         if (empty_idx > node_idx)
            break;

         input[empty_idx].FileId = input[node_idx].FileId;
         input[node_idx].FileId = -1;
      }

      //print_disk(input);
      printf("buffer length %d\n", buffer.str().length());
      printf("Max file id: %d\n", file_id);
      printf("iterations: %d\n", count);
      printf("checksum: %ld\n", checksum);
   }
}
