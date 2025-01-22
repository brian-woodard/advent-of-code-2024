#include "GlTextAtlas.h"
#include "GlDebug.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#else
#include <glad/glad.h>
#endif
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

unsigned int CGlTextAtlas::mVAO = 0;
unsigned int CGlTextAtlas::mVBO = 0;
bool CGlTextAtlas::mBuffersInitialized = false;

CGlTextAtlas::CGlTextAtlas(std::shared_ptr<CShader>& Shader, float X, float Y, float Width, float Height, std::shared_ptr<CFontAtlas> FontAtlas, float Scale)
   : CGlObject(Shader, X, Y, Width, Height), mTextColor(1.0f), mFontAtlas(FontAtlas), mScale(Scale)
{
   if (!mBuffersInitialized)
      InitBuffers();
}

void CGlTextAtlas::InitBuffers()
{
   GLCALL(glGenVertexArrays(1, &mVAO));
   GLCALL(glGenBuffers(1, &mVBO));
   GLCALL(glBindVertexArray(mVAO));
   GLCALL(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
   GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW));
   GLCALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr));
   GLCALL(glEnableVertexAttribArray(0));

   mBuffersInitialized = true;
}

void CGlTextAtlas::CalculateTextArea()
{
   float total_width = 0.0f;
   float max_height = 0.0f;

   const CFontAtlas::TGlyphData* glyph_data = mFontAtlas->GetGlyphData();
   for (const char& c : mTextContent)
   {
      int c_idx = static_cast<int>(c - 32);

      // Adjust glyph width and height to percentage, scaled by the text's scale factor
      float adjustedHeight = (glyph_data[c_idx].size_y * 0.2f) * mScale;

      // Advance cursor for the next character
      total_width += (glyph_data[c_idx].advance * 0.2f) * mScale;

      // Track the maximum height
      if (adjustedHeight > max_height)
      {
         max_height = adjustedHeight;
      }
   }

   // Update mWidth and mHeight
   mWidth = total_width;
   mHeight = max_height;
}


glm::vec4 CGlTextAtlas::GetColor() const
{
   return mTextColor;
}

void CGlTextAtlas::SetColor(const glm::vec4& Color)
{
   mTextColor = Color;
}

std::string CGlTextAtlas::GetText() const
{
   return mTextContent;
}

void CGlTextAtlas::SetText(const std::string& Text)
{
   mTextContent = Text;
   CalculateTextArea();
}

void CGlTextAtlas::SetScale(float ScaleValue)
{
   mScale = ScaleValue;
}

void CGlTextAtlas::Render(const glm::mat4& Projection)
{
   if (!mShader)
   {
      std::cerr << "Shader not set for text rendering!" << std::endl;
      return;
   }

   mShader->Use();
   mShader->SetUniform("projection", Projection);

   mShader->SetUniform("textColor", mTextColor);

   GLCALL(glBindVertexArray(mVAO));

   float x = mX;
   float y = mY;
   const CFontAtlas::TGlyphData* glyph_data = mFontAtlas->GetGlyphData();
   GLCALL(glBindTexture(GL_TEXTURE_2D, mFontAtlas->GetTexture()));
   for (const char& c : mTextContent)
   {
      int c_idx = static_cast<int>(c - 32);

      // Adjust glyph width and height to percentage, scaled by the text's scale factor
      float adjustedWidth = (glyph_data[c_idx].size_x * 0.2f) * mScale;
      float adjustedHeight = (glyph_data[c_idx].size_y * 0.2f) * mScale;

      // Calculate positions for the current glyph
      float xpos = x + (glyph_data[c_idx].x_off * 0.2f);
      float ypos = y - ((glyph_data[c_idx].size_y + glyph_data[c_idx].y_off) * 0.2f);

      float vertices[6][4] = {
         { xpos,                 ypos + adjustedHeight, glyph_data[c_idx].x0, glyph_data[c_idx].y0 },
         { xpos,                 ypos,                  glyph_data[c_idx].x0, glyph_data[c_idx].y1 },
         { xpos + adjustedWidth, ypos,                  glyph_data[c_idx].x1, glyph_data[c_idx].y1 },

         { xpos,                 ypos + adjustedHeight, glyph_data[c_idx].x0, glyph_data[c_idx].y0 },
         { xpos + adjustedWidth, ypos,                  glyph_data[c_idx].x1, glyph_data[c_idx].y1 },
         { xpos + adjustedWidth, ypos + adjustedHeight, glyph_data[c_idx].x1, glyph_data[c_idx].y0 }
      };

      GLCALL(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
      GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

      GLCALL(glDrawArrays(GL_TRIANGLES, 0, 6));

      // Advance position for the next character
      x += (glyph_data[c_idx].advance * 0.2f) * mScale;
   }

   GLCALL(glBindVertexArray(0));
   GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

