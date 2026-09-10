//------------------------------------------------------------------------------
//
// File Name:	GraphicsBuffer.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that handles data transfers between local/host memory
//              and external interfaces.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BufferDescriptor.h"
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

	// Typedefs:

    /// <summary>
    /// Type restriction for getting or setting data from a buffer.
    /// Cannot get/set a pointer value as it loses meaning when transferred,
    /// and thus becomes garbage. Note that is for "values that are pointers",
    /// not "pointers to values".
    /// The void restriction is to prevent void pointer usage in template
    /// functions.
    /// </summary>
    template<typename T>
    concept BufferValue = requires(T)
    {
        !(std::is_pointer_v<T> || std::is_void_v<T>);
    };

	// Class Definition:
	class GraphicsBuffer : public Object
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
        GraphicsBuffer(const BufferDescriptor& descriptor, size_t arraySize = 1);
        GraphicsBuffer(const GraphicsBuffer& other);
        GraphicsBuffer(GraphicsBuffer&& other) noexcept;
        GraphicsBuffer(GraphicsBuffer&& other, const BufferDescriptor& descriptor) noexcept;
        GraphicsBuffer& operator=(const GraphicsBuffer& other) noexcept;

		~GraphicsBuffer(void);

		// Public Static Functions:
	public:
        /// <summary>
        /// Sets up the given buffers so they are ready to copy data.
        /// </summary>
        /// <param name="destination">The buffer to write to.</param>
        /// <param name="source">The buffer to read from.</param>
        static void CopyActivate(GraphicsBuffer& destination, const GraphicsBuffer& source);

        /// <summary>
        /// Shuts down data transfer between the given buffers.
        /// </summary>
        /// <param name="destination">The buffer that was written to.</param>
        /// <param name="source">The buffer that was read from.</param>
        static void CopyDeactivate(GraphicsBuffer& destination, const GraphicsBuffer& source);

        /// <summary>
        /// Copies data between buffers.
        /// </summary>
        /// <param name="destination">The buffer to write to.</param>
        /// <param name="destinationOffset">The offset into the destination buffer to start writing.</param>
        /// <param name="source">The buffer to read from.</param>
        /// <param name="sourceOffset">The offset into the source buffer to start reading.</param>
        /// <param name="size">Amount of data to copy, in bytes.</param>
        static void Copy(GraphicsBuffer& destination, size_t destinationOffset, const GraphicsBuffer& source, size_t sourceOffset, size_t size);

		// Public Functions:
	public:
        /// <summary>
        /// Gets the descriptor used for defining blocks in this buffer.
        /// </summary>
        /// <returns>The descriptor used in this buffer.</returns>
        const BufferDescriptor& GetDescriptor(void) const;

        /// <summary>
        /// Gets the size of this buffer.
        /// </summary>
        /// <returns>The size of this buffer, in bytes.</returns>
        size_t Size(void) const;

        /// <summary>
        /// Gets the capacity of this buffer (how much data can be stored without reallocating).
        /// </summary>
        /// <returns>The capacity of this buffer, in bytes.</returns>
        size_t Capacity(void) const;

        /// <summary>
        /// Gets the size of a descriptor block in this buffer. Same as descriptor size.
        /// </summary>
        /// <returns>The size of a block, in bytes.</returns>
        size_t BlockSize(void) const;

        /// <summary>
        /// Gets the number of blocks this buffer contains.
        /// </summary>
        /// <returns>The number of blocks contained in this buffer.</returns>
        size_t BlockCount(void) const;

        /// <summary>
        /// Gets the maximum number of blocks this buffer can contain without needing to reallocate memory.
        /// </summary>
        /// <returns>The maximum number of blocks this buffer can contain.</returns>
        size_t BlockCapacity(void) const;

        /// <summary>
        /// Checks if the buffer interface is initialized and able to be used.
        /// </summary>
        /// <returns>True if the buffer is ready, false otherwise.</returns>
        bool GetInitialized(void) const;

        /// <summary>
        /// Initializes the buffer interface.
        /// </summary>
        void Initialize(void);

        /// <summary>
        /// Creates the Graphics API buffer. If the local buffer exists, the Graphics API buffer's memory is initialized to that of the local buffer.
        /// If the buffer already existed, it is destroyed and replaced.
        /// </summary>
        void CreateBuffer(void);

        /// <summary>
        /// Destroys the Graphics API buffer.
        /// </summary>
        /// <returns>True if the buffer existed, false otherwise.</returns>
        bool DestroyBuffer(void);

        /// <summary>
        /// Checks if there is a local buffer being used.
        /// </summary>
        /// <returns>True if the local buffer exists, false otherwise.</returns>
        bool GetUsingLocalCopy(void) const;

        /// <summary>
        /// Creates or destroys the local buffer.
        /// </summary>
        /// <param name="useLocalCopy">True to create the local buffer, false to destroy the local buffer.</param>
        void SetUsingLocalCopy(bool useLocalCopy);

        /// <summary>
        /// Sets up the buffer interface for reading and/or writing.
        /// </summary>
        void Activate(void);

        /// <summary>
        /// Shuts down the buffer interface after reading and/or writing.
        /// </summary>
        void Deactivate(void);

        /// <summary>
        /// Resizes the buffer to contain the requested number of blocks.
        /// </summary>
        /// <param name="count">The number of blocks to contain.</param>
        /// <param name="preserveData">Only if memory is reallocated: True to copy data from the old buffer to the new resized buffer, false to leave memory uninitialized.</param>
        void Resize(size_t count, bool preserveData = true);

        /// <summary>
        /// Ensures that enough memory is allocated to contain the requested number of blocks, and reallocates if necessary.
        /// </summary>
        /// <param name="count">The number of blocks to be able to contain.</param>
        /// <param name="preserveData">Only if memory is reallocated: True to copy data from the old buffer to the new resized buffer, false to leave memory uninitialized.</param>
        void Reserve(size_t count, bool preserveData = true);

        /// <summary>
        /// Reallocates both local and graphics API buffers to the specified number of blocks, only if they already exist.
        /// </summary>
        /// <param name="count">The new block count.</param>
        /// <param name="preserveData">True to copy data from the old buffer to the new resized buffer, false to leave memory uninitialized.</param>
        void ResizeForce(size_t count, bool preserveData = true);

