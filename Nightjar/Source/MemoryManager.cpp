//------------------------------------------------------------------------------
//
// File Name:	MemoryManager.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for monitoring and managing memory.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MemoryManager.h"

//#define MEMORY_MANAGER_DISABLE
#define MEMORY_MANAGER_MONITOR
//#define MEMORY_MANAGER_CONTROL

//#define WIN_CRT_HOOK_ENABLE
#pragma region Windows CRT Hooks
#if defined(WIN_CRT_HOOK_ENABLE) && defined(_DEBUG)
#define WIN_CRT_HOOK
#include <windows.h>
#include <tlhelp32.h>
#undef min
#undef max
#endif
#pragma endregion Windows CRT Hooks

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Class Implementation:
    //--------------------------------------------------------------------------

#pragma region Class Implementation

#ifndef MEMORY_MANAGER_DISABLE

    class MemoryManager_
    {
        friend MemoryManager;

        // Public Constants, Enums, and Structs:
    public:
        // Constructors/Destructors:
    private:
        constexpr MemoryManager_(void)
            : init_()
            , totalNewMemory_()
            , totalNewCalls_()
            , totalDeleteMemory_()
            , totalDeleteCalls_()
            , callCounts_(
                Requests("new(size_t), size = {}"),
                Requests("new(size_t, align_val_t), size = {}, align = {}"),
                Requests("new(size_t, nothrow_t), size = {}, nothrow = {}"),
                Requests("new(size_t, align_val_t, nothrow_t), size = {}, align = {}, nothrow = {}"),
                Requests("new[](size_t), size = {}"),
                Requests("new[](size_t, align_val_t), size = {}, align = {}"),
                Requests("new[](size_t, nothrow_t), size = {}, nothrow = {}"),
                Requests("new[](size_t, align_val_t, nothrow_t), size = {}, align = {}, nothrow = {}"),
                Requests("delete(void*)"),
                Requests("delete(void*) throw()"),
                Requests("delete(void*, size_t), size = {}"),
                Requests("delete(void*, align_val_t), align = {}"),
                Requests("delete(void*, size_t, align_val_t), size = {}, align = {}"),
                Requests("delete[](void*)"),
                Requests("delete[](void*) throw()"),
                Requests("delete[](void*, size_t), size = {}"),
                Requests("delete[](void*, align_val_t), align = {}"),
                Requests("delete[](void*, size_t, align_val_t), size = {}, align = {}"))
            , allocations_(nullptr)
            , ptrMap_(nullptr)
            , catMap_(nullptr)
            , ptrMapValid_(false)
            , ptrMutex_()
        {
            assert(&instance == this);
        }
    public:
        MemoryManager_(const MemoryManager_& other) = delete;

        MemoryManager_(MemoryManager_&& other) = delete;

        MemoryManager_& operator=(const MemoryManager_& other) = delete;

        ~MemoryManager_(void)
        {
            SafeDeleteCheck(allocations_);
        }

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        //
        void LogCurrentAllocations(void) const;

        // Public Event Handlers
    public:
        friend void* ::operator new(std::size_t size);
        friend void* ::operator new(std::size_t size, std::align_val_t align);
        friend void* ::operator new(std::size_t size, const std::nothrow_t& tag) noexcept;
        friend void* ::operator new(std::size_t size, std::align_val_t align, const std::nothrow_t& tag) noexcept;
        friend void* ::operator new[](std::size_t size);
        friend void* ::operator new[](std::size_t size, std::align_val_t align);
        friend void* ::operator new[](std::size_t size, const std::nothrow_t& tag) noexcept;
        friend void* ::operator new[](std::size_t size, std::align_val_t align, const std::nothrow_t& tag) noexcept;
        friend void ::operator delete(void* ptr) noexcept;
        friend void ::operator delete(void* ptr) throw();
        friend void ::operator delete(void* ptr, std::size_t size) noexcept;
        friend void ::operator delete(void* ptr, std::align_val_t align) noexcept;
        friend void ::operator delete(void* ptr, std::size_t size, std::align_val_t align) noexcept;
        friend void ::operator delete[](void* ptr) noexcept;
        friend void ::operator delete[](void* ptr) throw();
        friend void ::operator delete[](void* ptr, std::size_t size) noexcept;
        friend void ::operator delete[](void* ptr, std::align_val_t align) noexcept;
        friend void ::operator delete[](void* ptr, std::size_t size, std::align_val_t align) noexcept;

        // Private Constants, Enums, and Structs:
    private:
        //
        enum class RequestType : unsigned char
        {
            New,
            NewAlign,
            NewNothrow,
            NewAlignNothrow,
            NewArr,
            NewArrAlign,
            NewArrNothrow,
            NewArrAlignNothrow,
            Delete,
            DeleteThrow,
            DeleteSize,
            DeleteAlign,
            DeleteSizeAlign,
            DeleteArr,
            DeleteArrThrow,
            DeleteArrSize,
            DeleteArrAlign,
            DeleteArrSizeAlign,
            NUM_ENTRIES,
        };

        //
        struct Requests
        {
            std::atomic_size_t count;
            std::atomic_size_t memory;
            string_view_t format;
            //Requests();
            constexpr Requests(string_view_arg format_)
                : count()
                , memory()
                , format(format_)
            {
            }
            Requests(const Requests& other) = delete;
            Requests(Requests&& other) noexcept = delete;
            Requests& operator=(const Requests& other) = delete;
        };

        struct Allocation
        {
            size_t size;
            size_t category;
            long long timestamp;
        };

        // Allocator Implementation

        struct InternalAllocatorBase
        {
        public:
            static std::atomic_size_t allocatedMemory;
            static std::atomic_size_t deallocatedMemory;
        };

        template <typename T>
        struct InternalAllocator : public InternalAllocatorBase
        {
        public:
            // Reference: https://stackoverflow.com/questions/76525067/how-to-write-a-custom-allocator-for-stdmap-in-c20

            using value_type = T;

            InternalAllocator();

            template <typename TOther>
            InternalAllocator(const InternalAllocator<TOther>&) noexcept;

            T* allocate(std::size_t n);

            void deallocate(T* p, std::size_t n) noexcept;
        };

        typedef std::unordered_map<void*, Allocation, std::hash<void*>, std::equal_to<void*>, InternalAllocator<std::pair<void* const, Allocation>>> PtrMap_t;
        typedef std::unordered_map<size_t, string_view_t, std::hash<size_t>, std::equal_to<size_t>, InternalAllocator<std::pair<size_t const, string_view_t>>> CatMap_t;

        // Private Static Functions:
    private:
        //
        static bool Initialized(void);

        //
        static int AllocHook(int allocType, void* userData, size_t size,
            int blockType, long requestNumber,
            const unsigned char* filename, int lineNumber) noexcept;

        // Private Functions:
    private:
        //
        void* Add(RequestType type, size_t size, size_t category = 0);

        //
        void LogAdd(RequestType type, void* ptr, size_t size, size_t category = 0);

        //
        void Remove(RequestType type, void* ptr, size_t size = 0, size_t category = 0);

        //
        void LogRemove(RequestType type, void* ptr, size_t size, size_t category = 0);

        //
        void* Realloc(void* original, size_t size, size_t category = 0);

        //
        void TrackCall(RequestType type, void* ptr, size_t size = 0, size_t category = 0);

        // Private Static Variables:
    private:
        static MemoryManager_ instance;

        // Private Variables:
    private:
        size_t init_;
        std::atomic_size_t totalNewMemory_;
        std::atomic_size_t totalNewCalls_;
        std::atomic_size_t totalDeleteMemory_;
        std::atomic_size_t totalDeleteCalls_;
        array_t<Requests, (size_t)RequestType::NUM_ENTRIES> callCounts_;
        void* allocations_;
        PtrMap_t* ptrMap_;
        CatMap_t* catMap_;
        bool ptrMapValid_;
        mutable std::mutex ptrMutex_;
    };

