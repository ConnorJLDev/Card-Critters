//------------------------------------------------------------------------------
//
// File Name:	Container.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for a container of NamedObjects.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Object.h"

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

	// Class Definition:
	template <typename T>
	class Container : public Object
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		Container();

		virtual ~Container();

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Add an Item to the Container.
		/// </summary>
		/// <param name="item">The item to be added.</param>
		/// <returns>True if the item was added successfully, false otherwise.</returns>
		virtual bool Add(T* item);
		
		/// <summary>
		/// Add an Item to the Container.
		/// </summary>
		/// <param name="item">The item to be added.</param>
		/// <returns>True if the item was added successfully, false otherwise.</returns>
		//virtual bool Add(std::shared_ptr<T> item);

		/// <summary>
		/// Add an Item to the Container.
		/// </summary>
		/// <param name="item">The item to be added.</param>
		/// <returns>True if the item was added successfully, false otherwise.</returns>
		virtual bool Add(T& item);

		/// <summary>
		/// Find an Item in the Container that has a matching name.
		/// </summary>
		/// <param name="name">The name of the Item to be returned.</param>
		/// <returns>The pointer to the first Item located, or nullptr if no Item was found.</returns>
		virtual T* Find(string_view_arg name) const;

		/// <summary>
		/// Find an Item in the Container that has a matching name.
		/// </summary>
		/// <param name="name">The name of the Item to be returned.</param>
		/// <param name="item">The reference to be set to the Item, or to nullptr if no Item is found.</param>
		/// <returns>True if an item was located, false otherwise.</returns>
		virtual bool Find(string_view_arg name, T*& item) const;

		/// <summary>
		/// Removes an Item from the container.
		/// </summary>
		/// <param name="item">The Item to be removed.</param>
		/// <returns>True if the Item was found and removed, false otherwise.</returns>
		virtual bool Remove(T* item);

		/// <summary>
		/// Removes an Item from the container and destroys it.
		/// </summary>
		/// <param name="item">The Item to be removed and destroyed.</param>
		/// <returns>True if the Item was found and removed and destroyed, false otherwise.</returns>
		virtual bool Destroy(T*& item);

		/// <summary>
		/// Determines if the Container is empty (no Items exist).
		/// </summary>
		/// <returns>True if no Items exist, false otherwise.</returns>
		virtual bool IsEmpty() const;

		/// <summary>
		/// Determines how many Items are in the Container.
		/// </summary>
		/// <returns>The number of items in the Container.</returns>
		size_t Count() const;

		/// <summary>
		/// Free all Items in the Container.
		/// </summary>
		virtual void FreeAll();

		/// <summary>
		/// Deallocates unused space in the Container.
		/// </summary>
		virtual void Shrink();

		/// <summary>
		/// Gets the Item at the specified index.
		/// </summary>
		/// <param name="index">The index of the item.</param>
		/// <returns>A pointer to the Item.</returns>
		virtual T* GetFromIndex(unsigned index);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		/// <summary>
		/// Gets the iterator of an item in the container.
		/// </summary>
		/// <param name="item">The Item to be removed.</param>
		/// <param name="found">The value to contain if the Item was found.</param>
		/// <returns>The iterator containing the Item if found, else the end iterator.</returns>
		vector_t<T*>::const_iterator GetIter(T* item, bool& found) const;


		// Private Static Variables:
	private:

		// Private Variables:
	private:
		/// <summary>
		/// The maximum size of the Container.
		/// </summary>
		size_t itemMax_;

	protected:
		/// <summary>
		/// The actual Item Container.
		/// </summary>
		//vector_t<std::shared_ptr<T>> items_;
		vector_t<T*> items_;
	};

}	// namespace

#include "Container.inl"
