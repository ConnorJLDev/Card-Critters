//------------------------------------------------------------------------------
//
// File Name:	MaterialRenderSettings.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing render settings.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MaterialRenderSettings.h"

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
	// Protected Constants:
	//--------------------------------------------------------------------------

#define READKEY_DUAL(name, first, second)\
        , MaterialRenderSettings::ReadKey##name = #name\
        , MaterialRenderSettings::ReadKey##name##first = #name #first\
        , MaterialRenderSettings::ReadKey##name##second = #name #second\

#define READKEY_COLOR_ALPHA(name) READKEY_DUAL(Blend##name, Color, Alpha)
#define READKEY_FRONT_BACK(name) READKEY_DUAL(Stencil##name, Front, Back)

    constexpr StreamKey
          MaterialRenderSettings::ReadKeyFaceCull = "FaceCull"
        , MaterialRenderSettings::ReadKeyMixMode = "MixMode"
        , MaterialRenderSettings::ReadKeyBlendConstant = "BlendConstant"
        READKEY_COLOR_ALPHA(Source)
        READKEY_COLOR_ALPHA(Destination)
        READKEY_COLOR_ALPHA(Equation)
        , MaterialRenderSettings::ReadKeyLogicOperation = "LogicOperation"
        , MaterialRenderSettings::ReadKeyDepthComparison = "DepthComparison"
        , MaterialRenderSettings::ReadKeyDepthTest = "DepthTest"
        , MaterialRenderSettings::ReadKeyDepthWrite = "DepthWrite"
        , MaterialRenderSettings::ReadKeyColorMask = "ColorMask"
        , MaterialRenderSettings::ReadKeyStencilTest = "StencilTest"
        READKEY_FRONT_BACK(WriteMask)
        READKEY_FRONT_BACK(CompareMask)
        READKEY_FRONT_BACK(CompareReferenceValue)
        READKEY_FRONT_BACK(Comparison)
        READKEY_FRONT_BACK(OperationPass)
        READKEY_FRONT_BACK(OperationDepthFail)
        READKEY_FRONT_BACK(OperationFail)
        ;

    ENUM_MAPS_CLASS_SEQUENTIAL(MaterialRenderSettings, FaceCull,
        None,
        Front,
        Back,
        All
    );

    ENUM_MAPS_CLASS_SEQUENTIAL(MaterialRenderSettings, MixMode,
        None,
        Blend,
        Logic
    );

    ENUM_MAPS_CLASS_SEQUENTIAL(MaterialRenderSettings, BlendFactor,
        Zero,
        One,
        SourceColor,
        OneMinusSourceColor,
        DestinationColor,
        OneMinusDestinationColor,
        SourceAlpha,
        OneMinusSourceAlpha,
        DestinationAlpha,
        OneMinusDestinationAlpha,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha
    );

    ENUM_MAPS_CLASS_SEQUENTIAL(MaterialRenderSettings, BlendEquation,
        Add,
        Subtract,
        ReverseSubtract,
        Minimum,
        Maximum
    );

    ENUM_MAPS_CLASS_SEQUENTIAL(MaterialRenderSettings, LogicOperation,
        Clear,
        Set,
        Copy,
        CopyInverted,
        Noop,
        Invert,
        And,
        Nand,
        Or,
        Nor,
        Xor,
        Equivalent,
        AndReverse,
        AndInverted,
        OrReverse,
        OrInverted
    );

    ENUM_MAPS_CLASS_SEQUENTIAL(MaterialRenderSettings, Comparison,
        Never,
        LessOrEqual,
        Less,
        Equal,
        NotEqual,
        GreaterOrEqual,
        Greater,
        Always
    );

    ENUM_MAPS_CLASS_SEQUENTIAL(MaterialRenderSettings, StencilOperation,
        Keep,
        Zero,
        Replace,
        Increment,
        IncrementWrap,
        Decrement,
        DecrementWrap,
        Invert
    );

    static constexpr array_t<ResourceEnum, (size_t)MaterialRenderSettings::FaceCull::NUM_ENTRIES> FaceCullValues
    {
        0,
        GL_FRONT,
        GL_BACK,
        GL_FRONT_AND_BACK,
    };

    static constexpr array_t<ResourceEnum, (size_t)MaterialRenderSettings::BlendFactor::NUM_ENTRIES> BlendFactorValues
    {
        GL_ZERO,
        GL_ONE,
        GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR,
        GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,
        GL_CONSTANT_COLOR,
        GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA,
        GL_ONE_MINUS_CONSTANT_ALPHA,
    };

    static constexpr array_t<ResourceEnum, (size_t)MaterialRenderSettings::LogicOperation::NUM_ENTRIES> BlendLogicValues
    {
        GL_CLEAR,
        GL_SET,
        GL_COPY,
        GL_COPY_INVERTED,
        GL_NOOP,
        GL_INVERT,
        GL_AND,
        GL_NAND,
        GL_OR,
        GL_NOR,
        GL_XOR,
        GL_EQUIV,
        GL_AND_REVERSE,
        GL_AND_INVERTED,
        GL_OR_REVERSE,
        GL_OR_INVERTED,
    };

    static constexpr array_t<ResourceEnum, (size_t)MaterialRenderSettings::BlendEquation::NUM_ENTRIES> BlendEquationValues
    {
        GL_FUNC_ADD,
        GL_FUNC_SUBTRACT,
        GL_FUNC_REVERSE_SUBTRACT,
        GL_MIN,
        GL_MAX,
    };

    static constexpr array_t<ResourceEnum, (size_t)MaterialRenderSettings::Comparison::NUM_ENTRIES> ComparisonValues
    {
        GL_NEVER,
        GL_LEQUAL,
        GL_LESS,
        GL_EQUAL,
        GL_NOTEQUAL,
        GL_GEQUAL,
        GL_GREATER,
        GL_ALWAYS,
    };

    static constexpr array_t<ResourceEnum, (size_t)MaterialRenderSettings::StencilOperation::NUM_ENTRIES> StencilOpValues
    {
        GL_KEEP,
        GL_ZERO,
        GL_REPLACE,
        GL_INCR,
        GL_INCR_WRAP,
        GL_DECR,
        GL_DECR_WRAP,
        GL_INVERT,
    };

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

	MaterialRenderSettings::MaterialRenderSettings(void)
		: blendConstant_(0.0f, 0.0f, 0.0f, 0.0f)
        , mixMode_(MixMode::Blend)
        , blendLogic_(LogicOperation::Copy)
        , blendSrcColor_(BlendFactor::SourceAlpha)
        , blendSrcAlpha_(BlendFactor::SourceAlpha)
        , blendDstColor_(BlendFactor::OneMinusSourceAlpha)
        , blendDstAlpha_(BlendFactor::OneMinusSourceAlpha)
        , blendEquationColor_(BlendEquation::Add)
        , blendEquationAlpha_(BlendEquation::Add)
        , faceCull_(FaceCull::None)
        , depthComp_(Comparison::LessOrEqual)
        , depthTest_(true)
        , depthWrite_(true)
        , colorMaskR_(true)
        , colorMaskG_(true)
        , colorMaskB_(true)
        , colorMaskA_(true)
        , stencilTest_(false)
        , stencilWriteMaskFront_((StencilMask_t)-1)
        , stencilWriteMaskBack_((StencilMask_t)-1)
        , stencilCompMaskFront_((StencilMask_t)-1)
        , stencilCompMaskBack_((StencilMask_t)-1)
        , stencilCompRefFront_(0)
        , stencilCompRefBack_(0)
        , stencilCompFront_(Comparison::Always)
        , stencilCompBack_(Comparison::Always)
        , stencilOpPassFront_(StencilOperation::Keep)
        , stencilOpPassBack_(StencilOperation::Keep)
        , stencilOpDepthFailFront_(StencilOperation::Keep)
        , stencilOpDepthFailBack_(StencilOperation::Keep)
        , stencilOpFailFront_(StencilOperation::Keep)
        , stencilOpFailBack_(StencilOperation::Keep)
	{
	}

	MaterialRenderSettings::MaterialRenderSettings(const MaterialRenderSettings& other)
        : blendConstant_(other.blendConstant_)
        , mixMode_(other.mixMode_)
        , blendLogic_(other.blendLogic_)
        , blendSrcColor_(other.blendSrcColor_)
        , blendSrcAlpha_(other.blendSrcAlpha_)
        , blendDstColor_(other.blendDstColor_)
        , blendDstAlpha_(other.blendDstAlpha_)
        , blendEquationColor_(other.blendEquationColor_)
        , blendEquationAlpha_(other.blendEquationAlpha_)
        , faceCull_(other.faceCull_)
        , depthComp_(other.depthComp_)
        , depthTest_(other.depthTest_)
        , depthWrite_(other.depthWrite_)
        , colorMaskR_(other.colorMaskR_)
        , colorMaskG_(other.colorMaskG_)
        , colorMaskB_(other.colorMaskB_)
        , colorMaskA_(other.colorMaskA_)
        , stencilTest_(other.stencilTest_)
        , stencilWriteMaskFront_(other.stencilWriteMaskFront_)
        , stencilWriteMaskBack_(other.stencilWriteMaskBack_)
        , stencilCompMaskFront_(other.stencilCompMaskFront_)
        , stencilCompMaskBack_(other.stencilCompMaskBack_)
        , stencilCompRefFront_(other.stencilCompRefFront_)
        , stencilCompRefBack_(other.stencilCompRefBack_)
        , stencilCompFront_(other.stencilCompFront_)
        , stencilCompBack_(other.stencilCompBack_)
        , stencilOpPassFront_(other.stencilOpPassFront_)
        , stencilOpPassBack_(other.stencilOpPassBack_)
        , stencilOpDepthFailFront_(other.stencilOpDepthFailFront_)
        , stencilOpDepthFailBack_(other.stencilOpDepthFailBack_)
        , stencilOpFailFront_(other.stencilOpFailFront_)
        , stencilOpFailBack_(other.stencilOpFailBack_)
	{
	}

    MaterialRenderSettings::MaterialRenderSettings(MaterialRenderSettings&& other) noexcept
        : blendConstant_(std::move(other.blendConstant_))
        , mixMode_(other.mixMode_)
        , blendLogic_(other.blendLogic_)
        , blendSrcColor_(other.blendSrcColor_)
        , blendSrcAlpha_(other.blendSrcAlpha_)
        , blendDstColor_(other.blendDstColor_)
        , blendDstAlpha_(other.blendDstAlpha_)
        , blendEquationColor_(other.blendEquationColor_)
        , blendEquationAlpha_(other.blendEquationAlpha_)
        , faceCull_(other.faceCull_)
        , depthComp_(other.depthComp_)
        , depthTest_(other.depthTest_)
        , depthWrite_(other.depthWrite_)
        , colorMaskR_(other.colorMaskR_)
        , colorMaskG_(other.colorMaskG_)
        , colorMaskB_(other.colorMaskB_)
        , colorMaskA_(other.colorMaskA_)
        , stencilTest_(other.stencilTest_)
        , stencilWriteMaskFront_(other.stencilWriteMaskFront_)
        , stencilWriteMaskBack_(other.stencilWriteMaskBack_)
        , stencilCompMaskFront_(other.stencilCompMaskFront_)
        , stencilCompMaskBack_(other.stencilCompMaskBack_)
        , stencilCompRefFront_(other.stencilCompRefFront_)
        , stencilCompRefBack_(other.stencilCompRefBack_)
        , stencilCompFront_(other.stencilCompFront_)
        , stencilCompBack_(other.stencilCompBack_)
        , stencilOpPassFront_(other.stencilOpPassFront_)
        , stencilOpPassBack_(other.stencilOpPassBack_)
        , stencilOpDepthFailFront_(other.stencilOpDepthFailFront_)
        , stencilOpDepthFailBack_(other.stencilOpDepthFailBack_)
        , stencilOpFailFront_(other.stencilOpFailFront_)
        , stencilOpFailBack_(other.stencilOpFailBack_)
    {
    }

    MaterialRenderSettings& MaterialRenderSettings::operator=(const MaterialRenderSettings& other)
    {
        blendConstant_ = other.blendConstant_;
        mixMode_ = other.mixMode_;
        blendLogic_ = other.blendLogic_;
        blendSrcColor_ = other.blendSrcColor_;
        blendSrcAlpha_ = other.blendSrcAlpha_;
        blendDstColor_ = other.blendDstColor_;
        blendDstAlpha_ = other.blendDstAlpha_;
        blendEquationColor_ = other.blendEquationColor_;
        blendEquationAlpha_ = other.blendEquationAlpha_;
        faceCull_ = other.faceCull_;
        depthComp_ = other.depthComp_;
        depthTest_ = other.depthTest_;
        depthWrite_ = other.depthWrite_;
        colorMaskR_ = other.colorMaskR_;
        colorMaskG_ = other.colorMaskG_;
        colorMaskB_ = other.colorMaskB_;
        colorMaskA_ = other.colorMaskA_;
        stencilTest_ = other.stencilTest_;
        stencilWriteMaskFront_ = other.stencilWriteMaskFront_;
        stencilWriteMaskBack_ = other.stencilWriteMaskBack_;
        stencilCompMaskFront_ = other.stencilCompMaskFront_;
        stencilCompMaskBack_ = other.stencilCompMaskBack_;
        stencilCompRefFront_ = other.stencilCompRefFront_;
        stencilCompRefBack_ = other.stencilCompRefBack_;
        stencilCompFront_ = other.stencilCompFront_;
        stencilCompBack_ = other.stencilCompBack_;
        stencilOpPassFront_ = other.stencilOpPassFront_;
        stencilOpPassBack_ = other.stencilOpPassBack_;
        stencilOpDepthFailFront_ = other.stencilOpDepthFailFront_;
        stencilOpDepthFailBack_ = other.stencilOpDepthFailBack_;
        stencilOpFailFront_ = other.stencilOpFailFront_;
        stencilOpFailBack_ = other.stencilOpFailBack_;
        return *this;
    }

	//--------------------------------------------------------------------------

	MaterialRenderSettings::~MaterialRenderSettings(void)
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