#endif

#pragma endregion Class Implementation

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

#ifndef MEMORY_MANAGER_DISABLE

    constinit MemoryManager_ MemoryManager_::instance;

    static constexpr size_t InitValue = 3621;

    #define mmRT Nightjar::MemoryManager_::RequestType
    #define mm Nightjar::MemoryManager_::instance
    #define allocChar reinterpret_cast<char*>(allocations_)

#endif

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Allocator:
    //--------------------------------------------------------------------------

#pragma region Allocator

#ifndef MEMORY_MANAGER_DISABLE

    std::atomic_size_t MemoryManager_::InternalAllocatorBase::allocatedMemory{ 0 };

    std::atomic_size_t MemoryManager_::InternalAllocatorBase::deallocatedMemory{ 0 };

    template<typename T>
    MemoryManager_::InternalAllocator<T>::InternalAllocator()
    {
    }

    template<typename T>
    template<typename TOther>
    MemoryManager_::InternalAllocator<T>::InternalAllocator(const InternalAllocator<TOther>&) noexcept
    {
    }

    template<typename T>
    T* MemoryManager_::InternalAllocator<T>::allocate(size_t size)
    {
        if (size > std::size_t(-1) / sizeof(T))
        {
            throw std::bad_alloc();
        }
        size_t byteSize = size * sizeof(T);
        if (auto ptr = static_cast<T*>(std::malloc(byteSize)))
        {
            allocatedMemory += byteSize;
            return ptr;
        }
        throw std::bad_alloc();
    }

    template<typename T>
    void MemoryManager_::InternalAllocator<T>::deallocate(T* ptr, size_t size) noexcept
    {
        deallocatedMemory += size * sizeof(T);
        std::free(ptr);
    }

    template <class T, class U>
    static bool operator==(const MemoryManager_::InternalAllocator<T>&, const MemoryManager_::InternalAllocator<U>&)
    {
        return true;
    }

    template <class T, class U>
    static bool operator!=(const MemoryManager_::InternalAllocator<T>&, const MemoryManager_::InternalAllocator<U>&)
    {
        return false;
    }

