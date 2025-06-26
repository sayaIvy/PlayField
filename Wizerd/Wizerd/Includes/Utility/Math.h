#pragma once

class Math
{
public:
  template<typename T>
  static T Add(T a, T b)
  {
    return a + b;
  }

  template<typename T>
  static T Subtract(T a, T b)
  {
    return a - b;
  }

  template<typename T>
  static T Multiply(T a, T b)
  {
    return a * b;
  }

  template<typename T>
  static T Divide(T a, T b)
  {
    if (b == 0) return 0;
    return a / b;
  }

  template<typename T>
  static T Abs(T value)
  {
    return (value < 0) ? -value : value;
  }

  template<typename T>
  static T Min(T a, T b)
  {
    return (a < b) ? a : b;
  }

  template<typename T>
  static T Max(T a, T b)
  {
    return (a > b) ? a : b;
  }

  template<typename T>
  static T Lerp(T a, T b, T t)
  {
    return a + (b - a) * t;
  }

  template<typename T>
  static T Clamp(T value, T min, T max)
  {
    if (value < min) return min;
    if (value > max) return max;
    return value;
  }

  template<typename T>
  static T Normalize(T value, T min, T max)
  {
    return (value - min) / (max - min);
  }
};
