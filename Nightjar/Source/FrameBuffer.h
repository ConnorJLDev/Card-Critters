//------------------------------------------------------------------------------
//
// File Name:	FrameBuffer.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for encapsulating a render target.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsSystem.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
    class RenderBuffer;

	// Typedefs:

	// Class Definition:
	class FrameBuffer : public NamedObject
	{
        friend RenderBuffer;

		// Public Constants, Enums, and Structs:
	public:
        //
        enum class OutputType : unsigned char
        {
            Invalid = (unsigned char)-1,
            Color = 0,
            Depth,
            Stencil,
            DepthStencil,
            NUM_ENTRIES,
        };

        //
        typedef uint8_t RenderTargetIndex_t;

        //
        static constexpr RenderTargetIndex_t RenderTargetIndexInvalid = (RenderTargetIndex_t)-1;

		// Constructors/Destructors:
	public:
		FrameBuffer();

		~FrameBuffer(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
        /// <summary>
        /// Activates and updates the frame buffer.
        /// </summary>
        void Activate();

        /// <summary>
        /// Gets the dimensions of the frame buffer.
        /// </summary>
        /// <returns>The dimensions of the frame buffer in pixels.</returns>
        const UVec2& GetSize(void) const;

        /// <summary>
        /// Sets the dimensions of the frame buffer. Resizes associated render buffers and textures.
        /// </summary>
        /// <param name="size">The new size to set the frame buffer to.</param>
        void SetSize(const UVec2& size);

        /// <summary>
        /// Attempts to add a render target.
        /// </summary>
        /// <param name="type">The output data type.</param>
        /// <param name="texture">The texture to target, or nullptr to create a render buffer.</param>
        /// <param name="index">The target index to assign to. Only used for color outputs.</param>
        /// <returns>True if the target was successfully added, false otherwise.</returns>
        bool AddRenderTarget(OutputType type, Texture* texture = nullptr, RenderTargetIndex_t index = RenderTargetIndexInvalid);

        /// <summary>
        /// Checks if the target has an output assigned.
        /// </summary>
        /// <param name="type">The data type to check.</param>
        /// <param name="index">The target index to check. Only used for color outputs.</param>
        /// <returns>True if the target has an output assigned, false otherwise.</returns>
        bool GetRenderTargetAssigned(OutputType type, RenderTargetIndex_t index = RenderTargetIndexInvalid) const;

        /// <summary>
        /// Gets the texture associated with the target.
        /// </summary>
        /// <param name="type">The data type to check.</param>
        /// <param name="index">The target index to check. Only used for color outputs.</param>
        /// <returns>A pointer to the texture used by that target, or nullptr if the target is not assigned or the target is not a texture.</returns>
        Texture* GetRenderTargetTexture(OutputType type, RenderTargetIndex_t index = RenderTargetIndexInvalid) const;

        /// <summary>
        /// Gets the render buffer associated with the target.
        /// </summary>
        /// <param name="type">The data type to check.</param>
        /// <param name="index">The target index to check. Only used for color outputs.</param>
        /// <returns>A pointer to the render buffer used by that target, or nullptr if the target is not assigned or the target is not a render buffer.</returns>
        RenderBuffer* GetRenderTargetBuffer(OutputType type, RenderTargetIndex_t index = RenderTargetIndexInvalid) const;

        /// <summary>
        /// Removes the render target with the specified type.
        /// </summary>
        /// <param name="type">The data type to remove.</param>
        /// <param name="index">The target index to remove. Only used for color outputs.</param>
        /// <returns>True if the target was found and removed, or false if the target was not found.</returns>
        bool RemoveRenderTarget(OutputType type, RenderTargetIndex_t index = RenderTargetIndexInvalid);

        /// <summary>
        /// Removes all render targets.
        /// </summary>
        void ClearRenderTargets(void);

        /// <summary>
        /// Gets the number of render targets set.
        /// </summary>
        /// <returns>The number of assigned render targets.</returns>
        RenderTargetIndex_t GetRenderTargetCount(void) const;

        /// <summary>
        /// Gets the index of the render target with the highest index.
        /// </summary>
        /// <returns>The highest render target index assigned.</returns>
        RenderTargetIndex_t GetRenderTargetMax(void) const;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
        // Struct describing properties of a render target.
        struct RenderTarget
        {
            FrameBuffer& renderer;
            // Framebuffer can either output to raw renderbuffer OR to a texture, but not both.
            union
            {
                Texture* texture;
                RenderBuffer* buffer;
            };
            bool isTexture;
            RenderTargetIndex_t index;
            OutputType type;
            ResourceEnum target;
            RenderTarget(FrameBuffer& renderer, Texture* texture);
            RenderTarget(FrameBuffer& renderer, RenderBuffer* buffer);
            RenderTarget(const RenderTarget& other) = delete;
            RenderTarget(RenderTarget&& other) noexcept;
            RenderTarget& operator=(const RenderTarget& other);
            ~RenderTarget();

            //
            void UpdateSize(void) const;
        };

        typedef vector_t<RenderTarget> RenderTargetContainer_t;
        typedef RenderTargetContainer_t::iterator RenderTargetContainerIter_t;
        typedef RenderTargetContainer_t::const_iterator RenderTargetContainerConstIter_t;

		// Private Static Functions:
	private:
        /// <summary>
        /// Unbinds the frame buffer for reading and writing/drawing.
        /// </summary>
        static void UnbindBuffers();

		// Private Functions:
	private:
        /// <summary>
        /// Updates the internal render buffer used for depth and/or stencil when those components are requested but not assigned a render target.
        /// </summary>
        void UpdateInternalRenderBuffer();

        /// <summary>
        /// Tries to find the render target with the given type and index.
        /// </summary>
        /// <param name="type">The data type to find.</param>
        /// <param name="index">The target index to find. Only used for color outputs.</param>
        /// <returns>The iterator of the target if found, or the end iterator if not found.</returns>
        RenderTargetContainerIter_t FindRenderTarget(OutputType type, RenderTargetIndex_t index);

        /// <summary>
        /// Tries to find the render target with the given type and index.
        /// </summary>
        /// <param name="type">The data type to find.</param>
        /// <param name="index">The target index to find. Only used for color outputs.</param>
        /// <returns>The iterator of the target if found, or the end iterator if not found.</returns>
        RenderTargetContainerConstIter_t FindRenderTargetConst(OutputType type, RenderTargetIndex_t index) const;

        /// <summary>
        /// Creates the graphics data for this frame buffer.
        /// </summary>
        void CreateBuffers();

        /// <summary>
        /// Destroys the graphics data for this frame buffer.
        /// </summary>
        /// <returns>True if the graphics data existed and was destroyed, false otherwise.</returns>
        bool DestroyBuffers();

        /// <summary>
        /// Binds the frame buffer for reading and writing/drawing.
        /// </summary>
        void BindBuffers() const;

        /// <summary>
        /// Binds the frame buffer for reading.
        /// </summary>
        void BindBuffersRead() const;

        /// <summary>
        /// Binds the frame buffer for writing/drawing.
        /// </summary>
        void BindBuffersWrite() const;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
        // The frame buffer.
        ResourceID framebufferID_;
        // A non-output render buffer if depth and/or stencil is requested.
        RenderBuffer* renderbuffer_;
        // The dimensions of the frame buffer.
        UVec2 size_;
        // Flag for if depth processing occurs in this frame buffer.
        bool depthEnable_;
        // Flag for if stencil processing occurs in this frame buffer.
        bool stencilEnable_;
        // Flag for if the targets have been modified since the frame buffer was last activated.
        bool targetsDirty_;
        // The render targets.
        vector_t<RenderTarget> targets_;
        // The render target attachment points.
        vector_t<ResourceEnum> targetAttachments_;
	};

}	// namespace
