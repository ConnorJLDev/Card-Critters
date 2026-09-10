//------------------------------------------------------------------------------
//
// File Name:	TextSystem.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that handles TrueType text.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Texture.h"
#include "Font.h"
#include "TextSystem.h"
#include "BaseSystem.h"

#include TEXT_INCLUDE

#include FT_SYSTEM_H
#include FT_MODULE_H

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

    class TextSystem_ SYSTEM_FINAL : public BaseSystem
    {
        friend TextSystem;

        SYSTEM_COMMON(TextSystem_);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        TextSystem_(void);

        // All systems need a virtual destructor to have their destructor called 
        ~TextSystem_(void) override;

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:
        // Test func
        static void TestFunc(const string_t& fontName, const string_t& text);

        //
        static void* AllocatorAllocate(FT_Memory mm, long size) noexcept;

        //
        static void* AllocatorReallocate(FT_Memory mm, long cur_size, long new_size, void* block) noexcept;

        //
        static void AllocatorDeallocate(FT_Memory mm, void* block) noexcept;

        // Private Functions:
    private:
        /// <summary>
        /// Initializes the system.
        /// </summary>
        /// <returns>True if initialization succeeded, false otherwise.</returns>
        bool Init(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        // Private Static Variables:
    private:
        static TextSystem_* instance;

        static constexpr size_t memId_freetype = MEM_ID("FREETYPE");

        // Private Variables:
    private:
        FT_Library library_;
        FT_MemoryRec_ memoryManager_;
        Texture* textureTest_;
    };

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
    // Private Static Variables:
    //--------------------------------------------------------------------------

    TextSystem_* TextSystem_::instance = nullptr;

#define inst TextSystem_::instance

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    TextSystem_::TextSystem_(void)
        : BaseSystem("TextSystem", LoopFlags::None)
        , library_(nullptr)
        , textureTest_(nullptr)
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    TextSystem_::~TextSystem_(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* TextSystem::GetInstance(void)
    {
        return inst != nullptr ? inst : new TextSystem_();
    }

    FT_Library TextSystem::GetLibrary(void)
    {
        return inst->library_;
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

    void TextSystem_::TestFunc(const string_t& fontName, const string_t& text)
    {
        const Font* font = FontLibrary::Get_S(fontName);
        if (font == nullptr)
        {
            TraceError("Failed to test font {}", fontName);
            return;
        }

        font->SetSize(12, 100);

        IVec2 size{ 79, 14 };
        Mat2 mtx{ 0.75f, 0.25f, 0.0f, 1.0f };
        Vec2 pos{ static_cast<float>(size.x) / 2.0f, static_cast<float>(size.y) / 2.0f };
        const unsigned char* image = font->Draw(text, size, mtx, pos);

        // Image drawing code referenced from https://freetype.org/freetype2/docs/tutorial/example1.c
        for (int i = 0; i < size.y; i++)
        {
            for (int j = 0; j < size.x; j++)
            {
                putchar(image[j + i * size.x] == 0 ? ' '
                    : image[j + i * size.x] < 128 ? '+'
                    : '*');
            }
            putchar('\n');
        }

        delete[] image;
    }

    void* TextSystem_::AllocatorAllocate(FT_Memory, long size) noexcept
    {
        return MemoryManager::AllocateNoexcept((unsigned long)size, memId_freetype);
    }

    void* TextSystem_::AllocatorReallocate(FT_Memory, long, long new_size, void* block) noexcept
    {
        return MemoryManager::ReallocateNoexcept(block, (unsigned long)new_size, memId_freetype);
    }

    void TextSystem_::AllocatorDeallocate(FT_Memory, void* block) noexcept
    {
        MemoryManager::Deallocate(block, memId_freetype);
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    bool TextSystem_::Init(void)
    {
        memoryManager_.user = nullptr;
        memoryManager_.alloc = AllocatorAllocate;
        memoryManager_.realloc = AllocatorReallocate;
        memoryManager_.free = AllocatorDeallocate;

        FontLibrary::Init_S();

        auto error = FT_New_Library(&memoryManager_, &library_);

        const bool success{ error == FT_Err_Ok };
        if (!success)
        {
            TraceError("Error initializing FreeType library: 0x{:X}", error);
            return false;
        }

        FT_Add_Default_Modules(library_);
        TestFunc("Kenney/Kenney Future.ttf", "NIGHTJAR");

        // Return true if the initialization completed successfully.
        return true;
    }

    void TextSystem_::Exit(void)
    {
        FontLibrary::Shutdown_S();

        auto error = FT_Done_Library(library_);

        if (error != FT_Err_Ok)
        {
            TraceError("Error shutting down FreeType library: 0x{:X}", error);
        }
    }

#pragma endregion Private Functions

}	// namespace