#define TEMPLATE_TF(ret,name,end)\
    template ret MaterialRenderSettings::name<false>end;\
    template ret MaterialRenderSettings::name<true>end;\

#pragma region Public Functions

    void MaterialRenderSettings::Read(StreamIn& stream)
    {
        stream.TryReadEnum(ReadKeyFaceCull, ENUM_READ(FaceCull), faceCull_);
        stream.TryReadEnum(ReadKeyMixMode, ENUM_READ(MixMode), mixMode_);
        stream.TryReadValue(ReadKeyBlendConstant, blendConstant_);
#define READ_TRIO_ENUM(readkey, enumType, suffixPrimary, valuePrimary, suffixSecondary, valueSecondary)\
        if (stream.TryReadEnum(readkey, ENUM_READ(enumType), valuePrimary))\
        {\
            valueSecondary = valuePrimary;\
        }\
        stream.TryReadEnum(readkey##suffixPrimary, ENUM_READ(enumType), valuePrimary);\
        stream.TryReadEnum(readkey##suffixSecondary, ENUM_READ(enumType), valueSecondary);\

#define READ_TRIO(readkey, suffixPrimary, valuePrimary, suffixSecondary, valueSecondary)\
        if (stream.TryReadValue(readkey, valuePrimary))\
        {\
            valueSecondary = valuePrimary;\
        }\
        stream.TryReadValue(readkey##suffixPrimary, valuePrimary);\
        stream.TryReadValue(readkey##suffixSecondary,valueSecondary);\

#define READ_TRIO_ENUM_BLEND(readkey, enumType, valueName) READ_TRIO_ENUM(ReadKeyBlend##readkey, enumType, Color, blend##valueName##Color_, Alpha, blend##valueName##Alpha_)

        READ_TRIO_ENUM_BLEND(Source, BlendFactor, Src);
        READ_TRIO_ENUM_BLEND(Destination, BlendFactor, Dst);
        READ_TRIO_ENUM_BLEND(Equation, BlendEquation, Equation);
        BVec4 colorMask;
        if (stream.TryReadValue(ReadKeyColorMask, colorMask))
        {
            SetColorMask(colorMask);
        }
        stream.TryReadEnum(ReadKeyDepthComparison, ENUM_READ(MixMode), mixMode_);
        stream.TryReadValue(ReadKeyDepthTest, depthTest_);
        stream.TryReadValue(ReadKeyDepthWrite, depthWrite_);

#define READ_TRIO_STENCIL(readkey, valueName) READ_TRIO(ReadKeyStencil##readkey, Front, stencil##valueName##Front_, Back, stencil##valueName##Back_)
#define READ_TRIO_ENUM_STENCIL(readkey, enumType, valueName) READ_TRIO_ENUM(ReadKeyStencil##readkey, enumType, Front, stencil##valueName##Front_, Back, stencil##valueName##Back_)

        stream.TryReadValue(ReadKeyStencilTest, stencilTest_);
        READ_TRIO_STENCIL(WriteMask, WriteMask);
        READ_TRIO_STENCIL(CompareMask, CompMask);
        READ_TRIO_STENCIL(CompareReferenceValue, CompRef);
        READ_TRIO_ENUM_STENCIL(Comparison, Comparison, Comp);
        READ_TRIO_ENUM_STENCIL(OperationPass, StencilOperation, OpPass);
        READ_TRIO_ENUM_STENCIL(OperationDepthFail, StencilOperation, OpDepthFail);
        READ_TRIO_ENUM_STENCIL(OperationFail, StencilOperation, OpFail);
    }

    MaterialRenderSettings::FaceCull MaterialRenderSettings::GetFaceCull() const
    {
        return faceCull_;
    }

    void MaterialRenderSettings::SetFaceCull(FaceCull faceCull)
    {
        faceCull_ = faceCull;
    }

    BVec4 MaterialRenderSettings::GetColorMask() const
    {
        return BVec4(colorMaskR_, colorMaskG_, colorMaskB_, colorMaskA_);
    }

    void MaterialRenderSettings::SetColorMask(const BVec4& mask)
    {
        colorMaskR_ = mask.r;
        colorMaskG_ = mask.g;
        colorMaskB_ = mask.b;
        colorMaskA_ = mask.a;
    }

#pragma region Mixing/Blending

    MaterialRenderSettings::MixMode MaterialRenderSettings::GetMixMode() const
    {
        return mixMode_;
    }

    void MaterialRenderSettings::SetMixMode(MixMode blendMode)
    {
        mixMode_ = blendMode;
    }

    template<bool alpha>
    MaterialRenderSettings::BlendFactor MaterialRenderSettings::GetBlendSourceFactor(void) const
    {
        return alpha ? blendSrcAlpha_ : blendSrcColor_;
    }
    TEMPLATE_TF(MaterialRenderSettings::BlendFactor, GetBlendSourceFactor, () const);

    template<bool alpha>
    void MaterialRenderSettings::SetBlendSourceFactor(BlendFactor blendFactor)
    {
        (alpha ? blendSrcAlpha_ : blendSrcColor_) = blendFactor;
    }
    TEMPLATE_TF(void, SetBlendSourceFactor, (BlendFactor));

    template<bool alpha>
    MaterialRenderSettings::BlendFactor MaterialRenderSettings::GetBlendDestinationFactor() const
    {
        return alpha ? blendDstAlpha_ : blendDstColor_;
    }
    TEMPLATE_TF(MaterialRenderSettings::BlendFactor, GetBlendDestinationFactor, () const);

    template<bool alpha>
    void MaterialRenderSettings::SetBlendDestinationFactor(BlendFactor blendFactor)
    {
        (alpha ? blendDstAlpha_ : blendDstColor_) = blendFactor;
    }
    TEMPLATE_TF(void, SetBlendDestinationFactor, (BlendFactor));

    template<bool alpha>
    MaterialRenderSettings::BlendEquation MaterialRenderSettings::GetBlendEquation() const
    {
        return alpha ? blendEquationAlpha_ : blendEquationColor_;
    }
    TEMPLATE_TF(MaterialRenderSettings::BlendEquation, GetBlendEquation, () const);

    template<bool alpha>
    void MaterialRenderSettings::SetBlendEquation(BlendEquation blendEquation)
    {
        (alpha ? blendEquationAlpha_ : blendEquationColor_) = blendEquation;
    }
    TEMPLATE_TF(void, SetBlendEquation, (BlendEquation));

    const Vec4& MaterialRenderSettings::GetBlendConstant() const
    {
        return blendConstant_;
    }

    void MaterialRenderSettings::SetBlendConstant(const Vec4& blendConstant)
    {
        blendConstant_ = blendConstant;
    }

    MaterialRenderSettings::LogicOperation MaterialRenderSettings::GetLogicOperation() const
    {
        return blendLogic_;
    }

    void MaterialRenderSettings::SetLogicOperation(LogicOperation logicOperation)
    {
        blendLogic_ = logicOperation;
    }

#pragma endregion Mixing/Blending

#pragma region Depth

    bool MaterialRenderSettings::GetDepthTestEnable(void) const
    {
        return depthTest_;
    }

    void MaterialRenderSettings::SetDepthTestEnable(bool depthTestEnable)
    {
        depthTest_ = depthTestEnable;
    }

    bool MaterialRenderSettings::GetDepthWriteEnable(void) const
    {
        return depthWrite_;
    }

    void MaterialRenderSettings::SetDepthWriteEnable(bool depthWriteEnable)
    {
        depthWrite_ = depthWriteEnable;
    }

    MaterialRenderSettings::Comparison MaterialRenderSettings::GetDepthComparison(void) const
    {
        return depthComp_;
    }

    void MaterialRenderSettings::SetDepthComparison(Comparison depthComparison)
    {
        depthComp_ = depthComparison;
    }

#pragma endregion Depth

#pragma region Stencil

    bool MaterialRenderSettings::GetStencilTestEnable(void) const
    {
        return stencilTest_;
    }

    void MaterialRenderSettings::SetStencilTestEnable(bool stencilTestEnable)
    {
        stencilTest_ = stencilTestEnable;
    }

    template<bool back>
    MaterialRenderSettings::StencilMask_t MaterialRenderSettings::GetStencilWriteMask(void) const
    {
        return back ? stencilWriteMaskBack_ : stencilWriteMaskFront_;
    }
    TEMPLATE_TF(MaterialRenderSettings::StencilMask_t, GetStencilWriteMask, () const);

    template<bool back>
    void MaterialRenderSettings::SetStencilWriteMask(StencilMask_t mask)
    {
        (back ? stencilWriteMaskBack_ : stencilWriteMaskFront_) = mask;
    }
    TEMPLATE_TF(void, SetStencilWriteMask, (StencilMask_t));

    template<bool back>
    MaterialRenderSettings::StencilMask_t MaterialRenderSettings::GetStencilComparisonMask(void) const
    {
        return back ? stencilCompMaskBack_ : stencilCompMaskFront_;
    }
    TEMPLATE_TF(MaterialRenderSettings::StencilMask_t, GetStencilComparisonMask, () const);

    template<bool back>
    void MaterialRenderSettings::SetStencilComparisonMask(StencilMask_t mask)
    {
        (back ? stencilCompMaskBack_ : stencilCompMaskFront_) = mask;
    }
    TEMPLATE_TF(void, SetStencilComparisonMask, (StencilMask_t));

    template<bool back>
    MaterialRenderSettings::StencilMask_t MaterialRenderSettings::GetStencilComparisonReferenceValue(void) const
    {
        return back ? stencilCompRefBack_ : stencilCompRefFront_;
    }
    TEMPLATE_TF(MaterialRenderSettings::StencilMask_t, GetStencilComparisonReferenceValue, () const);

    template<bool back>
    void MaterialRenderSettings::SetStencilComparisonReferenceValue(StencilMask_t value)
    {
        (back ? stencilCompRefBack_ : stencilCompRefFront_) = value;
    }
    TEMPLATE_TF(void, SetStencilComparisonReferenceValue, (StencilMask_t));

    template<bool back>
    MaterialRenderSettings::Comparison MaterialRenderSettings::GetStencilComparison(void) const
    {
        return back ? stencilCompBack_ : stencilCompFront_;
    }
    TEMPLATE_TF(MaterialRenderSettings::Comparison, GetStencilComparison, () const);

    template<bool back>
    void MaterialRenderSettings::SetStencilComparison(Comparison comparison)
    {
        (back ? stencilCompBack_ : stencilCompFront_) = comparison;
    }
    TEMPLATE_TF(void, SetStencilComparison, (Comparison));

    template<bool back>
    MaterialRenderSettings::StencilOperation MaterialRenderSettings::GetStencilOperationPass(void) const
    {
        return back ? stencilOpPassBack_ : stencilOpPassFront_;
    }
    TEMPLATE_TF(MaterialRenderSettings::StencilOperation, GetStencilOperationPass, () const);

    template<bool back>
    void MaterialRenderSettings::SetStencilOperationPass(StencilOperation stencilOperation)
    {
        (back ? stencilOpPassBack_ : stencilOpPassFront_) = stencilOperation;
    }
    TEMPLATE_TF(void, SetStencilOperationPass, (StencilOperation));

    template<bool back>
    MaterialRenderSettings::StencilOperation MaterialRenderSettings::GetStencilOperationDepthFail(void) const
    {
        return back ? stencilOpDepthFailBack_ : stencilOpDepthFailFront_;
    }
    TEMPLATE_TF(MaterialRenderSettings::StencilOperation, GetStencilOperationDepthFail, () const);

    template<bool back>
    void MaterialRenderSettings::SetStencilOperationDepthFail(StencilOperation stencilOperation)
    {
        (back ? stencilOpDepthFailBack_ : stencilOpDepthFailFront_) = stencilOperation;
    }
    TEMPLATE_TF(void, SetStencilOperationDepthFail, (StencilOperation));

    template<bool back>
    MaterialRenderSettings::StencilOperation MaterialRenderSettings::GetStencilOperationFail(void) const
    {
        return back ? stencilOpFailBack_ : stencilOpFailFront_;
    }
    TEMPLATE_TF(MaterialRenderSettings::StencilOperation, GetStencilOperationFail, () const);

    template<bool back>
    void MaterialRenderSettings::SetStencilOperationFail(StencilOperation stencilOperation)
    {
        (back ? stencilOpFailBack_ : stencilOpFailFront_) = stencilOperation;
    }
    TEMPLATE_TF(void, SetStencilOperationFail, (StencilOperation));

#pragma endregion Stencil

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

    void MaterialRenderSettings::SetRenderData() const
    {
#define GetGL(valueArr, value) valueArr[(unsigned)value]

        // Culling
        if (faceCull_ == FaceCull::None)
        {
            glDisable(GL_CULL_FACE);
            OpenGLCheckError();
        }
        else
        {
            glEnable(GL_CULL_FACE);
            OpenGLCheckError();
            glCullFace(GetGL(FaceCullValues, faceCull_));
            OpenGLCheckError();
        }

        // Stencil
        if (stencilTest_)
        {
            glEnable(GL_STENCIL_TEST);
            OpenGLCheckError();
            // Front-facing polygons (and non-polygon primitives)
            glStencilMaskSeparate(GL_FRONT, stencilWriteMaskFront_);
            OpenGLCheckError();
            glStencilFuncSeparate(GL_FRONT, GetGL(ComparisonValues, stencilCompFront_), (GLint)stencilCompRefFront_, stencilCompMaskFront_);
            OpenGLCheckError();
            glStencilOpSeparate(GL_FRONT, GetGL(StencilOpValues, stencilOpFailFront_), GetGL(StencilOpValues, stencilOpDepthFailFront_), GetGL(StencilOpValues, stencilOpPassFront_));
            OpenGLCheckError();
            // Back-facing polygons
            glStencilMaskSeparate(GL_BACK, stencilWriteMaskBack_);
            OpenGLCheckError();
            glStencilFuncSeparate(GL_BACK, GetGL(ComparisonValues, stencilCompBack_), (GLint)stencilCompRefBack_, stencilCompMaskBack_);
            OpenGLCheckError();
            glStencilOpSeparate(GL_BACK, GetGL(StencilOpValues, stencilOpFailBack_), GetGL(StencilOpValues, stencilOpDepthFailBack_), GetGL(StencilOpValues, stencilOpPassBack_));
            OpenGLCheckError();
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
            OpenGLCheckError();
        }

        // Depth
        if (depthTest_)
        {
            glEnable(GL_DEPTH_TEST);
            OpenGLCheckError();
            glDepthFunc(GetGL(ComparisonValues, depthComp_));
            OpenGLCheckError();
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
            OpenGLCheckError();
#ifdef _DEBUG
            if (depthWrite_)
            {
                TraceWarning("Depth writing is enabled, but has no effect when depth comparison is disabled!");
            }
#endif
        }
        glDepthMask(depthWrite_ ? GL_TRUE : GL_FALSE);
        OpenGLCheckError();

        // Mixing
        switch (mixMode_)
        {
        case MixMode::None:
        {
            glDisable(GL_BLEND);
            OpenGLCheckError();
            glDisable(GL_COLOR_LOGIC_OP);
            OpenGLCheckError();
        }
        break;
        case MixMode::Blend:
        {
            glEnable(GL_BLEND);
            OpenGLCheckError();
            glDisable(GL_COLOR_LOGIC_OP);
            OpenGLCheckError();
            glBlendColor(blendConstant_.r, blendConstant_.g, blendConstant_.b, blendConstant_.a);
            OpenGLCheckError();
            glBlendFuncSeparate(
                GetGL(BlendFactorValues, blendSrcColor_),
                GetGL(BlendFactorValues, blendDstColor_),
                GetGL(BlendFactorValues, blendSrcAlpha_),
                GetGL(BlendFactorValues, blendDstAlpha_));
            OpenGLCheckError();
            glBlendEquationSeparate(GetGL(BlendEquationValues, blendEquationColor_), GetGL(BlendEquationValues, blendEquationAlpha_));
            OpenGLCheckError();
        }
        break;
        case MixMode::Logic:
        {
            glDisable(GL_BLEND);
            OpenGLCheckError();
            glEnable(GL_COLOR_LOGIC_OP);
            OpenGLCheckError();
            glLogicOp(GetGL(BlendLogicValues, blendLogic_));
            OpenGLCheckError();
        }
        break;
        default:
            assert(false);
            break;
        }

        // Color mask
        glColorMask(
            colorMaskR_ ? GL_TRUE : GL_FALSE,
            colorMaskG_ ? GL_TRUE : GL_FALSE,
            colorMaskB_ ? GL_TRUE : GL_FALSE,
            colorMaskA_ ? GL_TRUE : GL_FALSE);
        OpenGLCheckError();
    }

#pragma endregion Private Functions

}	// namespace
