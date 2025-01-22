#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <stb_truetype.h>

// TODO: Add support for multiple point sizes in one atlas

class CFontAtlas
{
public:

   // Hash map to hold fonts
   static std::unordered_map<std::string, std::shared_ptr<CFontAtlas>> FontMap;
   static std::vector<std::string> AvailableFonts;

   static constexpr int MAX_GLYPHS = 95;

   struct TGlyphData
   {
      float x0;
      float y0;
      float x1;
      float y1;
      int x_off;
      int y_off;
      int size_x;
      int size_y;
      int advance;
      int descent;
   };

   CFontAtlas(const char* Filename, int PointSize);
   ~CFontAtlas();

   //! \fn const std::string& GetFontName()
   //! \details Returns the font name defined in the font record
   const std::string& GetFontName() const { return mFontName; };

   //! \fn stbtt_packedchar* GetGlyphData()
   //! \details Returns the glyph data for the font
   const TGlyphData* GetGlyphData() const { return mGlyphData; };

   //! \fn float GetMaxAdvance()
   //! \details Returns the max advance defined in the font record
   float GetMaxAdvance() const { return mMaxAdvance; };

   //! \fn float GetMaxAscent()
   //! \details Returns the max ascent defined in the font record
   float GetMaxAscent() const { return mMaxAscent; };

   //! \fn unsigned int GetTexture()
   //! \details Returns the texture identifier for this atlas
   unsigned int GetTexture() const { return mTextureId; };

private:

   //! \fn void DefineTrueTypeFont()
   //! \details Creates a texture atlas from the TrueTypeFont
   void DefineTrueTypeFont();

   TGlyphData       mGlyphData[MAX_GLYPHS];
   stbtt_pack_range mRange;
   std::string      mFilename;
   std::string      mFontName;
   float            mMaxAdvance;
   float            mMaxAscent;
   float            mMaxDescent;
   float            mAscent;
   float            mDescent;
   float            mLineGap;
   int              mPointSize;
   unsigned int     mTextureId;
};