#pragma region Local Send/Fetch

        /// <summary>
        /// Sends the local buffer to the Graphics API buffer.
        /// </summary>
        void LocalSend(void);

        /// <summary>
        /// Sends a section of the local buffer to the Graphics API buffer.
        /// </summary>
        /// <param name="offset">The offset of the start of the section, in bytes.</param>
        /// <param name="size">The size of the section, in bytes.</param>
        void LocalSend(size_t offset, size_t size);

        /// <summary>
        /// Sends one or more descriptor blocks of the local buffer to the Graphics API buffer.
        /// </summary>
        /// <param name="index">The starting index of the blocks to send.</param>
        /// <param name="count">The number of blocks to send.</param>
        void LocalSendBlocks(size_t index, size_t count = 1);

        /// <summary>
        /// Fetches the Graphics API buffer into the local buffer.
        /// </summary>
        void LocalFetch(void);

        /// <summary>
        /// Fetches a section from the Graphics API buffer into the local buffer.
        /// </summary>
        /// <param name="offset">The offset of the start of the section, in bytes.</param>
        /// <param name="size">The size of the section, in bytes.</param>
        void LocalFetch(size_t offset, size_t size);

        /// <summary>
        /// Fetches one or more descriptor blocks from the Graphics API buffer into the local buffer.
        /// </summary>
        /// <param name="index">The starting index of the blocks to fetch.</param>
        /// <param name="count">The number of blocks to fetch.</param>
        void LocalFetchBlocks(size_t index, size_t count = 1);

#pragma endregion Local Send/Fetch

