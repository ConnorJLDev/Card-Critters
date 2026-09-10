//------------------------------------------------------------------------------
//
// File Name:	Container.inl
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

#include "Container.h"

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

	template <typename T>
	Container<T>::Container()
		: itemMax_(0)
		, items_()
	{
	}

	//--------------------------------------------------------------------------

	template <typename T>
	Container<T>::~Container()
	{
		FreeAll();
		Shrink();
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

	template <typename T>
	bool Container<T>::Add(T* item)
	{
		assert(item != nullptr);
#ifdef _DEBUG
		for (T* itemiter : items_)
		{
			assert(itemiter != item);
		}
#endif
		items_.push_back(item);
		size_t itemCount_ = Count();
		itemMax_ = itemMax_ > itemCount_ ? itemMax_ : itemCount_;
		return true;
	}

	/*template<typename T>
	bool Container<T>::Add(std::shared_ptr<T> item)
	{
		assert(item != nullptr);
		items_.push_back(item);
		size_t itemCount_ = Count();
		itemMax_ = itemMax_ > itemCount_ ? itemMax_ : itemCount_;
		return true;
	}*/

	template <typename T>
	bool Container<T>::Add(T& item)
	{
		return Add(&item);
	}

	template <typename T>
	T* Container<T>::Find(string_view_arg name) const
	{
		//auto iter = std::find_if(items_.begin(), items_.end(), [&name](const T& item) { return item->IsNamed(name); });
		//return iter == items_.end() ? nullptr : &*iter;
		for (T* item : items_)
		{
			if (item->IsNamed(name))
			{
				return item;
			}
		}
		return nullptr;
	}

	template <typename T>
	bool Container<T>::Find(string_view_arg name, T*& item) const
	{
		item = Find(name);
		return item != nullptr;
	}

	template <typename T>
	bool Container<T>::Remove(T* item)
	{
		bool found;
		auto itemIter = GetIter(item, found);
		if (found)
		{
			items_.erase(itemIter);
		}
		return found;
	}

	template <typename T>
	bool Container<T>::Destroy(T*& item)
	{
		bool found;
		auto itemIter = GetIter(item, found);
		if (found)
		{
			delete item;
			items_.erase(itemIter);
		}
		return found;
	}

	template <typename T>
	bool Container<T>::IsEmpty() const
	{
		return Count() <= 0;
	}

	template <typename T>
	size_t Container<T>::Count() const
	{
		return items_.size();
	}

	template <typename T>
	void Container<T>::FreeAll()
	{
		for (T* item : items_)
		{
			delete item;
		}
		items_.clear();
	}

	template <typename T>
	void Container<T>::Shrink()
	{
		items_.shrink_to_fit();
	}

	template<typename T>
	T* Container<T>::GetFromIndex(unsigned index)
	{
		return items_[index];
	}

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

	template <typename T>
	vector_t<T*>::const_iterator Container<T>::GetIter(T* item, bool& found) const
	{
		for (auto itemIter = items_.cbegin(); itemIter != items_.cend(); ++itemIter)
		{
			if (*itemIter == item)
			{
				found = true;
				return itemIter;
			}
		}
		found = false;
		return items_.end();
	}

#pragma endregion Private Functions

}	// namespace
