#include "GlPolygon.h"
#include "GlDebug.h"
#include <glm/gtc/matrix_transform.hpp>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#else
#include <glad/glad.h>
#endif
#include <iostream>

unsigned int CGlPolygon::mVAO = 0;
unsigned int CGlPolygon::mVBO = 0;
unsigned int CGlPolygon::mEBO = 0;
bool CGlPolygon::mPolygonsInitialized = false;

// Constructor
CGlPolygon::CGlPolygon(std::shared_ptr<CShader>& Shader, float X, float Y, float Width, float Height)
   : CGlObject(Shader, X, Y, Width, Height),
     mTexture(nullptr)
{
   if (!mPolygonsInitialized)
      InitBuffers();
}

void CGlPolygon::InitBuffers()
{
   float vertices[] = {
      0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
      1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
      1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
      0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
   };

   unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
   };

   GLCALL(glGenVertexArrays(1, &mVAO));
   GLCALL(glGenBuffers(1, &mVBO));
   GLCALL(glGenBuffers(1, &mEBO));

   GLCALL(glBindVertexArray(mVAO));

   GLCALL(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
   GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

   GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO));
   GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

   GLCALL(glEnableVertexAttribArray(0));
   GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
   GLCALL(glEnableVertexAttribArray(1));
   GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));

   GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
   GLCALL(glBindVertexArray(0));

   mPolygonsInitialized = true;
}

glm::vec4 CGlPolygon::GetColor() const
{
   return mColor;
}

void CGlPolygon::SetColor(const glm::vec4& Color)
{
   mColor = Color;
}

void CGlPolygon::SetTexture(std::shared_ptr<CTexture> Texture)
{
   mTexture = Texture;
}

// Render the polygon
void CGlPolygon::Render(const glm::mat4& Projection)
{
   if (!mShader || !mVAO)
   {
      std::cerr << "CGlPolygon: Shader or VAO is not initialized!" << std::endl;
      return;
   }

   mShader->Use();

   // Set the transformation matrix
   glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(mX, mY, 0.0f));
   transform = glm::scale(transform, glm::vec3(mWidth, mHeight, 1.0f));
   mShader->SetUniform("transform", Projection * transform);

   // Set the color
   mShader->SetUniform("uColor", mColor);

   // Set the texture (if available)
   if (mTexture && mTexture->GetTexture())
   {
      mShader->SetUniform("uSampleTexture", 1);
      glBindTexture(GL_TEXTURE_2D, mTexture->GetTexture());
   }
   else
   {
      mShader->SetUniform("uSampleTexture", 0);
   }

   GLCALL(glBindVertexArray(mVAO));
   GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
   GLCALL(glBindVertexArray(0));
}

