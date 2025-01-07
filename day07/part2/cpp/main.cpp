#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>

////////////////////////////////////////////////////////////////////////
// 2 operations
// 3 operands
// 2^2 = 4
//3267: 81+40+27
//3267: 81+40*27
//3267: 81*40+27
//3267: 81*40*27

// concate
// 3 operations
// 3^2 = 9
//3267: 81|40|27
//3267: 81+40|27
//3267: 81*40|27
//
//3267: 81|40+27
//3267: 81+40+27 *
//3267: 81*40+27 *
//
//3267: 81|40*27
//3267: 81+40*27 *
//3267: 81*40*27 *

////////////////////////////////////////////////////////////////////////
//// 2 operations
//// 4 operands
//// 2^3 = 8
//7290: 6+8+6+15
//7290: 6+8+6*15
//7290: 6+8*6+15
//7290: 6+8*6*15
//7290: 6*8+6+15
//7290: 6*8+6*15
//7290: 6*8*6+15
//7290: 6*8*6*15
//
//// concate
//// 3 operations
//// 3^3 = 27
//7290: 6|8|6|15
//7290: 6+8|6|15
//7290: 6*8|6|15
//7290: 6|8+6|15
//7290: 6+8+6|15
//7290: 6*8+6|15
//7290: 6|8*6|15
//7290: 6+8*6|15
//7290: 6*8*6|15
//
//7290: 6|8|6+15
//7290: 6+8|6+15
//7290: 6*8|6+15
//7290: 6|8+6+15
//7290: 6+8+6+15 *
//7290: 6*8+6+15 *
//7290: 6|8*6+15
//7290: 6+8*6+15 *
//7290: 6*8*6+15 *
//
//7290: 6|8|6*15
//7290: 6+8|6*15
//7290: 6*8|6*15
//7290: 6|8+6*15
//7290: 6+8+6*15 *
//7290: 6*8+6*15 *
//7290: 6|8*6*15
//7290: 6+8*6*15 *
//7290: 6*8*6*15 *

enum eOp
{
   CONCATE,
   ADDITION,
   MULTIPLY,
   NUM_OPS,
};

struct TOperand
{
   int Value;
   eOp Op;
};

struct TOperation
{
   uint64_t Result;
   std::vector<TOperand> Operands;
};

struct TInput
{
   std::vector<TOperation> Operations;
};

uint64_t check_operation(TOperation& Operation)
{
   size_t operand_count = Operation.Operands.size() - 1;
   size_t loop_count = 1;
   uint64_t result = 0;

   assert(operand_count > 0 && operand_count < 32);

   for (size_t i = 0; i < operand_count; i++)
      loop_count *= 3;

   for (size_t i = 0; i < loop_count; i++)
   {
      size_t divisor = 1;
      for (size_t j = 0; j < operand_count; j++)
      {
         Operation.Operands[j].Op = eOp((i / divisor) % NUM_OPS);
         divisor *= 3;
      }

      uint64_t value = Operation.Operands[0].Value;
      for (size_t j = 0; j < Operation.Operands.size() - 1; j++)
      {
         char operation;

         switch (Operation.Operands[j].Op)
         {
            case CONCATE:
            {
               int tmp = Operation.Operands[j+1].Value;
               while (tmp > 0)
               {
                  tmp /= 10;
                  value *= 10;
               }
               value += Operation.Operands[j+1].Value;
               operation = '|';
               break;
            }
            case ADDITION:
               value += Operation.Operands[j+1].Value;
               operation = '+';
               break;
            case MULTIPLY:
               value *= Operation.Operands[j+1].Value;
               operation = '*';
               break;
            default:
               operation = '|';
         }
      }

      if (value == Operation.Result)
      {
         result = value;
         break;
      }
   }

   return result;
}

int main()
{
   TInput input;
   uint64_t result = 0;
   uint64_t max_operations = 0;

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
               operation.Operands.push_back({std::atoi(operands.c_str()), CONCATE});
            skip_first = false;
         }

         //printf("%llu: ", operation.Result);
         //for (auto& op : operation.Operands)
         //{
         //   printf("%d ", op);
         //}
         //printf("\n");

         if (operation.Operands.size() > max_operations)
         {
            max_operations = operation.Operands.size();
         }

         input.Operations.push_back(operation);
      }
   }

   for (size_t i = 0; i < input.Operations.size(); i++)
   {
      result += check_operation(input.Operations[i]);
   }

   printf("Result: %llu max ops %d\n", result, max_operations);
}