#pragma region GetLocalBuffer

        /// <summary>
        /// Gets a const pointer to the local memory buffer, if it exists.
        /// </summary>
        /// <param name="offset">Optional offset into the memory buffer to get.</param>
        /// <returns>A const pointer to the local memory buffer with optional offset.</returns>
        const void* GetLocalBuffer(size_t offset = 0) const;

        /// <summary>
        /// Gets a pointer to the local memory buffer, if it exists.
        /// </summary>
        /// <param name="offset">Optional offset into the memory buffer to get.</param>
        /// <returns>A pointer to the local memory buffer with optional offset.</returns>
        void* GetLocalBuffer(size_t offset = 0);

        /// <summary>
        /// Gets a const pointer to a block in the local memory buffer, if it exists.
        /// </summary>
        /// <param name="index">Index of the block to get.</param>
        /// <param name="offset">Optional offset into the block to get.</param>
        /// <returns>A const pointer to the block with optional offset.</returns>
        const void* GetLocalBufferBlock(size_t index, size_t offset = 0) const;

        /// <summary>
        /// Gets a pointer to a block in the local memory buffer, if it exists.
        /// </summary>
        /// <param name="index">Index of the block to get.</param>
        /// <param name="offset">Optional offset into the block to get.</param>
        /// <returns>A pointer to the block with optional offset.</returns>
        void* GetLocalBufferBlock(size_t index, size_t offset = 0);

#pragma endregion GetLocalBuffer

#pragma region GetOffset

        /// <summary>
        /// Gets the offset of the property with the given name.
        /// </summary>
        /// <param name="name">The name of the property to get.</param>
        /// <returns>The offset of the named property.</returns>
        size_t GetOffset(string_view_arg name);

        /// <summary>
        /// Gets the offset of the element at the given index of the property array with the given name.
        /// </summary>
        /// <param name="name">The name of the property to get.</param>
        /// <param name="index">The index into the property array.</param>
        /// <returns>The offset of the element in the named property.</returns>
        size_t GetOffset(string_view_arg name, size_t index);

        /// <summary>
        /// Gets the offset of the property with the given name in the specified block.
        /// </summary>
        /// <param name="blockIndex">The index of the block to get the property of.</param>
        /// <param name="name">The name of the property to get.</param>
        /// <returns>The offset of the named property in the block.</returns>
        size_t GetOffsetBlock(size_t blockIndex, string_view_arg name);

        /// <summary>
        /// Gets the offset of the element at the given index of the property array with the given name in the specified block.
        /// </summary>
        /// <param name="blockIndex">The index of the block to get the property of.</param>
        /// <param name="name">The name of the property to get.</param>
        /// <param name="index">The index into the property array.</param>
        /// <returns>The offset of the element in the named property in the block.</returns>
        size_t GetOffsetBlock(size_t blockIndex, string_view_arg name, size_t index);

#pragma endregion GetOffset

