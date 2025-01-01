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
   int op1 = -1;
   int op2 = -1;
   bool enabled = true;
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
               if (enabled && input[idx] == 'm' &&
                   input[idx+1] == 'u' &&
                   input[idx+2] == 'l' &&
                   input[idx+3] == '(')
               {
                  idx += 4;

                  op1 = -1;
                  op2 = -1;
                  state = STATE_READ_OP1;
               }
               else if (input[idx] == 'd' &&
                        input[idx+1] == 'o' &&
                        input[idx+2] == '(' &&
                        input[idx+3] == ')')
               {
                  idx += 4;
                  enabled = true;
               }
               else if (input[idx] == 'd' &&
                        input[idx+1] == 'o' &&
                        input[idx+2] == 'n' &&
                        input[idx+3] == '\'' &&
                        input[idx+4] == 't' &&
                        input[idx+5] == '(' &&
                        input[idx+6] == ')')
               {
                  idx += 7;
                  enabled = false;
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
                     if (enabled)
                        value += (op1 * op2);

                     out_file << op1 << " " << op2 << std::endl;
                  }
                  state = STATE_READ_MUL;
                  idx++;
               }
               break;
         }
      }

      printf("Value: %llu\n", value);
   }
}
