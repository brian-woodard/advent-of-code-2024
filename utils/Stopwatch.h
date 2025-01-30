///////////////////////////////////////////////////////////////////////////////
// 
// File: Stopwatch.h
// Date:
// Revision:
// Creator: Brian Woodard
// License: (C) Copyright 2024 by Everus Engineering LLC. All Rights Reserved.
//
// Scoped timing class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <chrono>

class CStopwatch
{
public:

   typedef std::chrono::duration<double> double_secs;

   //! \fn CStopwatch()
   //! \details A manual timer constructor. User must call Start() and then
   //!          GetTime() manually to get the duration in seconds.
   CStopwatch() : mDuration(nullptr) {}

   //! \fn CStopwatch(double* Duration)
   //! \details A scoped timer constructor, accepts a pointer for where to
   //!          store the duration in seconds upon destruction.
   CStopwatch(double* Duration);

   //! \fn ~CStopwatch()
   //! \details A scoped timer destructor, calculates the duration in seconds
   //!          and sets that to the mDuration passed in on construction.
   ~CStopwatch();

   //! \fn double GetTime()
   //! \details Returns the time, in seconds, since the stopwatch was started
   double GetTime();

   //! \fn void Start()
   //! \details Starts the stopwatch timer
   void Start();

private:

   std::chrono::high_resolution_clock::time_point mStart;
   std::chrono::high_resolution_clock::time_point mEnd;
   double*                                        mDuration;

};
