#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>

struct TOperation
{
   uint64_t Result;
   std::vector<int> Operands;
};

struct TInput
{
   std::vector<TOperation> Operations;
};

uint64_t check_operation(const TOperation& Operation)
{
   uint64_t op_mask = 0;
   uint64_t mask_size = Operation.Operands.size() - 1;
   uint64_t max_num_ops = 1 << mask_size;
   uint64_t result = 0;

   assert(mask_size < 32);

   for (uint64_t i = 0; i < max_num_ops; i++)
   {
      uint64_t value = Operation.Operands[0];

      for (uint64_t ops = 0; ops < mask_size; ops++)
      {
         if ((op_mask & (1 << ops)) == 0)
         {
            value += Operation.Operands[ops+1];
         }
         else
         {
            value *= Operation.Operands[ops+1];
         }
      }

      if (value == Operation.Result)
      {
         result = value;
         break;
      }

      op_mask++;
   }

   return result;
}

int main()
{
   TInput input;
   uint64_t result = 0;

   std::ifstream file("../input.txt");

   if (file.is_open())
   {
      while (1)
      {
         std::string line;

         std::getline(file, line);
         if (file.eof())
            break;

         std::stringstream str_stream(line);
         std::string result;
         std::string operands;

         std::getline(str_stream, result, ':');
         std::getline(str_stream, operands, ':');

         TOperation operation;

         operation.Result = std::strtoull(result.c_str(), nullptr, 10);

         std::stringstream ops_stream(operands);
         bool skip_first = true;
         while (std::getline(ops_stream, operands, ' '))
         {
            if (!skip_first)
               operation.Operands.push_back(std::atoi(operands.c_str()));
            skip_first = false;
         }

         //printf("%llu: ", operation.Result);
         //for (auto& op : operation.Operands)
         //{
         //   printf("%d ", op);
         //}
         //printf("\n");

         input.Operations.push_back(operation);
      }
   }

   for (size_t i = 0; i < input.Operations.size(); i++)
   {
      result += check_operation(input.Operations[i]);
   }

   printf("Result: %llu\n", result);
}
