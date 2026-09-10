//------------------------------------------------------------------------------
//
// File Name:	Systems.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Common header to include all system types.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

// Engine systems
#include "IOSystem.h"
#include "PersistenceSystem.h"
#include "PlatformSystem.h"
#include "PerformanceSystem.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "TextSystem.h"
#include "AudioSystem.h"
#include "SceneSystem.h"
#include "MessageSystem.h"

// Game systems
#include "SaveSystem.h"
#include "CheatSystem.h"
#include "TurnSystem.h"

// Engine systems that need SceneSystem::IsChanging
#ifdef _EDITOR
#include "ImGuiSystem.h"
#endif
