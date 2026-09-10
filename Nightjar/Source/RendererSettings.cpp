//------------------------------------------------------------------------------
//
// File Name:	RendererSettings.cpp
// Author(s):	JSand
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new object.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "RendererSettings.h"

#include "GraphicsSystem.h"

#include GRAPHICS_INCLUDE

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

	RendererSettings::RendererSettings(void)
	{
	}

	//--------------------------------------------------------------------------

	RendererSettings::~RendererSettings(void)
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

    void RendererSettings::Activate()
    {
        glViewport((GLint)viewportOffset_.x, (GLint)viewportOffset_.y, (GLsizei)viewportSize_.x, (GLsizei)viewportSize_.y);
        OpenGLCheckError();

        if (scissorEnable_)
        {
            glEnable(GL_SCISSOR_TEST);
            OpenGLCheckError();

            glScissor((GLint)scissorOffset_.x, (GLint)scissorOffset_.y, (GLsizei)scissorSize_.x, (GLsizei)scissorSize_.y);
            OpenGLCheckError();
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
            OpenGLCheckError();
        }
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

#pragma endregion Private Functions

}	// namespace
