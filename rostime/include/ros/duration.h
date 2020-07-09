/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2008, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Willow Garage, Inc. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

#ifndef ROS_DURATION_H
#define ROS_DURATION_H

/*********************************************************************
 ** Pragmas
 *********************************************************************/

#ifdef _MSC_VER
  // Rostime has some magic interface that doesn't directly include
  // its implementation, this just disbales those warnings.
  #pragma warning(disable: 4244)
  #pragma warning(disable: 4661)
#endif

#include <iostream>
#include <math.h>
#include <stdexcept>
#include <climits>
#include <stdint.h>
#include "rostime_decl.h"

namespace boost {
  namespace posix_time {
    class time_duration;
  }
}

namespace ros
{
ROSTIME_DECL void normalizeSecNSecSigned(int64_t& sec, int64_t& nsec);
ROSTIME_DECL void normalizeSecNSecSigned(int32_t& sec, int32_t& nsec);

/**
 * \brief Base class for Duration implementations.  Provides storage, common functions and operator overloads.
 * This should not need to be used directly.
 */
template <class T>
class DurationBase
{
public:
  int32_t sec, nsec;
  DurationBase() : sec(0), nsec(0) { }
  DurationBase(int32_t _sec, int32_t _nsec);
  explicit DurationBase(double t){fromSec(t);};
  T operator+(const T &rhs) const;
  T operator-(const T &rhs) const;
  T operator-() const;
  T operator*(double scale) const;
  T& operator+=(const T &rhs);
  T& operator-=(const T &rhs);
  T& operator*=(double scale);
  bool operator==(const T &rhs) const;
  inline bool operator!=(const T &rhs) const { return !(*static_cast<const T*>(this) == rhs); }
  bool operator>(const T &rhs) const;
  bool operator<(const T &rhs) const;
  bool operator>=(const T &rhs) const;
  bool operator<=(const T &rhs) const;
  double toSec() const { return static_cast<double>(sec) + 1e-9*static_cast<double>(nsec); };
  int64_t toNSec() const {return static_cast<int64_t>(sec)*1000000000ll + static_cast<int64_t>(nsec);  };
  T& fromSec(double t);
  T& fromNSec(int64_t t);
  bool isZero() const;
  boost::posix_time::time_duration toBoost() const;
  static const T MIN; //!< Minimum representable duration (negative)
  static const T MAX; //!< Maximum representable duration
  static const T ZERO; //!< Zero duration
  static const T YEAR; //!< One year duration (Gregorian year of 365.2425 days)
  static const T WEEK; //!< One week duration
  static const T DAY; //!< One day duration
  static const T HOUR; //!< One hour duration
  static const T MINUTE; //!< One minute duration
  static const T SECOND; //!< One second duration
  static const T MILLISECOND; //!< One millisecond duration
  static const T MICROSECOND; //!< One microsecond duration
  static const T NANOSECOND; //!< One nanosecond duration
};

class Rate;

/**
 * \brief Duration representation for use with the Time class.
 *
 * ros::DurationBase provides most of its functionality.
 */
class ROSTIME_DECL Duration : public DurationBase<Duration>
{
public:
  Duration()
  : DurationBase<Duration>()
  { }

  Duration(int32_t _sec, int32_t _nsec)
  : DurationBase<Duration>(_sec, _nsec)
  {}

  explicit Duration(double t) { fromSec(t); }
  explicit Duration(const Rate&);
  /**
   * \brief sleep for the amount of time specified by this Duration.  If a signal interrupts the sleep, resleeps for the time remaining.
   * @return True if the desired sleep duration was met, false otherwise.
   */
  bool sleep() const;
};

extern ROSTIME_DECL const Duration DURATION_MAX;
extern ROSTIME_DECL const Duration DURATION_MIN;
template<> const Duration DurationBase<Duration>::MAX;
template<> const Duration DurationBase<Duration>::MIN;
template<> const Duration DurationBase<Duration>::ZERO;
template<> const Duration DurationBase<Duration>::YEAR;
template<> const Duration DurationBase<Duration>::WEEK;
template<> const Duration DurationBase<Duration>::DAY;
template<> const Duration DurationBase<Duration>::HOUR;
template<> const Duration DurationBase<Duration>::MINUTE;
template<> const Duration DurationBase<Duration>::SECOND;
template<> const Duration DurationBase<Duration>::MILLISECOND;
template<> const Duration DurationBase<Duration>::MICROSECOND;
template<> const Duration DurationBase<Duration>::NANOSECOND;

/**
 * \brief Duration representation for use with the WallTime class.
 *
 * ros::DurationBase provides most of its functionality.
 */
class ROSTIME_DECL WallDuration : public DurationBase<WallDuration>
{
public:
  WallDuration()
  : DurationBase<WallDuration>()
  { }

  WallDuration(int32_t _sec, int32_t _nsec)
  : DurationBase<WallDuration>(_sec, _nsec)
  {}

  explicit WallDuration(double t) { fromSec(t); }
  explicit WallDuration(const Rate&);
  /**
   * \brief sleep for the amount of time specified by this Duration.  If a signal interrupts the sleep, resleeps for the time remaining.
   * @return True if the desired sleep duration was met, false otherwise.
   */
  bool sleep() const;
};

template<> const WallDuration DurationBase<WallDuration>::MAX;
template<> const WallDuration DurationBase<WallDuration>::MIN;
template<> const WallDuration DurationBase<WallDuration>::ZERO;
template<> const WallDuration DurationBase<WallDuration>::YEAR;
template<> const WallDuration DurationBase<WallDuration>::WEEK;
template<> const WallDuration DurationBase<WallDuration>::DAY;
template<> const WallDuration DurationBase<WallDuration>::HOUR;
template<> const WallDuration DurationBase<WallDuration>::MINUTE;
template<> const WallDuration DurationBase<WallDuration>::SECOND;
template<> const WallDuration DurationBase<WallDuration>::MILLISECOND;
template<> const WallDuration DurationBase<WallDuration>::MICROSECOND;
template<> const WallDuration DurationBase<WallDuration>::NANOSECOND;

ROSTIME_DECL std::ostream &operator <<(std::ostream &os, const Duration &rhs);
ROSTIME_DECL std::ostream &operator <<(std::ostream &os, const WallDuration &rhs);

}

#endif // ROS_DURATION_H
