// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"
#include "Accumulator.h"

using namespace std;
using namespace Common;
using namespace Reliability::LoadBalancingComponent;

Accumulator::Accumulator()
    : count_(0),
      sum_(0.0),
      squaredSum_(0.0)
{
}

Accumulator::Accumulator(Accumulator const& other)
    : count_(other.count_),
      sum_(other.sum_),
      squaredSum_(other.squaredSum_)
{
}

Accumulator::~Accumulator()
{
}

void Accumulator::Clear()
{
    count_ = 0;
    sum_ = 0.0;
    squaredSum_ = 0.0;
}

Accumulator & Accumulator::operator= (Accumulator const & other)
{
    if (this != &other)
    {
        count_ = other.count_;
        sum_ = other.sum_;
        squaredSum_ = other.squaredSum_;
    }

    return *this;
}

void Accumulator::AddOneValue(int64 value)
{
    double val = static_cast<double>(value);
    ++count_;
    sum_ += val;
    squaredSum_ += val * val;
}

void Accumulator::AdjustOneValue(int64 oldValue, int64 newValue)
{
    ASSERT_IFNOT( count_ > 0, "Accumulator is empty, not able to adjust values");

    if (oldValue == newValue)
    {
        return;
    }

    double oldVal = static_cast<double>(oldValue);
    double newVal = static_cast<double>(newValue);

    double diff = newVal - oldVal;
    sum_ += diff;
    squaredSum_ += diff * (newVal + oldVal);
}

double Accumulator::get_NormStdDev() const
{
    double value = (sum_ == 0.0 ? 0 : (count_ * squaredSum_) / (sum_ * sum_) - 1);

    // to deal with the case when it is negative with very small absolute value
    return value < 0 ? 0 : sqrt(value);
}