#endif

#pragma endregion Allocator

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    //--------------------------------------------------------------------------

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

#if defined(MEMORY_MANAGER_CONTROL)

#error Memory manager control mode not implemented!

#elif defined(MEMORY_MANAGER_MONITOR)

#ifdef WIN_CRT_HOOK
    static constexpr size_t hookID = 69;

    int MemoryManager_::AllocHook(int allocType, void* userData, size_t size, int blockType, long requestNumber, const unsigned char* filename, int lineNumber) noexcept
    {
        UNREFERENCED_PARAMETER(filename);
        UNREFERENCED_PARAMETER(lineNumber);
        UNREFERENCED_PARAMETER(requestNumber);

        if (blockType == _CRT_BLOCK)
        {
            return TRUE;
        }

        switch (allocType)
        {
        case _HOOK_ALLOC:
            mm.LogAdd(RequestType::New, userData, size, hookID);
            break;
        case _HOOK_REALLOC:
            mm.LogRemove(RequestType::Delete, userData, 0, hookID);
            mm.LogAdd(RequestType::New, userData, size, hookID);
            break;
        case _HOOK_FREE:
            mm.LogRemove(RequestType::Delete, userData, size, hookID);
            break;
        }

        return TRUE;
    }
#endif

    bool MemoryManager::Init()
    {
#ifdef WIN_CRT_HOOK
        _CrtSetAllocHook(MemoryManager_::AllocHook);
#endif

        mm.init_ = InitValue;
        mm.allocations_ = std::calloc(1, 1024);

        if (mm.allocations_ == nullptr)
        {
            mm.init_ = (size_t)-1;
        }

        mm.ptrMap_ = new MemoryManager_::PtrMap_t();
        mm.catMap_ = new MemoryManager_::CatMap_t();
        mm.ptrMapValid_ = mm.ptrMap_ != nullptr;
        if (!mm.ptrMapValid_)
        {
            mm.init_ = (size_t)-1;
        }

        return MemoryManager_::Initialized();
    }

    void MemoryManager::Shutdown()
    {
        StaticManagement::StaticConstantDestroy();
        StaticManagement::StaticDestroy();
#ifdef _DEBUG
        Log();
#endif
        mm.ptrMapValid_ = false;
        SafeDeleteCheck(mm.ptrMap_);
        SafeDeleteCheck(mm.catMap_);
    }

    void MemoryManager::Log()
    {
        mm.LogCurrentAllocations();
    }

    void* MemoryManager::Allocate(size_t size, MemId_t id)
    {
        return mm.Add(mmRT::New, size, id);
    }

    void* MemoryManager::AllocateNoexcept(size_t size, MemId_t id) noexcept
    {
        void* ptr;
        try
        {
            ptr = mm.Add(mmRT::NewNothrow, size, id);
        }
        catch (std::bad_alloc)
        {
            ptr = nullptr;
        }
        return ptr;
    }

    void* MemoryManager::Reallocate(void* original, size_t size, MemId_t id)
    {
        //return original == nullptr ? Allocate(size, id) : mm.Realloc(original, size, id);
        if (original == nullptr)
        {
            return size > 0 ? Allocate(size, id) : nullptr;
        }
        if (size == 0)
        {
            Deallocate(original, id);
            return nullptr;
        }
        return mm.Realloc(original, size, id);
    }

    void* MemoryManager::ReallocateNoexcept(void* original, size_t size, MemId_t id) noexcept
    {
        void* ptr;
        try
        {
            ptr = Reallocate(original, size, id);
        }
        catch (std::bad_alloc)
        {
            ptr = nullptr;
        }
        return ptr;
    }

    void MemoryManager::Deallocate(void* original, MemId_t id) noexcept
    {
        mm.Remove(mmRT::Delete, original, 0, id);
    }

    void MemoryManager::Deallocate(void* original, size_t size, MemId_t id) noexcept
    {
        mm.Remove(mmRT::DeleteSize, original, size, id);
    }