#pragma region SetDirect

        /// <summary>
        /// Sets a property value directly through the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property being set.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetDirectValue(string_view_arg name, const T& value);

        /// <summary>
        /// Sets a property array element value directly through the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property array being set.</param>
        /// <param name="index">The index of the array to write to.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetDirectValue(string_view_arg name, size_t index, const T& value);

        /// <summary>
        /// Sets property array element values directly through the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property array being set.</param>
        /// <param name="index">The starting index of the array to write to.</param>
        /// <param name="values">The address of the value(s) to write.</param>
        /// <param name="count">How many elements to write.</param>
        template<BufferValue T>
        void SetDirectValues(string_view_arg name, size_t index, const T* values, size_t count);

        /// <summary>
        /// Sets data at the given offset from a property directly through the buffer interface.
        /// </summary>
        /// <param name="name">The name of the property being set.</param>
        /// <param name="offset">The offset into the property to start writing to.</param>
        /// <param name="data">The data to write.</param>
        /// <param name="size">The amount of data to write, in bytes.</param>
        void SetDirect(string_view_arg name, size_t offset, const void* data, size_t size);

        /// <summary>
        /// Sets a property value of a block directly through the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="name">The name of the property being set.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetDirectBlockValue(size_t blockIndex, string_view_arg name, const T& value);

        /// <summary>
        /// Sets a property array element value of a block directly through the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="name">The name of the property array being set.</param>
        /// <param name="index">The index of the array to write to.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetDirectBlockValue(size_t blockIndex, string_view_arg name, size_t index, const T& value);

        /// <summary>
        /// Sets property array element values of a block directly through the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="name">The name of the property array being set.</param>
        /// <param name="index">The starting index of the array to write to.</param>
        /// <param name="values">The address of the value(s) to write.</param>
        /// <param name="count">How many elements to write.</param>
        template<BufferValue T>
        void SetDirectBlockValues(size_t blockIndex, string_view_arg name, size_t index, const T* values, size_t count);

        /// <summary>
        /// Sets data at the given offset from a property of a block directly through the buffer interface.
        /// </summary>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="name">The name of the property being set.</param>
        /// <param name="offset">The offset into the property to start writing to.</param>
        /// <param name="data">The data to write.</param>
        /// <param name="size">The amount of data to write, in bytes.</param>
        void SetDirectBlock(size_t blockIndex, string_view_arg name, size_t offset, const void* data, size_t size);

        /// <summary>
        /// Sets a value at the given offset directly through the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="offset">The offset into the buffer to start writing to.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetDirectValue(size_t offset, const T& value);

        /// <summary>
        /// Sets values starting at the given offset directly through the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="offset">The offset into the buffer to start writing to.</param>
        /// <param name="values">The address of the value(s) to write.</param>
        /// <param name="count">How many elements to write.</param>
        template<BufferValue T>
        void SetDirectValues(size_t offset, const T* values, size_t count = 1);

        /// <summary>
        /// Sets data at the given offset directly through the buffer interface.
        /// </summary>
        /// <param name="offset">The offset into the buffer to start writing.</param>
        /// <param name="data">The data to write into the buffer.</param>
        /// <param name="size">The amount of data to write, in bytes.</param>
        void SetDirect(size_t offset, const void* data, size_t size);

        /// <summary>
        /// Sets a value at the given offset into a block directly through the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="offset">The offset into the block to start writing to.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetDirectBlockValue(size_t blockIndex, size_t offset, const T& value);

        /// <summary>
        /// Sets values starting at the given offset into a block directly through the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="offset">The offset into the block to start writing to.</param>
        /// <param name="values">The address of the value(s) to write.</param>
        /// <param name="count">How many elements to write.</param>
        template<BufferValue T>
        void SetDirectBlockValues(size_t blockIndex, size_t offset, const T* value, size_t count = 1);

        /// <summary>
        /// Sets data at the given offset into a block directly through the buffer interface.
        /// </summary>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="offset">The offset into the block to start writing to.</param>
        /// <param name="data">The data to write into the buffer.</param>
        /// <param name="size">The amount of data to write, in bytes.</param>
        void SetDirectBlock(size_t blockIndex, size_t offset, const void* data, size_t size);

#pragma endregion SetDirect

