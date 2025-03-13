#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

//Button A: X+94, Y+34
//Button B: X+22, Y+67
//Prize: X=8400, Y=5400

// This is an algebra problem, 2 equations can be created from the input above:
// eq. 1: 94x + 22y = 8400
// eq. 2: 34x + 67y = 5400
//
// Solving for x and y gives: 80 for x and 40 for y

struct TEquation
{
   int Coefficient1;
   int Coefficient2;
   int Value;
};

struct TEquations
{
   TEquation Equation[2];
   int       ButtonA;
   int       ButtonB;
};

void print_equations(const TEquations& Equations)
{
   printf("eq. 1: %dx + %dy = %d\n",
          Equations.Equation[0].Coefficient1,
          Equations.Equation[0].Coefficient2,
          Equations.Equation[0].Value);
   printf("eq. 2: %dx + %dy = %d\n\n",
          Equations.Equation[1].Coefficient1,
          Equations.Equation[1].Coefficient2,
          Equations.Equation[1].Value);
}

void solve_equation(TEquations& Equations)
{
   // Solve for x by plugging in y from equation 2
   int numerator = (Equations.Equation[0].Value * Equations.Equation[1].Coefficient2) - (Equations.Equation[1].Value * Equations.Equation[0].Coefficient2);
   int denominator = (Equations.Equation[0].Coefficient1 * Equations.Equation[1].Coefficient2) - (Equations.Equation[1].Coefficient1 * Equations.Equation[0].Coefficient2);

   if (numerator % denominator == 0)
   {
      int x = numerator / denominator;
      int y = (Equations.Equation[0].Value - (Equations.Equation[0].Coefficient1 * x)) / Equations.Equation[0].Coefficient2;

      Equations.ButtonA = x;
      Equations.ButtonB = y;
   }
}

int main()
{
   //std::ifstream file("../test.txt"); // 480
   std::ifstream file("../input.txt");
   std::vector<TEquations> machines;
   uint64_t cost = 0;

   if (file.is_open())
   {
      TEquations eq = {};

      while (!file.eof())
      {
         std::string line;

         std::getline(file, line);

         if (strstr(line.c_str(), "Button A"))
         {
            eq = TEquations();

            size_t coef1_start = line.find("X");
            size_t coef1_end = line.find(",");
            size_t coef2_start = line.find("Y");
            size_t coef2_end = line.length();

            std::string coef1 = line.substr(coef1_start+1, coef1_end-coef1_start);
            std::string coef2 = line.substr(coef2_start+1, coef2_end-coef2_start);

            eq.Equation[0].Coefficient1 = std::strtol(coef1.c_str(), nullptr, 10);
            eq.Equation[1].Coefficient1 = std::strtol(coef2.c_str(), nullptr, 10);
         }
         else if (strstr(line.c_str(), "Button B"))
         {
            size_t coef1_start = line.find("X");
            size_t coef1_end = line.find(",");
            size_t coef2_start = line.find("Y");
            size_t coef2_end = line.length();

            std::string coef1 = line.substr(coef1_start+1, coef1_end-coef1_start);
            std::string coef2 = line.substr(coef2_start+1, coef2_end-coef2_start);

            eq.Equation[0].Coefficient2 = std::strtol(coef1.c_str(), nullptr, 10);
            eq.Equation[1].Coefficient2 = std::strtol(coef2.c_str(), nullptr, 10);
         }
         else if (strstr(line.c_str(), "Prize"))
         {
            size_t val1_start = line.find("X=");
            size_t val1_end = line.find(",");
            size_t val2_start = line.find("Y=");
            size_t val2_end = line.length();

            std::string val1 = line.substr(val1_start+2, val1_end-val1_start);
            std::string val2 = line.substr(val2_start+2, val2_end-val2_start);

            eq.Equation[0].Value = std::strtol(val1.c_str(), nullptr, 10);
            eq.Equation[1].Value = std::strtol(val2.c_str(), nullptr, 10);

            //print_equations(eq);

            machines.push_back(eq);
         }

         if (file.eof())
            break;
      }
   }

   printf("Number of machines: %ld\n", machines.size());

   for (auto& machine : machines)
   {
      solve_equation(machine);
      cost += machine.ButtonA * 3 + machine.ButtonB * 1;
   }

   printf("Cost: %ld\n", cost);
}
