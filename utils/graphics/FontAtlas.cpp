
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#else
#include <glad/glad.h>
#endif
#include "FontAtlas.h"
#include "GlDebug.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

// Enable for debug png writing of font atlas
#if 0
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

std::unordered_map<std::string, std::shared_ptr<CFontAtlas>> CFontAtlas::FontMap;
std::vector<std::string> CFontAtlas::AvailableFonts;

CFontAtlas::CFontAtlas(const char* Filename, int PointSize)
   : mGlyphData{},
     mFilename(Filename),
     mFontName(Filename),
     mMaxAdvance(0.0f),
     mMaxAscent(0.0f),
     mMaxDescent(0.0f),
     mPointSize(PointSize),
     mTextureId(0)
{
   DefineTrueTypeFont();

   AvailableFonts.push_back(mFontName);

   // sort alphabetically
   std::sort(AvailableFonts.begin(), AvailableFonts.end(),
      [](const std::string& a, const std::string& b)
      {
         return a < b;
      });
}

CFontAtlas::~CFontAtlas()
{
   if (mTextureId)
   {
      GLCALL(glDeleteTextures(1, &mTextureId));
   }
}

void CFontAtlas::DefineTrueTypeFont()
{
   std::ifstream font(mFilename);
   std::stringstream buffer;

   buffer << font.rdbuf();

   size_t pos = mFontName.find_last_of(".");
   if (pos != std::string::npos)
      mFontName.erase(pos);

   pos = mFontName.find_last_of("/");
   if (pos != std::string::npos)
      mFontName.erase(mFontName.begin(), mFontName.begin() + pos + 1);

   //std::cout << "Read font: " << mFilename << " size " << buffer.str().length() << std::endl;

   // Make a bitmap large enough to hold glyphs
   stbtt_packedchar glyphs[MAX_GLYPHS];
   int width = 1024;
   int max_height = 1024;
   unsigned char* bitmap = new unsigned char[max_height*width];

   memset(bitmap, 0, max_height*width);

   mRange = { static_cast<float>(mPointSize), 32, nullptr, MAX_GLYPHS, glyphs, 0, 0 };

   // Do the packing based on the ranges of point sizes specified
   // TODO: Add support for multiple point sizes (just support 1 for now)
   stbtt_pack_context pc;
   stbtt_PackBegin(&pc, bitmap, width, max_height, 0, 1, nullptr);
   stbtt_PackSetOversampling(&pc, 1, 1);
   stbtt_PackFontRanges(&pc, (const unsigned char*)buffer.str().c_str(), 0, &mRange, 1);
   stbtt_PackEnd(&pc);

   // Get the global metrics for each range
   stbtt_fontinfo info;
   stbtt_InitFont(&info, (const unsigned char*)buffer.str().c_str(), stbtt_GetFontOffsetForIndex((const unsigned char*)buffer.str().c_str(), 0));

   float scale = stbtt_ScaleForPixelHeight(&info, mRange.font_size);
   int ascent;
   int descent;
   int line_gap;
   stbtt_GetFontVMetrics(&info, &ascent, &descent, &line_gap);
   mAscent = ascent * scale;
   mDescent = descent * scale;
   mLineGap = line_gap * scale;

   // Get bitmap height
   int height = 0;
   for (int i = 0; i < MAX_GLYPHS; i++)
   {
      if (glyphs[i].y1 > height)
         height = glyphs[i].y1;
   }

   // generate a 4-channel image from the 1-channel provided by STB
   unsigned char* bitmap_rgba = new unsigned char[height*width*4];

   memset(bitmap_rgba, 0, height*width*4);

   for (int y = 0; y < height; y++)
   {
      for (int x = 0; x < width; x++)
      {
         int src_idx = (y * width) + x;
         int dst_idx = (y * width * 4) + (x * 4);

         bitmap_rgba[dst_idx + 0] = bitmap[src_idx];
         bitmap_rgba[dst_idx + 1] = bitmap[src_idx];
         bitmap_rgba[dst_idx + 2] = bitmap[src_idx];
         bitmap_rgba[dst_idx + 3] = 255;
      }
   }

   for (int i = 0; i < MAX_GLYPHS; i++)
   {
      mGlyphData[i].x0 = (float)glyphs[i].x0 / (float)width;
      mGlyphData[i].y0 = (float)glyphs[i].y0 / (float)height;
      mGlyphData[i].x1 = (float)glyphs[i].x1 / (float)width;
      mGlyphData[i].y1 = (float)glyphs[i].y1 / (float)height;
      mGlyphData[i].x_off = glyphs[i].xoff;
      mGlyphData[i].y_off = glyphs[i].yoff;
      mGlyphData[i].size_x = glyphs[i].xoff2 - glyphs[i].xoff;
      mGlyphData[i].size_y = abs(glyphs[i].yoff2 - glyphs[i].yoff);
      mGlyphData[i].advance = glyphs[i].xadvance;
   }

   // Create OpenGL texture atlas
   GLCALL(glGenTextures(1, &mTextureId));
   GLCALL(glBindTexture(GL_TEXTURE_2D, mTextureId));
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
   GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap_rgba));

   //std::cout << "Created font atlas " << mTextureId << "(" << width << ", " << height << ")" << std::endl;

#if 0
   // Debug - write font atlas to png
   std::string png_file = mFontName + ".png";
   stbi_write_png(png_file.c_str(), width, height, 4, bitmap_rgba, width * 4);
#endif

   delete [] bitmap;
   delete [] bitmap_rgba;
}