#else

    bool MemoryManager::Init()
    {
        return true;
    }

    void MemoryManager::Shutdown()
    {
    }

    void MemoryManager::Log()
    {
        TraceInfo("Memory manager is disabled.");
    }

    void* MemoryManager::Allocate(size_t size, MemId_t)
    {
        return std::malloc(size);
    }

    void* MemoryManager::AllocateNoexcept(size_t size, MemId_t) noexcept
    {
        return Allocate(size);
    }

    void* MemoryManager::Reallocate(void* original, size_t size, MemId_t)
    {
        return realloc(original, size);
    }

    void* MemoryManager::ReallocateNoexcept(void* original, size_t size, MemId_t) noexcept
    {
        return Reallocate(original, size);
    }

    void MemoryManager::Deallocate(void* original, MemId_t) noexcept
    {
        std::free(original);
    }

    void MemoryManager::Deallocate(void* original, size_t, MemId_t) noexcept
    {
        Deallocate(original);
    }

#endif

    void MemoryManager::Copy(void* destination, size_t destinationSize, const void* source, size_t sourceSize)
    {
        memcpy_s(destination, destinationSize, source, sourceSize);
    }

    void MemoryManager::Copy(void* destination, const void* source, size_t size)
    {
        Copy(destination, size, source, size);
    }

    void MemoryManager::Move(void* destination, size_t destinationSize, const void* source, size_t sourceSize)
    {
        memmove_s(destination, destinationSize, source, sourceSize);
    }

    void MemoryManager::Move(void* destination, const void* source, size_t size)
    {
        Move(destination, size, source, size);
    }

    void MemoryManager::RegisterCategory(MemId_t id, string_view_arg categoryName)
    {
        if (mm.catMap_ != nullptr)
        {
            (*mm.catMap_)[id] = categoryName;
        }
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

#ifndef MEMORY_MANAGER_DISABLE

    void MemoryManager_::LogCurrentAllocations(void) const
    {
        size_t totalSize = 0;

        const size_t
              totalNewCalls = mm.totalNewCalls_
            , totalDeleteCalls = mm.totalDeleteCalls_
            , totalNewMem = mm.totalNewMemory_
            , totalDeleteMem = mm.totalDeleteMemory_
            ;

        // Convert the map to a vector for sorting.
        std::vector<std::pair<void*, Allocation>, InternalAllocator<std::pair<void*, Allocation>>> allocations(mm.ptrMap_->begin(), mm.ptrMap_->end()); //PtrMap_t::value_type
        // Sort by category, then size, then location.
        std::sort(allocations.begin(), allocations.end(), [](const auto& a, const auto& b)
        {
            //return a.second.category < b.second.category;// && a.second.size < b.second.size && a.first < b.first;
            return std::tie(a.second.category, a.second.size, a.first) < std::tie(b.second.category, b.second.size, b.first);
        });
        string_t buffer{};
#define PRINT_BUFFER printf("%s\n", buffer.c_str())
        // Trace each allocation.
        for (const auto& [addr, alloc] : allocations)
        {
            string_view_t catName;
            if (catMap_ != nullptr)
            {
                auto catNameIter = catMap_->find(alloc.category);
                catName = catNameIter == catMap_->end() ? "" : catNameIter->second;
            }
            else
            {
                catName = "";
            }
            if (catName.empty())
            {
                if (Trace::IsActive())
                {
                    TraceWarning("Memory left over at address 0x{:016X}: Category 0x{:016X}, {} bytes.", (size_t)addr, alloc.category, alloc.size);
                }
                else
                {
                    FORMAT_SET(buffer, "Memory left over at address 0x{:016X}: Category 0x{:016X}, {} bytes.\0", (size_t)addr, alloc.category, alloc.size); PRINT_BUFFER;
                }
            }
            else
            {
                if (Trace::IsActive())
                {
                    TraceWarning("Memory left over at address 0x{:016X}: Category \"{}\", {} bytes.", (size_t)addr, catName, alloc.size);
                }
                else
                {
                    FORMAT_SET(buffer, "Memory left over at address 0x{:016X}: Category \"{}\", {} bytes.\0", (size_t)addr, catName, alloc.size); PRINT_BUFFER;
                }
            }
            totalSize += alloc.size;
        }
#define SHOW_CHANGES(name, inc, dec) "New/Delete " name ": {}/{} ({:+d})", (size_t)inc, (size_t)dec, (long long)inc - (long long)dec
        using ptrAlloc = Nightjar::MemoryManager_::PtrMap_t::allocator_type;
        if (Trace::IsActive())
        {
            if (totalSize)
            {
                TraceWarning("Total memory left over: {} bytes.", totalSize);
            }
            TraceInfo(SHOW_CHANGES("calls", totalNewCalls, totalDeleteCalls));
            TraceInfo(SHOW_CHANGES("memory", totalNewMem, totalDeleteMem));
            TraceInfo(SHOW_CHANGES("internal", ptrAlloc::allocatedMemory, ptrAlloc::deallocatedMemory));
        }
        else
        {
            if (totalSize)
            {
                FORMAT_SET(buffer, "Total memory left over: {} bytes.", totalSize); PRINT_BUFFER;
            }
            FORMAT_SET(buffer, SHOW_CHANGES("calls", totalNewCalls, totalDeleteCalls)); PRINT_BUFFER;
            FORMAT_SET(buffer, SHOW_CHANGES("memory", totalNewMem, totalDeleteMem)); PRINT_BUFFER;
            FORMAT_SET(buffer, SHOW_CHANGES("internal", ptrAlloc::allocatedMemory, ptrAlloc::deallocatedMemory)); PRINT_BUFFER;
        }
#undef SHOW_CHANGES
    }

#endif

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#ifndef MEMORY_MANAGER_DISABLE

    bool MemoryManager_::Initialized(void)
    {
        return mm.init_ == InitValue;
    }

    void* MemoryManager_::Add(RequestType type, size_t size, size_t category)
    {
        assert(Initialized());
        //++totalNewCalls_;
        if (size == 0)
        {
            ++size; // avoid std::malloc(0) which may return nullptr on success
        }

        if (void* ptr = std::malloc(size))
        {
            LogAdd(type, ptr, size, category);
            return ptr;
        }

        throw std::bad_alloc{}; // required by [new.delete.single]/3
    }

    void MemoryManager_::LogAdd(RequestType type, void* ptr, size_t size, size_t category)
    {
        TrackCall(type, ptr, size, category);
        //++totalNewCalls_;
        if (size == 0)
        {
            return;
        }
        if (ptrMapValid_)
        {
#ifdef WIN_CRT_HOOK
            if (category != hookID)
#endif
            {
                ++totalNewCalls_;
                totalNewMemory_ += size;
                std::lock_guard<std::mutex> lock(ptrMutex_);
                ptrMap_->emplace(PtrMap_t::value_type{ PtrMap_t::key_type{ ptr }, PtrMap_t::mapped_type{ size, category, Stopwatch::Now().time_since_epoch().count() } });
            }
        }
    }

    void MemoryManager_::Remove(RequestType type, void* ptr, size_t size, size_t category)
    {
        assert(Initialized());
        //++totalDeleteCalls_;
        LogRemove(type, ptr, size, category);
        std::free(ptr);
    }

    void MemoryManager_::LogRemove(RequestType type, void* ptr, size_t size, size_t category)
    {
        TrackCall(type, ptr, size, category);
        //++totalDeleteCalls_;
        if (ptr != nullptr && ptrMapValid_) [[likely]]
        {
#ifdef WIN_CRT_HOOK
            if (category != hookID)
#endif
            {
                ++totalDeleteCalls_;
                std::lock_guard<std::mutex> lock(ptrMutex_);
                const Allocation& sizeOrig = ptrMap_->at(ptr);
                assert(size == 0 || size == sizeOrig.size);
                totalDeleteMemory_ += sizeOrig.size;
                ptrMap_->erase(ptr);
            }
        }
        else
        {
            //totalDeleteMemory_ += size;
        }
    }

    void* MemoryManager_::Realloc(void* original, size_t size, size_t category)
    {
        const Allocation& oldAlloc = ptrMap_->at(original);
        if (oldAlloc.size == size) [[unlikely]]
        {
            return original;
        }
        size_t copySize = std::min(size, oldAlloc.size);
        void* ptr = Add(RequestType::New, size, category);
        MemoryManager::Copy(ptr, original, copySize);
        Remove(RequestType::DeleteSize, original, oldAlloc.size, category);
        return ptr;
    }

    void MemoryManager_::TrackCall(RequestType type, void*, size_t size, size_t)
    {
        assert(Initialized());
        auto& req = callCounts_[(unsigned char)type];
        ++req.count;
        req.memory += size;
    }

#endif

#pragma endregion Private Functions

} // namespace

