#pragma once

#include <memory>
#include "GlObject.h"
#include "Texture.h"

class CGlPolygon : public CGlObject
{
public:
   // Constructor
   CGlPolygon(std::shared_ptr<CShader>& Shader, float X, float Y, float Width, float Height);

   // Render function
   void Render(const glm::mat4& Projection) override;

   glm::vec4 GetColor() const override;
   void SetColor(const glm::vec4& Color) override;

   void SetTexture(std::shared_ptr<CTexture> Texture);

private:
   void InitBuffers();     // Initialize VAO, VBO, and EBO

   static unsigned int mVAO;            // Vertex Array Object
   static unsigned int mVBO;            // Vertex Buffer Object
   static unsigned int mEBO;            // Element Buffer Object for polygons
   glm::vec4 mColor;
   std::shared_ptr<CTexture> mTexture;

   static bool mPolygonsInitialized;

};

