//------------------------------------------------------------------------------
//
// File Name:	PerformanceTracker.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Struct that represents timing data of some given object and
//              computes statistics of that data.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PerformanceTracker.h"
#include "PerformanceSystem.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4583)

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Public Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    PerformanceTracker::PerformanceTracker(void)
        : name()
        , times(4)
        , timeCapacity(4)
        , has_cpuUpdate(true)
        , has_gfxUpdate(true)
        , has_cpuRender(true)
        , has_gfxRender(true)
        , step(4)
        , statsAll()
        , object()
        , parent()
        , children()
        , gfxTimer()
    {
    }

    PerformanceTracker::PerformanceTracker(bool multi)
        : name()
        , times(multi ? 4 : 1)
        , timeCapacity(multi ? 4 : 1)
        , has_cpuUpdate(multi)
        , has_gfxUpdate(multi)
        , has_cpuRender(multi)
        , has_gfxRender(multi)
        , step(multi ? 4 : 1)
        , statsAll()
        , object()
        , parent()
        , children()
        , gfxTimer()
    {
    }

    PerformanceTracker::PerformanceTracker(const Object& obj, string_view_arg name_, const Object& parent_)
        : name(name_)
        , times(4)
        , timeCapacity(4)
        , has_cpuUpdate(true)
        , has_gfxUpdate(true)
        , has_cpuRender(true)
        , has_gfxRender(true)
        , step(4)
        , statsAll()
        , object(obj.GetGUID())
        , parent(parent_.GetGUID())
        , children()
        , gfxTimer()
    {
        if (object != parent)
        {
            auto& tracker = PerformanceSystem::GetTracker(parent_);
            tracker.children.push_back(object);
        }
        gfxTimer.Initialize();
    }

    PerformanceTracker::PerformanceTracker(PerformanceTracker&& other) noexcept
        : name(std::move(other.name))
        , times(std::move(other.times))
        , timeCapacity(std::move(other.timeCapacity))
        , has_cpuUpdate(other.has_cpuUpdate)
        , has_gfxUpdate(other.has_gfxUpdate)
        , has_cpuRender(other.has_cpuRender)
        , has_gfxRender(other.has_gfxRender)
        , step(std::move(other.step))
        , statsAll(std::move(other.statsAll))
        , object(std::move(other.object))
        , parent(std::move(other.parent))
        , children(std::move(other.children))
        , gfxTimer(std::move(other.gfxTimer))
    {
    }

    PerformanceTracker& PerformanceTracker::operator=(PerformanceTracker&& other) noexcept
    {
        if (&other != this)
        {
            name = std::move(other.name);
            times = std::move(other.times);
            timeCapacity = std::move(other.timeCapacity);
            has_cpuUpdate = other.has_cpuUpdate;
            has_gfxUpdate = other.has_gfxUpdate;
            has_cpuRender = other.has_cpuRender;
            has_gfxRender = other.has_gfxRender;
            step = std::move(other.step);
            statsAll = std::move(other.statsAll);
            object = std::move(other.object);
            parent = std::move(other.parent);
            children = std::move(other.children);
            gfxTimer = std::move(other.gfxTimer);
        }

        return *this;
    }

    //--------------------------------------------------------------------------

    PerformanceTracker::~PerformanceTracker(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    nsec_t& PerformanceTracker::operator[](size_t index)
    {
        return times[index];
    }

    nsec_t PerformanceTracker::operator[](size_t index) const
    {
        return times[index];
    }

    nsec_t& PerformanceTracker::Front(void)
    {
        return times.front();
    }

    nsec_t PerformanceTracker::FrontOrDefault(void) const
    {
        return times.empty() ? nsec_t() : times.front();
    }

    size_t PerformanceTracker::Size(void) const
    {
        return times.size();
    }

    bool PerformanceTracker::Empty(void) const
    {
        return times.empty();
    }

    void PerformanceTracker::SetCapacity(size_t capacity)
    {
        timeCapacity = static_cast<decltype(timeCapacity)>(capacity) * step;
        auto oldSize = times.size();
        times.resize(timeCapacity);
        if (oldSize < timeCapacity)
        {
            times.resize(oldSize);
        }
    }

    void PerformanceTracker::Push(nsec_t time)
    {
        if (times.size() >= timeCapacity)
        {
            times.pop_back();
        }
        times.push_front(time);
    }

    void PerformanceTracker::Next(void)
    {
        if (times.size() >= timeCapacity)
        {
            for (unsigned i = 0; i < step; ++i)
            {
                times.pop_back();
                times.emplace_front();
            }
        }
        else
        {
            for (unsigned i = 0; i < step; ++i)
            {
                times.emplace_front();
            }
        }
    }

    void PerformanceTracker::Push(nsec_t time, EngineLoop engineLoop, PerformanceSource source)
    {
        times[GetOffset(engineLoop, source)] = time;
    }

    void PerformanceTracker::CalculateSingle(bool quartiles)
    {
        if (!times.empty())
        {
            stats.Calculate(times.begin(), times.end(), quartiles);
        }
    }

    void PerformanceTracker::CalculateAll(bool quartiles)
    {
        if (times.empty())
        {
            return;
        }
        stats_t::CalculateInterleaved(times.begin(), times.end(), statsAll.data(), statsAll.size());
        if (quartiles)
        {
            unsigned char index = 0;
            for (unsigned engineLoop = 0; engineLoop < static_cast<unsigned>(EngineLoop::NUM_ENTRIES); ++engineLoop)
            {
                for (unsigned source = 0; source < static_cast<unsigned>(PerformanceSource::NUM_ENTRIES); ++source, ++index)
                {
                    auto end = PackedIterator(times.end(), step, times.end());
                    auto begin = PackedIterator(times.begin() + index, step, times.end());
                    statsAll[index].Calculate(begin, end, quartiles);
                }
            }
        }
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

    unsigned PerformanceTracker::GetOffset(EngineLoop engineLoop, PerformanceSource source)
    {
        unsigned sourceOffset = static_cast<unsigned>(source);
        unsigned engineLoopOffset = static_cast<unsigned>(engineLoop) * static_cast<unsigned>(PerformanceSource::NUM_ENTRIES);
        return sourceOffset + engineLoopOffset;
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

    //--------------------------------------------------------------------------
    // Iterator:
    //--------------------------------------------------------------------------

#pragma region Iterator

    PerformanceTracker::PackedIterator::PackedIterator(const baseIter_t& iter, unsigned char step_, const baseIter_t& iterEnd)
        : baseIter_t(iter)
        , step(step_)
        , end(iterEnd)
    {
    }

    PerformanceTracker::PackedIterator& PerformanceTracker::PackedIterator::operator++(void) noexcept
    {
        for (unsigned i = 0; i < step && !(*this == end); ++i)
        {
            baseIter_t::operator++();
        }
        return *this;
    }

    PerformanceTracker::PackedIterator PerformanceTracker::PackedIterator::operator++(int) noexcept
    {
        PackedIterator tmp = *this;
        PackedIterator::operator++();
        return tmp;
    }

    PerformanceTracker::PackedIterator& PerformanceTracker::PackedIterator::operator+=(const difference_type offset) noexcept
    {
        const difference_type total = step * offset;
        if (total > 0)
        {
            for (difference_type i = 0; i < total && !(*this == end); ++i)
            {
                baseIter_t::operator++();
            }
        }
        else
        {
            for (difference_type i = 0; i > total && !(*this == end); --i)
            {
                baseIter_t::operator--();
            }
        }
        return *this;
    }

    PerformanceTracker::PackedIterator PerformanceTracker::PackedIterator::operator+(const difference_type offset) const noexcept
    {
        return PackedIterator(baseIter_t::operator+(step * offset), step, end);
    }

    PerformanceTracker::PackedIterator& PerformanceTracker::PackedIterator::operator--(void) noexcept
    {
        baseIter_t::operator-=(step);
        return *this;
    }

    PerformanceTracker::PackedIterator PerformanceTracker::PackedIterator::operator--(int) noexcept
    {
        PackedIterator tmp = *this;
        baseIter_t::operator-=(step);
        return tmp;
    }

    PerformanceTracker::PackedIterator& PerformanceTracker::PackedIterator::operator-=(const difference_type offset) noexcept
    {
        baseIter_t::operator-=(step * offset);
        return *this;
    }

    /*PerformanceTracker::PackedIterator PerformanceTracker::PackedIterator::operator-(const difference_type offset) const noexcept
    {
        return PackedIterator(baseIter_t::operator-(step * offset), step, end);
    }*/

    bool PerformanceTracker::PackedIterator::operator==(const PackedIterator& other) const noexcept
    {
        return baseIter_t::operator==(other);
    }

    PerformanceTracker::PackedIterator::difference_type PerformanceTracker::PackedIterator::operator-(const PackedIterator& other) const noexcept
    {
        return (((static_cast<baseIter_t>(*this) - (step - 1)) - other) / step) + 1;
    }

#pragma endregion Iterator

}	// namespace

#pragma warning(pop)
