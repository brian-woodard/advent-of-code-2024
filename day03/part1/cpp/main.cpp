#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

enum TState
{
   STATE_READ_MUL,
   STATE_READ_OP1,
   STATE_READ_OP2,
};

int main()
{
   std::ifstream file("../input.txt");
   std::ofstream out_file("test1.txt");
   uint64_t value = 0;
   uint64_t num_muls = 0;
   int op1 = -1;
   int op2 = -1;
   TState state = STATE_READ_MUL;

   if (file.is_open())
   {
      std::stringstream buffer;
      std::string input;

      buffer << file.rdbuf();
      input = buffer.str();

      size_t idx = 0;
      while (idx < input.length())
      {
         switch (state)
         {
            case STATE_READ_MUL:
               if (input[idx] == 'm' &&
                   input[idx+1] == 'u' &&
                   input[idx+2] == 'l' &&
                   input[idx+3] == '(')
               {
                  idx += 4;

                  op1 = -1;
                  op2 = -1;
                  state = STATE_READ_OP1;
               }
               else
               {
                  idx++;
               }
               break;
            case STATE_READ_OP1:
               if (isdigit(input[idx]))
               {
                  if (op1 == -1)
                  {
                     op1 = (int)(input[idx] - '0');
                  }
                  else
                  {
                     op1 *= 10;
                     op1 += (int)(input[idx] - '0');
                  }
                  idx++;
               }
               else
               {
                  if (input[idx] == ',' && op1 != -1)
                     state = STATE_READ_OP2;
                  else
                     state = STATE_READ_MUL;
                  idx++;
               }
               break;
            case STATE_READ_OP2:
               if (isdigit(input[idx]))
               {
                  if (op2 == -1)
                  {
                     op2 = (int)(input[idx] - '0');
                  }
                  else
                  {
                     op2 *= 10;
                     op2 += (int)(input[idx] - '0');
                  }
                  idx++;
               }
               else
               {
                  if (input[idx] == ')' && op2 != -1)
                  {
                     value += (op1 * op2);

                     out_file << op1 << " " << op2 << std::endl;

                     num_muls++;
                  }
                  state = STATE_READ_MUL;
                  idx++;
               }
               break;
         }
      }

      printf("Value: %llu\n", value);
      printf("Num muls: %d\n", num_muls);
   }
}