#pragma region SetLocal

        /// <summary>
        /// Sets a property value in the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property being set.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetLocalValue(string_view_arg name, const T& value);

        /// <summary>
        /// Sets a property array element value in the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property array being set.</param>
        /// <param name="index">The index of the array to write to.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetLocalValue(string_view_arg name, size_t index, const T& value);

        /// <summary>
        /// Sets property array element values in the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property array being set.</param>
        /// <param name="index">The starting index of the array to write to.</param>
        /// <param name="values">The address of the value(s) to write.</param>
        /// <param name="count">How many elements to write.</param>
        template<BufferValue T>
        void SetLocalValues(string_view_arg name, size_t index, const T* value, size_t count);

        /// <summary>
        /// Sets memory at the given offset from a property in the local buffer.
        /// </summary>
        /// <param name="name">The name of the property being set.</param>
        /// <param name="offset">The offset into the property to start writing to.</param>
        /// <param name="data">The data to write.</param>
        /// <param name="size">The amount of data to write, in bytes.</param>
        void SetLocal(string_view_arg name, size_t offset, const void* data, size_t size);

        /// <summary>
        /// Sets a property value of a block in the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="name">The name of the property being set.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetLocalBlockValue(size_t blockIndex, string_view_arg name, const T& value);

        /// <summary>
        /// Sets a property array element value of a block in the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="name">The name of the property array being set.</param>
        /// <param name="index">The index of the array to write to.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetLocalBlockValue(size_t blockIndex, string_view_arg name, size_t index, const T& value);

        /// <summary>
        /// Sets property array element values of a block in the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="name">The name of the property array being set.</param>
        /// <param name="index">The starting index of the array to write to.</param>
        /// <param name="values">The address of the value(s) to write.</param>
        /// <param name="count">How many elements to write.</param>
        template<BufferValue T>
        void SetLocalBlockValues(size_t blockIndex, string_view_arg name, size_t index, const T* value, size_t count);

        /// <summary>
        /// Sets memory at the given offset from a property of a block in the local buffer.
        /// </summary>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="name">The name of the property being set.</param>
        /// <param name="offset">The offset into the property to start writing to.</param>
        /// <param name="data">The data to write.</param>
        /// <param name="size">The amount of data to write, in bytes.</param>
        void SetLocalBlock(size_t blockIndex, string_view_arg name, size_t offset, const void* data, size_t size);

        /// <summary>
        /// Sets a value at the given offset in the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="offset">The offset into the buffer to start writing to.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetLocalValue(size_t offset, const T& value);

        /// <summary>
        /// Sets values starting at the given offset in the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="offset">The offset into the buffer to start writing to.</param>
        /// <param name="values">The address of the value(s) to write.</param>
        /// <param name="count">How many elements to write.</param>
        template<BufferValue T>
        void SetLocalValues(size_t offset, const T* values, size_t count);

        /// <summary>
        /// Sets memory at the given offset in the local buffer.
        /// </summary>
        /// <param name="offset">The offset into the buffer to start writing.</param>
        /// <param name="data">The data to write into the buffer.</param>
        /// <param name="size">The amount of data to write, in bytes.</param>
        void SetLocal(size_t offset, const void* data, size_t size);

        /// <summary>
        /// Sets a value at the given offset into a block in the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="offset">The offset into the block to start writing to.</param>
        /// <param name="value">The value to set.</param>
        template<BufferValue T>
        void SetLocalBlockValue(size_t blockIndex, size_t offset, const T& value);

        /// <summary>
        /// Sets values starting at the given offset into a block in the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="offset">The offset into the block to start writing to.</param>
        /// <param name="values">The address of the value(s) to write.</param>
        /// <param name="count">How many elements to write.</param>
        template<BufferValue T>
        void SetLocalBlockValues(size_t blockIndex, size_t offset, const T* values, size_t count);

        /// <summary>
        /// Sets memory at the given offset into a block in the local buffer.
        /// </summary>
        /// <param name="blockIndex">The index of the block being modified.</param>
        /// <param name="offset">The offset into the block to start writing to.</param>
        /// <param name="data">The data to write into the buffer.</param>
        /// <param name="size">The amount of data to write, in bytes.</param>
        void SetLocalBlock(size_t blockIndex, size_t offset, const void* data, size_t size);

#pragma endregion SetLocal

