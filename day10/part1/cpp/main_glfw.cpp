#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <chrono>
#include <thread>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GlDebug.h"
#include "FontAtlas.h"
#include "Shader.h"
#include "GlRect.h"
#include "GlTextAtlas.h"

GLFWwindow*              window;
std::shared_ptr<CShader> polygon_shader;
std::shared_ptr<CShader> text_shader;
std::shared_ptr<CShader> rect_shader;
std::vector<int> input;
std::unordered_map<int, std::unordered_map<int, bool>> trailheads;
int width = 0;
int height = 0;
bool done = false;
bool pause = false;

struct TSquare
{
   int Index;
   int Color; // 0 = Green, 1 = White, 2 = Yellow
};

std::vector<TSquare> squares;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (window == nullptr || scancode == -1 || mods == -1)
      return;

   if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
   {
      pause = !pause;
   }
}

void resize(GLFWwindow* window, int width, int height)
{
   if (window == nullptr)
      return;

   GLCALL(glViewport(0, 0, width, height));
}

void render()
{
   glm::mat4 projection = glm::ortho(0.0f, 100.0f, 100.0f, 0.0f, -1.0f, 1.0f);

   //// Start a new ImGui frame
   //ImGui_ImplOpenGL3_NewFrame();
   //ImGui_ImplGlfw_NewFrame();
   //ImGui::NewFrame();

   //ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

   // Clear the window with the background color
   GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

   CGlTextAtlas text = CGlTextAtlas(text_shader, 0.0f, 0.0f, 0.0f, 0.0f, CFontAtlas::FontMap["DroidSansMono"], 0.25f, true);
   text.SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

   glm::vec2 adv = glm::vec2((100.0f / (float)width), (100.0f / (float)height));

   // draw any squares
   CGlRect rect = CGlRect(rect_shader, adv.x * 0.5 + adv.x * 2.0f, adv.y * 0.5 + adv.y * 3.0f, adv.x, adv.y);
   rect.SetColor(glm::vec4(1.0f));

   for (const auto& square : squares)
   {
      int x = square.Index % width;
      int y = square.Index / width;

      glm::vec2 pos = glm::vec2(adv.x * 0.5 + (adv.x * x),
                                adv.y * 0.5 + (adv.y * y));

      if (square.Color == 0)
         rect.SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
      else if (square.Color == 1)
         rect.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.2f));
      else if (square.Color == 2)
         rect.SetColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

      rect.SetPosition(pos.x, pos.y);
      rect.Render(projection);
   }

   glm::vec2 pos = glm::vec2(adv.x * 0.5f, adv.y * 0.5f);

   for (int y = 0; y < height; y++)
   {
      for (int x = 0; x < width; x++)
      {
         int idx = (y * width) + x;

         text.SetPosition(pos.x, pos.y);
         text.SetText(std::to_string(input[idx]));
         text.Render(projection);

         pos.x += adv.x;
      }

      pos.y += adv.y;
      pos.x = adv.x * 0.5f;
   }

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

   if (pause)
   {
      printf(">>> Paused\n");
      while (pause)
      {
         std::this_thread::sleep_for(std::chrono::milliseconds(20));
      }
   }

   if (score == 9)
   {
      // Add to hash map
      if (TrailheadScore.find(Idx) == TrailheadScore.end())
      {
         //printf("Add (%d, %d)\n", x, y);
         squares.push_back({Idx, 2});
         std::this_thread::sleep_for(std::chrono::milliseconds(200));
      }
      TrailheadScore[Idx] = true;
      return;
   }
   else
   {
      bool found = false;
      for (const auto& square : squares)
      {
         if (square.Index == Idx)
         {
            found = true;
            break;
         }
      }

      //if (!found)
      {
         squares.push_back({Idx, 1});
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
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

void check_trailhead_thread()
{
   while (!done)
   {
      uint64_t result = 0;

      trailheads.clear();

      for (size_t i = 0; i < input.size(); i++)
      {
         if (input[i] == 0)
         {
            squares.push_back({i, 0});
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            check_trailhead(input, i, -1, width, height, trailheads[i]);
            result += trailheads[i].size();
            //printf("Result: (%d, %d) %ld\n", i % width, i / width, trailheads[i].size());
            squares.clear();
         }
      }

      printf("Result: %ld\n", result);
   }
}

int main()
{
   std::ifstream file("../input.txt");

   if (file.is_open())
   {
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
            assert(width == (int)line.length());
         }

         for (const auto& c : line)
         {
            input.push_back((int)(c - '0'));
         }

         height++;
      }

      printf("%d x %d\n", width, height);
      print_map(input, width, height);
   }

   std::thread thr(check_trailhead_thread);

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

   // Calculate the time per frame based on the framerate
   int framerate = 60; 
   std::chrono::nanoseconds frame_time = std::chrono::nanoseconds(0);
   if (framerate)
      frame_time = std::chrono::nanoseconds(1000000000 / framerate);

   auto start_frame = std::chrono::high_resolution_clock::now();
   auto end_frame = start_frame + frame_time;

   // Initialize fonts
   CFontAtlas::FontMap["DroidSansMono"] = std::make_shared<CFontAtlas>("../../../fonts/DroidSansMono.ttf", 12.0f);

   // Initialize shaders
   polygon_shader = std::make_shared<CShader>("../../../shaders/polygon.vert", "../../../shaders/polygon.frag");
   text_shader = std::make_shared<CShader>("../../../shaders/text.vert", "../../../shaders/text.frag");
   rect_shader = std::make_shared<CShader>("../../../shaders/rect.vert", "../../../shaders/rect.frag");

   // Calculate initial window size
   CGlTextAtlas text = CGlTextAtlas(text_shader, 50.0f, 50.0f, 0.0f, 0.0f, CFontAtlas::FontMap["DroidSansMono"]);
   text.SetColor(glm::vec4(1.0f));
   text.SetText("0");
   text.CalculateTextArea();

   //int window_width = text.GetSize().x * 4.0f * width;
   //int window_height = text.GetSize().y * 3.0f * height;
   int window_width = text.GetSize().x * width;
   int window_height = text.GetSize().y * height;

   printf("text (%f, %f) win (%d, %d)\n", text.GetSize().x, text.GetSize().y, window_width, window_height);
   glfwSetWindowSize(window, window_width, window_height);

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

   done = true;
   thr.join();

   glfwDestroyWindow(window);
   glfwTerminate();
}