//------------------------------------------------------------------------------
// Global new/delete:
//------------------------------------------------------------------------------

#pragma region Global new/delete

#ifndef MEMORY_MANAGER_DISABLE

// Reference: https://en.cppreference.com/w/cpp/memory/new/operator_new.html
// Reference: https://en.cppreference.com/w/cpp/memory/new/operator_delete.html

void* ::operator new(std::size_t size)
{
    return mm.Add(mmRT::New, size);
}

void* operator new(std::size_t size, std::align_val_t)
{
    return mm.Add(mmRT::NewAlign, size);
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    return mm.Add(mmRT::NewNothrow, size);
}

void* operator new(std::size_t size, std::align_val_t, const std::nothrow_t&) noexcept
{
    return mm.Add(mmRT::NewAlignNothrow, size);
}

void* operator new[](std::size_t size)
{
    return mm.Add(mmRT::NewArr, size);
}

void* operator new[](std::size_t size, std::align_val_t)
{
    return mm.Add(mmRT::NewArrAlign, size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    return mm.Add(mmRT::NewArrNothrow, size);
}

void* operator new[](std::size_t size, std::align_val_t, const std::nothrow_t&) noexcept
{
    return mm.Add(mmRT::NewArrAlignNothrow, size);
}

void operator delete(void* ptr) noexcept
{
    mm.Remove(mmRT::Delete, ptr);
}

/*void operator delete(void* ptr) throw()
{
    mm.Remove(mmRT::DeleteThrow, ptr);
}*/

void operator delete(void* ptr, std::size_t size) noexcept
{
    mm.Remove(mmRT::DeleteSize, ptr, size);
}

void operator delete(void* ptr, std::align_val_t) noexcept
{
    mm.Remove(mmRT::DeleteAlign, ptr);
}

void operator delete(void* ptr, std::size_t size, std::align_val_t) noexcept
{
    mm.Remove(mmRT::DeleteSizeAlign, ptr, size);
}

void operator delete[](void* ptr) noexcept
{
    mm.Remove(mmRT::DeleteArr, ptr);
}

/*void operator delete[](void* ptr) throw()
{
    mm.Remove(mmRT::DeleteArrThrow, ptr);
}*/

void operator delete[](void* ptr, std::size_t size) noexcept
{
    mm.Remove(mmRT::DeleteArrSize, ptr, size);
}

void operator delete[](void* ptr, std::align_val_t) noexcept
{
    mm.Remove(mmRT::DeleteArrAlign, ptr);
}

void operator delete[](void* ptr, std::size_t size, std::align_val_t) noexcept
{
    mm.Remove(mmRT::DeleteArrSizeAlign, ptr, size);
}

#endif

#pragma endregion Global new/delete