#pragma region GetDirect

        /// <summary>
        /// Retrieves a property value directly from the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property being retrieved.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetDirectValue(string_view_arg name, T& value);

        /// <summary>
        /// Retrieves a property array element value directly from the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property array being retrieved.</param>
        /// <param name="index">The index of the array to retrieve.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetDirectValue(string_view_arg name, size_t index, T& value);

        /// <summary>
        /// Retrieves property array element values directly from the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property array being retrieved.</param>
        /// <param name="index">The starting index of the array to read from.</param>
        /// <param name="values">The address of the value(s) to store the retrieved data.</param>
        /// <param name="count">How many elements to retrieve.</param>
        template<BufferValue T>
        void GetDirectValues(string_view_arg name, size_t index, T* value, size_t count);

        /// <summary>
        /// Retrieves data at the given offset from a property directly from the buffer interface.
        /// </summary>
        /// <param name="name">The name of the property being retrieved.</param>
        /// <param name="offset">The offset into the property to start reading from.</param>
        /// <param name="data">The address to store the retrieved data.</param>
        /// <param name="size">The amount of data to retrieve, in bytes.</param>
        void GetDirect(string_view_arg name, size_t offset, void* data, size_t size);

        /// <summary>
        /// Retrieves a property value of a block directly from the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="name">The name of the property being retrieved.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetDirectBlockValue(size_t blockIndex, string_view_arg name, T& value);

        /// <summary>
        /// Retrieves a property array element value of a block directly from the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="name">The name of the property array being retrieved.</param>
        /// <param name="index">The index of the array to write to.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetDirectBlockValue(size_t blockIndex, string_view_arg name, size_t index, T& value);

        /// <summary>
        /// Retrieves property array element values of a block directly from the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="name">The name of the property array being retrieved.</param>
        /// <param name="index">The starting index of the array to reading from.</param>
        /// <param name="values">The address of the value(s) to store the retrieved data.</param>
        /// <param name="count">How many elements to retrieve.</param>
        template<BufferValue T>
        void GetDirectBlockValues(size_t blockIndex, string_view_arg name, size_t index, T* values, size_t count);

        /// <summary>
        /// Retrieves data at the given offset from a property of a block directly from the buffer interface.
        /// </summary>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="name">The name of the property being retrieved.</param>
        /// <param name="offset">The offset into the property to start reading from.</param>
        /// <param name="data">The address to store the retrieved data.</param>
        /// <param name="size">The amount of data to retrieve, in bytes.</param>
        void GetDirectBlock(size_t blockIndex, string_view_arg name, size_t offset, void* data, size_t size);

        /// <summary>
        /// Retrieves a value at the given offset directly from the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="offset">The offset into the buffer to start reading from.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetDirectValue(size_t offset, T& value);

        /// <summary>
        /// Retrieves values starting at the given offset directly from the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="offset">The offset into the buffer to start reading from.</param>
        /// <param name="values">The address of the value(s) to store the retrieved data.</param>
        /// <param name="count">How many elements to retrieve.</param>
        template<BufferValue T>
        void GetDirectValues(size_t offset, T* values, size_t count = 1);

        /// <summary>
        /// Retrieves data at the given offset directly from the buffer interface.
        /// </summary>
        /// <param name="offset">The offset into the buffer to start reading from.</param>
        /// <param name="data">The address to store the retrieved data.</param>
        /// <param name="size">The amount of data to retrieve, in bytes.</param>
        void GetDirect(size_t offset, void* data, size_t size);

        /// <summary>
        /// Retrieves a value at the given offset into a block directly from the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="offset">The offset into the block to start reading from.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetDirectBlockValue(size_t blockIndex, size_t offset, T& value);

        /// <summary>
        /// Retrieves values starting at the given offset into a block directly from the buffer interface.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="offset">The offset into the block to start reading from.</param>
        /// <param name="values">The address of the value(s) to store the retrieved data.</param>
        /// <param name="count">How many elements to retrieve.</param>
        template<BufferValue T>
        void GetDirectBlockValues(size_t blockIndex, size_t offset, T* values, size_t count = 1);

        /// <summary>
        /// Retrieves data at the given offset into a block directly from the buffer interface.
        /// </summary>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="offset">The offset into the block to start reading from.</param>
        /// <param name="data">The address to store the retrieved data.</param>
        /// <param name="size">The amount of data to retrieve, in bytes.</param>
        void GetDirectBlock(size_t blockIndex, size_t offset, void* data, size_t size);

#pragma endregion GetDirect

