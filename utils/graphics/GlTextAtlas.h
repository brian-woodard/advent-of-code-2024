#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "GlObject.h"
#include "FontAtlas.h"

class CGlTextAtlas : public CGlObject
{
public:
   CGlTextAtlas(std::shared_ptr<CShader>& Shader, float X, float Y, float Width, float Height, std::shared_ptr<CFontAtlas> FontAtlas, float Scale = 1.0, bool FlipVertical = false);
   ~CGlTextAtlas() override = default;

   void CalculateTextArea();

   glm::vec4 GetColor() const override;
   void SetColor(const glm::vec4& Color) override;

   std::string GetText() const override;
   void SetText(const std::string& Text) override;
   void SetScale(float ScaleValue);
   void SetFont(std::shared_ptr<CFontAtlas> FontAtlas) {mFontAtlas = FontAtlas;}

   void Render(const glm::mat4& Projection) override;

private:
   void InitBuffers();

   std::string mTextContent;
   glm::vec4 mTextColor;
   std::shared_ptr<CFontAtlas> mFontAtlas;
   float mScale;
   bool mFlipVertical;

   static unsigned int mVAO; // Vertex Array Object
   static unsigned int mVBO; // Vertex Buffer Object
   static bool mBuffersInitialized;
};

