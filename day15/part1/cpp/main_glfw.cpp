#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <chrono>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GlDebug.h"
#include "FontAtlas.h"
#include "Shader.h"
#include "GlRect.h"

GLFWwindow*              window;
std::shared_ptr<CShader> rect_shader;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (window == nullptr || scancode == -1 || mods == -1)
      return;

   if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
   {
      //pause = !pause;
   }
}

void resize(GLFWwindow* window, int width, int height)
{
   if (window == nullptr)
      return;

   GLCALL(glViewport(0, 0, width, height));
}

void render(std::vector<char>& Map, int Width, int Height)
{
   glm::mat4 projection = glm::ortho(0.0f, 100.0f, 100.0f, 0.0f, -1.0f, 1.0f);
   glm::vec2 adv = glm::vec2((100.0f / (float)Width), (100.0f / (float)Height));

   // Clear the window with the background color
   GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

   // draw tiles
   CGlRect rect = CGlRect(rect_shader, adv.x * 0.5 + adv.x * 2.0f, adv.y * 0.5 + adv.y * 3.0f, adv.x, adv.y);
   rect.SetColor(glm::vec4(1.0f));

   glm::vec2 pos = glm::vec2(adv.x * 0.5f, adv.y * 0.5f);                       
                                                                                
   for (int y = 0; y < Height; y++)                                             
   {                                                                            
      for (int x = 0; x < Width; x++)                                           
      {                                                                         
         int idx = (y * Width) + x;
         glm::vec2 pos = glm::vec2(adv.x * 0.5 + (adv.x * x),
                                   adv.y * 0.5 + (adv.y * y));

         if (Map[idx] == '#')
         {
            rect.SetColor(glm::vec4(1.0f));
            rect.SetPosition(pos.x, pos.y);
            rect.Render(projection);
         }
         else if (Map[idx] == 'O')
         {
            rect.SetColor(glm::vec4(0.7f, 0.8f, 0.1f, 1.0f));
            rect.SetPosition(pos.x, pos.y);
            rect.Render(projection);
         }
         else if (Map[idx] == '@')
         {
            rect.SetColor(glm::vec4(0.8f, 0.2f, 0.1f, 1.0f));
            rect.SetPosition(pos.x, pos.y);
            rect.Render(projection);
         }
                                                                                
         pos.x += adv.x;                                                        
      }                                                                         
                                                                                
      pos.y += adv.y;                                                           
      pos.x = adv.x * 0.5f;                                                     
   }                                                                            
                                                                                
   glfwSwapBuffers(window);                                                     
   glfwPollEvents();                                                            
}

void print_map(std::vector<char>& Map, int Width)
{
   printf("\n");
   for (size_t i = 0; i < Map.size(); i++)
   {
      if (i > 0 && i % Width == 0)
         printf("\n");
      printf("%c", Map[i]);
   }
   printf("\n");
}

bool move(std::vector<char>& Map, char Move, int& Position, int Width, int Height)
{
   int x = Position % Width;
   int y = Position / Width;

   switch (Move)
   {
      case '<':
      {
         if (x > 1)
         {
            int idx = (y * Width) + x - 1;
            if (Map[idx] == '.')
            {
               bool update_position = false;
               if (Map[Position] == '@')
                  update_position = true; 
               Map[idx] = Map[Position];
               Map[Position] = '.';
               if (update_position)
                  Position = idx;
               return true;
            }
            else if (Map[idx] == 'O')
            {
               if (move(Map, Move, idx, Width, Height))
               {
                  bool update_position = false;
                  if (Map[Position] == '@')
                     update_position = true; 
                  Map[idx] = Map[Position];
                  Map[Position] = '.';
                  if (update_position)
                     Position = idx;
                  return true;
               }
               else
               {
                  return false;
               }
            }
         }
         else
         {
            return false;
         }
         break;
      }
      case '>':
      {
         if (x < Width - 1)
         {
            int idx = (y * Width) + x + 1;
            if (Map[idx] == '.')
            {
               bool update_position = false;
               if (Map[Position] == '@')
                  update_position = true; 
               Map[idx] = Map[Position];
               Map[Position] = '.';
               if (update_position)
                  Position = idx;
               return true;
            }
            else if (Map[idx] == 'O')
            {
               if (move(Map, Move, idx, Width, Height))
               {
                  bool update_position = false;
                  if (Map[Position] == '@')
                     update_position = true; 
                  Map[idx] = Map[Position];
                  Map[Position] = '.';
                  if (update_position)
                     Position = idx;
                  return true;
               }
               else
               {
                  return false;
               }
            }
         }
         else
         {
            return false;
         }
         break;
      }
      case '^':
      {
         if (y > 1)
         {
            int idx = ((y - 1) * Width) + x;
            if (Map[idx] == '.')
            {
               bool update_position = false;
               if (Map[Position] == '@')
                  update_position = true; 
               Map[idx] = Map[Position];
               Map[Position] = '.';
               if (update_position)
                  Position = idx;
               return true;
            }
            else if (Map[idx] == 'O')
            {
               if (move(Map, Move, idx, Width, Height))
               {
                  bool update_position = false;
                  if (Map[Position] == '@')
                     update_position = true; 
                  Map[idx] = Map[Position];
                  Map[Position] = '.';
                  if (update_position)
                     Position = idx;
                  return true;
               }
               else
               {
                  return false;
               }
            }
         }
         else
         {
            return false;
         }
         break;
      }
      case 'v':
      {
         if (y < Height - 1)
         {
            int idx = ((y + 1) * Width) + x;
            if (Map[idx] == '.')
            {
               bool update_position = false;
               if (Map[Position] == '@')
                  update_position = true; 
               Map[idx] = Map[Position];
               Map[Position] = '.';
               if (update_position)
                  Position = idx;
               return true;
            }
            else if (Map[idx] == 'O')
            {
               if (move(Map, Move, idx, Width, Height))
               {
                  bool update_position = false;
                  if (Map[Position] == '@')
                     update_position = true; 
                  Map[idx] = Map[Position];
                  Map[Position] = '.';
                  if (update_position)
                     Position = idx;
                  return true;
               }
               else
               {
                  return false;
               }
            }
         }
         else
         {
            return false;
         }
         break;
      }
      default:
         break;
   }

   return false;
}

