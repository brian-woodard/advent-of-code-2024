
#include <iostream>
#include <algorithm>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#else
#include <glad/glad.h>
#endif
#include "Texture.h"
#include "GlDebug.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unordered_map<std::string, std::shared_ptr<CTexture>> CTexture::TextureMap;
std::vector<std::string> CTexture::AvailableTextures;

CTexture::CTexture(const char* Filename)
   : mFilename(Filename),
     mTextureId(0),
     mWidth(0),
     mHeight(0),
     mChannels(0)
{
   if (strlen(Filename) == 0)
      return;

   stbi_set_flip_vertically_on_load(1);
   unsigned char* data = stbi_load(mFilename.c_str(), &mWidth, &mHeight, &mChannels, 0);

   //std::cout << "Loaded texture (" << mWidth << ", " << mHeight << ") channels " << mChannels << std::endl;

   if (!data)
   {
      std::cerr << "Failed to load texture: " << mFilename << std::endl;
      return;
   }

   GLCALL(glGenTextures(1, &mTextureId));
   GLCALL(glBindTexture(GL_TEXTURE_2D, mTextureId));

   // Using GL_RGBA in case the image has alpha channels
   if (mChannels == 4)
   {
      GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
      GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
   }
   else
   {
      GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // Disable byte-alignment restriction
      GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, mWidth, mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data));
   }

   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

   stbi_image_free(data);

   GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

CTexture::~CTexture()
{
   if (mTextureId)
   {
      GLCALL(glDeleteTextures(1, &mTextureId));
   }
}

std::shared_ptr<CTexture> GetOrCreateTexture(const char* Filename)
{
   std::shared_ptr<CTexture> texture_ptr = nullptr;

   if (CTexture::TextureMap.find(Filename) != CTexture::TextureMap.end())
   {
      texture_ptr = CTexture::TextureMap[Filename];
   }
   else
   {
      // Try loading the texture
      texture_ptr = std::make_shared<CTexture>(Filename);
      if (texture_ptr->GetTexture())
      {
         CTexture::TextureMap[Filename] = texture_ptr;

         CTexture::AvailableTextures.push_back(Filename);

         // Sort alphabetically
         std::sort(CTexture::AvailableTextures.begin(), CTexture::AvailableTextures.end(),
            [](const std::string& a, const std::string& b)
            {
               return a < b;
            });
      }
      else
      {
         texture_ptr = nullptr;
      }
   }

   return texture_ptr;
}

