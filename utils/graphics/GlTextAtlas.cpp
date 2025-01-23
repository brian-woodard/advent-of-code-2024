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

CGlTextAtlas::CGlTextAtlas(std::shared_ptr<CShader>& Shader, float X, float Y, float Width, float Height, std::shared_ptr<CFontAtlas> FontAtlas, float Scale, bool FlipVertical)
   : CGlObject(Shader, X, Y, Width, Height), mTextColor(1.0f), mFontAtlas(FontAtlas), mScale(Scale), mFlipVertical(FlipVertical)
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
      float adjustedHeight = (glyph_data[c_idx].size_y * 1.0f) * mScale;

      // Advance cursor for the next character
      total_width += (glyph_data[c_idx].advance * 1.0f) * mScale;

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
      float adjustedWidth = (glyph_data[c_idx].size_x * 1.0f) * mScale;
      float adjustedHeight = (glyph_data[c_idx].size_y * 1.0f) * mScale;

      // Calculate positions for the current glyph
      float xpos = x + (glyph_data[c_idx].x_off * 1.0f);
      float ypos = y - ((glyph_data[c_idx].size_y + glyph_data[c_idx].y_off) * 1.0f);

      glm::vec2 tex_coord_ll;
      glm::vec2 tex_coord_ur;

      if (mFlipVertical)
      {
         tex_coord_ll.x = glyph_data[c_idx].x0;
         tex_coord_ll.y = glyph_data[c_idx].y1;
         tex_coord_ur.x = glyph_data[c_idx].x1;
         tex_coord_ur.y = glyph_data[c_idx].y0;
      }
      else
      {
         tex_coord_ll.x = glyph_data[c_idx].x0;
         tex_coord_ll.y = glyph_data[c_idx].y0;
         tex_coord_ur.x = glyph_data[c_idx].x1;
         tex_coord_ur.y = glyph_data[c_idx].y1;
      }

      float vertices[6][4] = {
         { xpos - adjustedWidth * 0.5f, ypos + adjustedHeight * 0.5f, tex_coord_ll.x, tex_coord_ll.y },
         { xpos - adjustedWidth * 0.5f, ypos - adjustedHeight * 0.5f, tex_coord_ll.x, tex_coord_ur.y },
         { xpos + adjustedWidth * 0.5f, ypos - adjustedHeight * 0.5f, tex_coord_ur.x, tex_coord_ur.y },

         { xpos - adjustedWidth * 0.5f, ypos + adjustedHeight * 0.5f, tex_coord_ll.x, tex_coord_ll.y },
         { xpos + adjustedWidth * 0.5f, ypos - adjustedHeight * 0.5f, tex_coord_ur.x, tex_coord_ur.y },
         { xpos + adjustedWidth * 0.5f, ypos + adjustedHeight * 0.5f, tex_coord_ur.x, tex_coord_ll.y }
      };

      GLCALL(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
      GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

      GLCALL(glDrawArrays(GL_TRIANGLES, 0, 6));

      // Advance position for the next character
      x += (glyph_data[c_idx].advance * 1.0f) * mScale;
   }

   GLCALL(glBindVertexArray(0));
   GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

