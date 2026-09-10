//------------------------------------------------------------------------------
//
// File Name:	Sampler.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Sampler used for textures in shaders.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Sampler.h"

#include GRAPHICS_INCLUDE

#include "Library.inl"
#include "LibraryStatic.inl"

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

    ENUM_MAPS_CLASS_SEQUENTIAL(Sampler, WrapMode, Clamp, Border, Repeat, Mirror);
    ENUM_MAPS_CLASS_SEQUENTIAL(Sampler, FilterMinification, Nearest, Linear, NearestMipmapNearest, LinearMipmapNearest, NearestMipmapLinear, LinearMipmapLinear);
    ENUM_MAPS_CLASS_SEQUENTIAL(Sampler, FilterMagnification, Nearest, Linear);
    ENUM_MAPS_CLASS_SEQUENTIAL(Sampler, Swizzle, Red, Green, Blue, Alpha, Zero, One);

	//--------------------------------------------------------------------------
	// Public Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Constants:
	//--------------------------------------------------------------------------

    constexpr array_t<ResourceEnum, Sampler::WrapMode::NUM_ENTRIES> Sampler::WrapModeValues
    {
        GL_CLAMP_TO_EDGE,
        GL_CLAMP_TO_BORDER,
        GL_REPEAT,
        GL_MIRRORED_REPEAT,
    };

    constexpr array_t<ResourceEnum, Sampler::FilterMinification::NUM_ENTRIES> Sampler::FilterMinificationValues
    {
        GL_NEAREST,
        GL_LINEAR,
        GL_NEAREST_MIPMAP_NEAREST,
        GL_LINEAR_MIPMAP_NEAREST,
        GL_NEAREST_MIPMAP_LINEAR,
        GL_LINEAR_MIPMAP_LINEAR
    };

    constexpr array_t<ResourceEnum, Sampler::FilterMagnification::NUM_ENTRIES> Sampler::FilterMagnificationValues
    {
        GL_NEAREST,
        GL_LINEAR,
    };

    constexpr array_t<ResourceEnum, Sampler::Swizzle::NUM_ENTRIES> Sampler::SwizzleValues
    {
        GL_RED,
        GL_GREEN,
        GL_BLUE,
        GL_ALPHA,
        GL_ONE,
        GL_ZERO,
    };
    
    constexpr StreamKey
          Sampler::ReadKeyWrapR = "WrapR"
        , Sampler::ReadKeyWrapS = "WrapS"
        , Sampler::ReadKeyWrapT = "WrapT"
        , Sampler::ReadKeyFilterMin = "FilterMin"
        , Sampler::ReadKeyFilterMag = "FilterMag"
        , Sampler::ReadKeySwizzleR = "SwizzleR"
        , Sampler::ReadKeySwizzleG = "SwizzleG"
        , Sampler::ReadKeySwizzleB = "SwizzleB"
        , Sampler::ReadKeySwizzleA = "SwizzleA"
        , Sampler::ReadKeyLODMin = "LODMin"
        , Sampler::ReadKeyLODMax = "LODMax"
        , Sampler::ReadKeyLODBias = "LODBias"
        , Sampler::ReadKeyBorderColor = "BorderColor"
        ;

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

	Sampler::Sampler(void)
		: NamedObject()
        , wrapR_(WrapMode::Mirror)
        , wrapS_(WrapMode::Mirror)
        , wrapT_(WrapMode::Mirror)
        , filterMin_(FilterMinification::Linear)
        , filterMag_(FilterMagnification::Linear)
        , swizzleR_(Swizzle::Red)
        , swizzleG_(Swizzle::Green)
        , swizzleB_(Swizzle::Blue)
        , swizzleA_(Swizzle::Alpha)
        , lodLimits_(-1000.0f, 1000.0f)
        , lodBias_(0.0f)
        , borderColor_(0.0f, 0.0f, 0.0f, 0.0f)
	{
	}

    Sampler::Sampler(const Sampler& other)
        : NamedObject(other)
        , wrapR_(other.wrapR_)
        , wrapS_(other.wrapS_)
        , wrapT_(other.wrapT_)
        , filterMin_(other.filterMin_)
        , filterMag_(other.filterMag_)
        , swizzleR_(other.swizzleR_)
        , swizzleG_(other.swizzleG_)
        , swizzleB_(other.swizzleB_)
        , swizzleA_(other.swizzleA_)
        , lodLimits_(other.lodLimits_)
        , lodBias_(other.lodBias_)
        , borderColor_(other.borderColor_)
    {
    }

    Sampler::Sampler(Sampler&& other) noexcept
        : NamedObject(std::move(other))
        , wrapR_(std::move(other.wrapR_))
        , wrapS_(std::move(other.wrapS_))
        , wrapT_(std::move(other.wrapT_))
        , filterMin_(std::move(other.filterMin_))
        , filterMag_(std::move(other.filterMag_))
        , swizzleR_(std::move(other.swizzleR_))
        , swizzleG_(std::move(other.swizzleG_))
        , swizzleB_(std::move(other.swizzleB_))
        , swizzleA_(std::move(other.swizzleA_))
        , lodLimits_(std::move(other.lodLimits_))
        , lodBias_(std::move(other.lodBias_))
        , borderColor_(std::move(other.borderColor_))
    {
    }

    Sampler& Sampler::operator=(const Sampler& other)
    {
        NamedObject::operator=(other);
        wrapR_ = other.wrapR_;
        wrapS_ = other.wrapS_;
        wrapT_ = other.wrapT_;
        filterMin_ = other.filterMin_;
        filterMag_ = other.filterMag_;
        swizzleR_ = other.swizzleR_;
        swizzleG_ = other.swizzleG_;
        swizzleB_ = other.swizzleB_;
        swizzleA_ = other.swizzleA_;
        lodLimits_ = other.lodLimits_;
        lodBias_ = other.lodBias_;
        borderColor_ = other.borderColor_;
        return *this;
    }

	//--------------------------------------------------------------------------

	Sampler::~Sampler(void)
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

    void Sampler::Read(StreamIn& stream)
    {
        ReadName(stream);
        stream.TryReadEnum(ReadKeyWrapR, ENUM_READ(WrapMode), wrapR_);
        stream.TryReadEnum(ReadKeyWrapS, ENUM_READ(WrapMode), wrapS_);
        stream.TryReadEnum(ReadKeyWrapT, ENUM_READ(WrapMode), wrapT_);
        stream.TryReadEnum(ReadKeyFilterMin, ENUM_READ(FilterMinification), filterMin_);
        stream.TryReadEnum(ReadKeyFilterMag, ENUM_READ(FilterMagnification), filterMag_);
        stream.TryReadEnum(ReadKeySwizzleR, ENUM_READ(Swizzle), swizzleR_);
        stream.TryReadEnum(ReadKeySwizzleG, ENUM_READ(Swizzle), swizzleG_);
        stream.TryReadEnum(ReadKeySwizzleB, ENUM_READ(Swizzle), swizzleB_);
        stream.TryReadEnum(ReadKeySwizzleA, ENUM_READ(Swizzle), swizzleA_);
        stream.TryReadValue(ReadKeyLODMin, lodMin_);
        stream.TryReadValue(ReadKeyLODMax, lodMax_);
        stream.TryReadValue(ReadKeyLODBias, lodBias_);
        stream.TryReadValue(ReadKeyBorderColor, borderColor_);
    }

    Sampler::WrapMode Sampler::GetWrapR(void) const
    {
        return wrapR_;
    }

    void Sampler::SetWrapR(WrapMode wrap)
    {
        wrapR_ = wrap;
    }

    Sampler::WrapMode Sampler::GetWrapS(void) const
    {
        return wrapS_;
    }

    void Sampler::SetWrapS(WrapMode wrap)
    {
        wrapS_ = wrap;
    }

    Sampler::WrapMode Sampler::GetWrapT(void) const
    {
        return wrapT_;
    }

    void Sampler::SetWrapT(WrapMode wrap)
    {
        wrapT_ = wrap;
    }

    Sampler::FilterMinification Sampler::GetFilterMinification(void) const
    {
        return filterMin_;
    }

    void Sampler::SetFilterMinification(FilterMinification filter)
    {
        filterMin_ = filter;
    }

    Sampler::FilterMagnification Sampler::GetFilterMagnification(void) const
    {
        return filterMag_;
    }

    void Sampler::SetFilterMagnification(FilterMagnification filter)
    {
        filterMag_ = filter;
    }

    array_t<Sampler::Swizzle, 4> Sampler::GetSwizzle(void) const
    {
        return array_t<Swizzle, 4>({swizzleR_, swizzleG_, swizzleB_, swizzleA_});
    }

    void Sampler::SetSwizzle(array_t<Swizzle, 4> swizzle)
    {
        swizzleR_ = swizzle[0];
        swizzleG_ = swizzle[1];
        swizzleB_ = swizzle[2];
        swizzleA_ = swizzle[3];
    }

    float Sampler::GetLODMin(void) const
    {
        return lodMin_;
    }

    void Sampler::SetLODMin(float lod)
    {
        lodMin_ = lod;
    }

    float Sampler::GetLODMax(void) const
    {
        return lodMax_;
    }

    void Sampler::SetLODMax(float lod)
    {
        lodMax_ = lod;
    }

    float Sampler::GetLODBias(void) const
    {
        return lodBias_;
    }

    void Sampler::SetLODBias(float bias)
    {
        lodBias_ = bias;
    }

    const Vec4& Sampler::GetBorderColor(void) const
    {
        return borderColor_;
    }

    void Sampler::SetBorderColor(const Vec4& color)
    {
        borderColor_ = color;
    }

    void Sampler::Activate() const
    {
        // Wrap mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, (GLint)WrapModeValues[(unsigned int)wrapR_]);
        OpenGLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)WrapModeValues[(unsigned int)wrapS_]);
        OpenGLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)WrapModeValues[(unsigned int)wrapT_]);
        OpenGLCheckError();
        // Filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)FilterMinificationValues[(unsigned int)filterMin_]);
        OpenGLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)FilterMagnificationValues[(unsigned int)filterMag_]);
        OpenGLCheckError();
        // Swizzle
        const array_t<GLint, 4> swizzle
        ({
            (GLint)SwizzleValues[(unsigned int)swizzleR_],
            (GLint)SwizzleValues[(unsigned int)swizzleG_],
            (GLint)SwizzleValues[(unsigned int)swizzleB_],
            (GLint)SwizzleValues[(unsigned int)swizzleA_],
        });
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle.data());
        OpenGLCheckError();
        // LOD
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, (GLfloat)lodMin_);
        OpenGLCheckError();
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, (GLfloat)lodMax_);
        OpenGLCheckError();
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, (GLfloat)lodBias_);
        OpenGLCheckError();
        // Border
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, reinterpret_cast<const GLfloat*>(&borderColor_));
        OpenGLCheckError();
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

    //--------------------------------------------------------------------------
    // Library Implementation:
    //--------------------------------------------------------------------------

#pragma region Library Implementation

    template SamplerLibrary;

    INSTANTIATE_LIBRARY_IMGUI_SELECTOR(Sampler);

    IMPLEMENT_READ(Sampler);

    IMPLEMENT_WRITE(Sampler);

#pragma endregion Library Implementation

}	// namespace
