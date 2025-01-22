#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GlDebug.h"

GLFWwindow* window;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (window == nullptr || scancode == -1 || mods == -1)
      return;

   //if (key == GLFW_KEY_E && action == GLFW_PRESS)
   //{
   //   page_manager.HandleKeyboardInput(CPageManager::T_EDIT_KEY, true);
   //}
}

void resize(GLFWwindow* window, int width, int height)
{
   if (window == nullptr)
      return;

   GLCALL(glViewport(0, 0, width, height));
}

void render()
{
   //// Start a new ImGui frame
   //ImGui_ImplOpenGL3_NewFrame();
   //ImGui_ImplGlfw_NewFrame();
   //ImGui::NewFrame();

   //ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

   // Clear the window with the background color
   GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

   //// Finish ImGui frame
   //ImGui::Render();
   //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

   //// Update and Render additional Platform Windows
   //ImGui::UpdatePlatformWindows();
   //ImGui::RenderPlatformWindowsDefault();

   glfwSwapBuffers(window);
   glfwPollEvents();
}

void print_map(const std::vector<int>& Map, int Width, int Height)
{
   for (int y = 0; y < Height; y++)
   {
      for (int x = 0; x < Width; x++)
      {
         int idx = (y * Width) + x;
         std::cout << Map[idx];
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}

void check_trailhead(const std::vector<int>& Map, int Idx, int PrevScore, int Width, int Height, std::unordered_map<int, bool>& TrailheadScore)
{
   int x = Idx % Width;
   int y = Idx / Width;
   int score = 0;

   if (x < 0 || x >= Width ||
       y < 0 || y >= Height)
   {
      return;
   }

   score = Map[Idx];

   if (score != PrevScore + 1)
   {
      return;
   }

   if (score == 9)
   {
      // Add to hash map
      //if (TrailheadScore.find(Idx) == TrailheadScore.end())
      //{
      //   printf("Add (%d, %d)\n", x, y);
      //}
      TrailheadScore[Idx] = true;
      return;
   }

   // Check up
   check_trailhead(Map, Idx - Width, score, Width, Height, TrailheadScore);

   // Check down 
   check_trailhead(Map, Idx + Width, score, Width, Height, TrailheadScore);

   // Check right 
   check_trailhead(Map, Idx + 1, score, Width, Height, TrailheadScore);

   // Check left 
   check_trailhead(Map, Idx - 1, score, Width, Height, TrailheadScore);
}

int main()
{
   std::ifstream file("../test.txt");

   if (file.is_open())
   {
      std::vector<int> input;
      std::unordered_map<int, std::unordered_map<int, bool>> trailheads;
      int width = 0;
      int height = 0;
      uint64_t result = 0;

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
         else
         {
            assert(width == line.length());
         }

         for (const auto& c : line)
         {
            input.push_back((int)(c - '0'));
         }

         height++;
      }

      printf("%d x %d\n", width, height);
      print_map(input, width, height);

      for (size_t i = 0; i < input.size(); i++)
      {
         if (input[i] == 0)
         {
            check_trailhead(input, i, -1, width, height, trailheads[i]);
            result += trailheads[i].size();
            //printf("Result: (%d, %d) %d\n", i % width, i / width, trailheads[i].size());
         }
      }

      printf("Result: %d\n", result);
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

   GLCALL(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));

   GLCALL(glEnable(GL_BLEND));
   GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

   GLCALL(glEnable(GL_STENCIL_TEST));
   GLCALL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));

   //// Initialize ImGui
   //IMGUI_CHECKVERSION();
   //ImGui::CreateContext();
   //ImGuiIO& io = ImGui::GetIO();

   //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

   //ImGui::StyleColorsDark();

   //// Initialize ImGui backends
   //ImGui_ImplGlfw_InitForOpenGL(window, true);
   //ImGui_ImplOpenGL3_Init();

   //// Load fonts
   //std::cout << "Load ImGui fonts" << std::endl;
   //io.Fonts->AddFontFromFileTTF("../data/fonts/ProggyClean.ttf", 13.0f);
   //io.Fonts->AddFontDefault();

   //// Initialize fonts
   //CFontAtlas::FontMap["DroidSansMono"] = std::make_shared<CFontAtlas>("../data/fonts/DroidSansMono.ttf", 21);
   //CFontAtlas::FontMap["DejaVuSansMono"] = std::make_shared<CFontAtlas>("../data/fonts/DejaVuSansMono.ttf", 21);
   //CFontAtlas::FontMap["FiraCode-Regular"] = std::make_shared<CFontAtlas>("../data/fonts/FiraCode-Regular.ttf", 21);
   //CFontAtlas::FontMap["ProggyClean"] = std::make_shared<CFontAtlas>("../data/fonts/ProggyClean.ttf", 21);


   // Calculate the time per frame based on the framerate
   int framerate = 60; 
   std::chrono::nanoseconds frame_time = std::chrono::nanoseconds(0);
   if (framerate)
      frame_time = std::chrono::nanoseconds(1000000000 / framerate);

   auto start_frame = std::chrono::high_resolution_clock::now();
   auto end_frame = start_frame + frame_time;

   while (!glfwWindowShouldClose(window))
   {
      render();

      // Calculate the time taken for the entire frame
      auto end = std::chrono::high_resolution_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_frame);

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

   glfwDestroyWindow(window);
   glfwTerminate();
}