#pragma region GetLocal

        /// <summary>
        /// Retrieves a property value from the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property being retrieved.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetLocalValue(string_view_arg name, T& value);

        /// <summary>
        /// Retrieves a property array element value from the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property array being retrieved.</param>
        /// <param name="index">The index of the array to retrieve.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetLocalValue(string_view_arg name, size_t index, T& value);

        /// <summary>
        /// Retrieves property array element values from the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name of the property array being retrieved.</param>
        /// <param name="index">The starting index of the array to read from.</param>
        /// <param name="values">The address of the value(s) to store the retrieved data.</param>
        /// <param name="count">How many elements to retrieve.</param>
        template<BufferValue T>
        void GetLocalValues(string_view_arg name, size_t index, T* values, size_t count);

        /// <summary>
        /// Retrieves data at the given offset from a property from the local buffer.
        /// </summary>
        /// <param name="name">The name of the property being retrieved.</param>
        /// <param name="offset">The offset into the property to start reading from.</param>
        /// <param name="data">The address to store the retrieved data.</param>
        /// <param name="size">The amount of data to retrieve, in bytes.</param>
        void GetLocal(string_view_arg name, size_t offset, void* data, size_t size);

        /// <summary>
        /// Retrieves a property value of a block from the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="name">The name of the property being retrieved.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetLocalBlockValue(size_t blockIndex, string_view_arg name, T& value);

        /// <summary>
        /// Retrieves a property array element value of a block from the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="name">The name of the property array being retrieved.</param>
        /// <param name="index">The index of the array to write to.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetLocalBlockValue(size_t blockIndex, string_view_arg name, size_t index, T& value);

        /// <summary>
        /// Retrieves property array element values of a block from the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="name">The name of the property array being retrieved.</param>
        /// <param name="index">The starting index of the array to reading from.</param>
        /// <param name="values">The address of the value(s) to store the retrieved data.</param>
        /// <param name="count">How many elements to retrieve.</param>
        template<BufferValue T>
        void GetLocalBlockValues(size_t blockIndex, string_view_arg name, size_t index, T* values, size_t count);

        /// <summary>
        /// Retrieves data at the given offset from a property of a block from the local buffer.
        /// </summary>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="name">The name of the property being retrieved.</param>
        /// <param name="offset">The offset into the property to start reading from.</param>
        /// <param name="data">The address to store the retrieved data.</param>
        /// <param name="size">The amount of data to retrieve, in bytes.</param>
        void GetLocalBlock(size_t blockIndex, string_view_arg name, size_t offset, void* data, size_t size);

        /// <summary>
        /// Retrieves a value at the given offset from the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="offset">The offset into the buffer to start reading from.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetLocalValue(size_t offset, T& value);

        /// <summary>
        /// Retrieves values starting at the given offset from the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="offset">The offset into the buffer to start reading from.</param>
        /// <param name="values">The address of the value(s) to store the retrieved data.</param>
        /// <param name="count">How many elements to retrieve.</param>
        template<BufferValue T>
        void GetLocalValues(size_t offset, T* values, size_t count);

        /// <summary>
        /// Retrieves data at the given offset from the local buffer.
        /// </summary>
        /// <param name="offset">The offset into the buffer to start reading from.</param>
        /// <param name="data">The address to store the retrieved data.</param>
        /// <param name="size">The amount of data to retrieve, in bytes.</param>
        void GetLocal(size_t offset, void* data, size_t size);

        /// <summary>
        /// Retrieves a value at the given offset into a block from the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="offset">The offset into the block to start reading from.</param>
        /// <param name="value">The value to store the retrieved data.</param>
        template<BufferValue T>
        void GetLocalBlockValue(size_t blockIndex, size_t offset, T& value);

        /// <summary>
        /// Retrieves values starting at the given offset into a block from the local buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="offset">The offset into the block to start reading from.</param>
        /// <param name="values">The address of the value(s) to store the retrieved data.</param>
        /// <param name="count">How many elements to retrieve.</param>
        template<BufferValue T>
        void GetLocalBlockValues(size_t blockIndex, size_t offset, T* values, size_t count);

        /// <summary>
        /// Retrieves data at the given offset into a block from the local buffer.
        /// </summary>
        /// <param name="blockIndex">The index of the block being retrieved.</param>
        /// <param name="offset">The offset into the block to start reading from.</param>
        /// <param name="data">The address to store the retrieved data.</param>
        /// <param name="size">The amount of data to retrieve, in bytes.</param>
        void GetLocalBlock(size_t blockIndex, size_t offset, void* data, size_t size);

#pragma endregion GetLocal

		// Private Constants, Enums, and Structs:
	protected:

		// Private Static Functions:
    protected:

		// Private Functions:
    protected:
        /// <summary>
        /// Copies the contents of another buffer into this buffer and resizes accordingly. Only affects local and graphics API buffers if they already exist.
        /// </summary>
        /// <param name="other">The buffer to copy from.</param>
        void CopyBuffer(const GraphicsBuffer& other);

		// Private Static Variables:
    protected:

		// Private Variables:
    protected:
        // The descriptor for this buffer. Must always exist, otherwise we don't know important information like capacity.
        const BufferDescriptor& descriptor_;
        // Identifier of this buffer for the graphics API.
        ResourceID id_;
        // How many descriptor blocks are contained in this buffer.
        size_t blockCount_;
        // Maximum number of descriptor blocks this buffer can contain without needing to resize.
        size_t blockCapacity_;
        // Pointer to local copy of buffer.
        void* local_;
	};

}	// namespace

#include "GraphicsBuffer.inl"
