///////////////////////////////////////////////////////////////////////////////
// 
// File: Stopwatch.cpp
// Date:
// Revision:
// Creator: Brian Woodard
// License: (C) Copyright 2024 by Everus Engineering LLC. All Rights Reserved.
//
// Scoped timing class
//
///////////////////////////////////////////////////////////////////////////////
#include "Stopwatch.h"

CStopwatch::CStopwatch(double* Duration)
   : mStart (std::chrono::high_resolution_clock::now()),
     mDuration (Duration)
{
}

CStopwatch::~CStopwatch()
{
   if (mDuration)
   {
      mEnd = std::chrono::high_resolution_clock::now();

      auto duration = std::chrono::duration_cast<double_secs>(mEnd - mStart);
      *mDuration = duration.count();
   }
}

double CStopwatch::GetTime()
{
   mEnd = std::chrono::high_resolution_clock::now();

   return std::chrono::duration_cast<double_secs>(mEnd - mStart).count();
}

void CStopwatch::Start()
{
   mStart = std::chrono::high_resolution_clock::now();
}

