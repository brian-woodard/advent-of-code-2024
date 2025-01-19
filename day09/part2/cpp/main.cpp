#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct TDiskNode
{
   int FileId;
   int Size;
};

void print_disk(const std::vector<TDiskNode>& Disk)
{
   for (const auto& node : Disk)
   {
      if (node.FileId == -1)
      {
         for (int i = 0; i < node.Size; i++)
         {
            std::cout << '.';
         }
      }
      else
      {
         for (int i = 0; i < node.Size; i++)
         {
            std::cout << std::to_string(node.FileId);
         }
      }
   }
   printf("\nSize: %ld\n", Disk.size());
}

int main()
{
   std::ifstream file("../input.txt");

   if (file.is_open())
   {
      std::stringstream buffer;
      std::vector<TDiskNode> input;

      buffer << file.rdbuf();

      input.reserve(buffer.str().length());

      int file_id = 0;
      int max_file_id = 0;
      bool is_file = true;
      for (size_t i = 0; i < buffer.str().length(); i++)
      {
         int size = buffer.str().c_str()[i] - '0';

         if (size <= 0 || size > 9)
         {
            is_file = !is_file;
            continue;
         }

         if (is_file)
         {
            max_file_id = file_id;
            input.push_back({file_id++, size});
         }
         else
         {
            input.push_back({-1, size});
         }

         is_file = !is_file;
      }

      //print_disk(input);

      int count = 0;
      file_id = max_file_id;
      while (1)
      {
         count++;

         int empty_idx = 0;
         int node_idx = input.size() - 1;

         // Find first empty node
         for (int i = 0; i < input.size(); i++)
         {
            if (input[i].FileId == -1)
            {
               empty_idx = i;
               break;
            }
         }

         // Find start index for node
         for (int i = node_idx; i >= 0; i--)
         {
            if (input[i].FileId == file_id)
            {
               node_idx = i;
               break;
            }
         }

         if (node_idx < empty_idx || file_id < 0)
            break;

         // Now search from left for an empty block that can fit the node
         for (int i = empty_idx; i < node_idx; i++)
         {
            if (input[i].FileId == -1 && input[i].Size >= input[node_idx].Size)
            {
               int split_size = input[i].Size - input[node_idx].Size;

               // Swap nodes
               TDiskNode tmp = input[i];
               input[i] = input[node_idx];
               input[node_idx] = tmp;

               // if empty space was larger, add an empty spot after, and fix size
               if (split_size != 0)
               {
                  input[node_idx].Size -= split_size;
                  input.insert(input.begin() + i + 1, {-1, split_size});
               }

               //print_disk(input);

               break;
            }
         }

         file_id--;
      }

      //print_disk(input);

      uint64_t checksum = 0;
      uint64_t idx = 0;
      for (size_t i = 0; i < input.size(); i++)
      {
         if (input[i].FileId == -1)
         {
            idx += input[i].Size;
         }
         else
         {
            for (int j = 0; j < input[i].Size; j++)
            {
               checksum += (input[i].FileId * idx++);
            }
         }
      }

      printf("buffer length %ld\n", buffer.str().length());
      printf("max file id: %d\n", max_file_id);
      printf("checksum: %ld\n", checksum);
   }
}