int main()
{
   //std::ifstream file("../test1.txt"); // 2028
   //std::ifstream file("../test2.txt"); // 10092
   std::ifstream file("../input.txt"); // 1294459

   if (file.is_open())
   {
      std::string line;
      std::vector<char> map;
      std::vector<char> moves;
      int width = 0;
      int height = 0;
      int start = 0;
      bool read_map = true;
      uint64_t gps_sum = 0;

      while (!file.eof())
      {
         if (file.eof())
            break;

         std::getline(file, line);

         if (read_map)
         {
            if (line.length() > 0)
            {
               if (width == 0)
               {
                  width = line.length();
               }
               else
               {
                  assert(width == (int)line.length());
               }

               for (size_t i = 0; i < line.length(); i++)
               {
                  map.push_back(line[i]);
                  if (line[i] == '@')
                     start = map.size() - 1;
               }

               height++;
            }
            else
            {
               read_map = false;
            }
         }
         else
         {
            for (size_t i = 0; i < line.length(); i++)
               moves.push_back(line[i]);
         }
      }

      // Start GLFW
      if (!glfwInit())
      {
         std::cerr << "Error: GLFW Initialization failed." << std::endl;
         return 1;
      }

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

      // Create the display window
      window = glfwCreateWindow(800, 600, "Advent of Code", nullptr, nullptr);

      if (!window)
      {
         std::cerr << "Error: GLFW Window Creation Failed" << std::endl;
         glfwTerminate();
         return 1;
      }

      // Set up GLFW callback handlers                                             
      glfwSetKeyCallback(window, key_callback);                                    
      glfwSetWindowSizeCallback(window, resize);                                   
                                                                                   
      glfwMakeContextCurrent(window);                                              
                                                                                   
      gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);                          
                                                                                   
      GLCALL(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));                                
                                                                                   
      GLCALL(glEnable(GL_BLEND));                                                  
      GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

      printf("%d x %d\n", width, height);
      printf("Number of moves: %ld\n", moves.size());

      //print_map(map, width);
      //for (size_t i = 0; i < moves.size(); i++)
      //{
      //   move(map, moves[i], start, width, height);
      //   //print_map(map, width);
      //}

      //print_map(map, width);

      // Calculate the time per frame based on the framerate                       
      int framerate = 60;                                                          
      std::chrono::nanoseconds frame_time = std::chrono::nanoseconds(0);           
      if (framerate)                                                               
         frame_time = std::chrono::nanoseconds(1000000000 / framerate);            
                                                                                   
      auto start_frame = std::chrono::high_resolution_clock::now();                
      auto end_frame = start_frame + frame_time;                                   
      auto start_move_timer = std::chrono::high_resolution_clock::now();
                                                                                   
      // Initialize shaders                                                        
      rect_shader = std::make_shared<CShader>("../../../shaders/rect.vert", "../../../shaders/rect.frag");
                                                                                   
      int window_width = 15 * width;                                 
      int window_height = 15 * height;                               
      size_t moves_made = 0;
                                                                                   
      glfwSetWindowSize(window, window_width, window_height);                      
                                                                                   
      while (!glfwWindowShouldClose(window))                                       
      {                                                                            
         render(map, width, height);
                                                                                   
         // Calculate the time taken for the entire frame                          
         auto end = std::chrono::high_resolution_clock::now();                     
         auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_frame);
         auto move_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_move_timer);

         if (move_elapsed.count() > 10 && moves_made < moves.size())
         {
            printf("Move %ld: %c\n", moves_made+1, moves[moves_made]);
            move(map, moves[moves_made++], start, width, height);
            start_move_timer = end;
         }

         // Sleep for the remaining time to maintain the frame rate                
         if (elapsed < frame_time)                                                 
         {                                                                         
            std::this_thread::sleep_until(end_frame);                              
            start_frame = end_frame;                                               
         }                                                                         
         else                                                                      
         {                                                                         
            start_frame = end;                                                     
         }                                                                         
         end_frame = start_frame + frame_time;                                     
      }

      // calculate gps sum
      for (size_t i = 0; i < map.size(); i++)
      {
         if (map[i] == 'O')
         {
            int x = i % width;
            int y = i / width;

            gps_sum += (100 * y) + x;
         }
      }

      printf("GPS sum: %ld\n", gps_sum);
   }
}
