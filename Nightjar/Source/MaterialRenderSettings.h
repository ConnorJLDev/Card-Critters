//------------------------------------------------------------------------------
//
// File Name:	MaterialRenderSettings.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing render settings.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

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
    class MaterialRenderSettings
    {
        // Public Constants, Enums, and Structs:
    public:
        // Most of the enum comments are derived from OpenGL documentation (https://registry.khronos.org/OpenGL-Refpages/gl4/html/)

        // The smallest integer type that can represent the entire stencil mask.
        typedef uint8_t StencilMask_t;

        // Enum describing methods of culling polygon faces.
        enum class FaceCull : unsigned char
        {
            None,   // Do not cull either side of a polygon.
            Front,  // Cull the front of a polygon.
            Back,   // Cull the back of a polygon.
            All,    // Cull both sides of a polygon.
            NUM_ENTRIES,
        };

        // Enum describing methods of mixing between source (rendered) and destination (buffer).
        enum class MixMode : unsigned char
        {
            None,   // Do not perform mixing between source and destination.
            Blend,  // Perform arithmetic blending between source and destination.
            Logic,  // Perform a logic operation between source and destination.
            NUM_ENTRIES,
        };

        // Enum describing factors applied to source/destination when blended.
        enum class BlendFactor : unsigned char
        {
            // Factors are described as (R,G,B),A.
            // Rs, Gs, Bs, and As are the RGBA components of the source, respectively.
            // Rd, Gd, Bd, and Ad are the RGBA components of the destination, respectively.
            // Rc, Gc, Bc, and Ac are the RGBA components of the constant blend color, respectively.
            Zero,                       // (0,0,0),0
            One,                        // (1,1,1),1
            SourceColor,                // (Rs,Gs,Bs),As
            OneMinusSourceColor,        // (1-Rs,1-Gs,1-Bs),1-As
            DestinationColor,           // (Rd,Gd,Bd),Ad
            OneMinusDestinationColor,   // (1-Rd,1-Gd,1-Bd),1-Ad
            SourceAlpha,                // (As,As,As),As
            OneMinusSourceAlpha,        // (1-As,1-As,1-As),1-As
            DestinationAlpha,           // (Ad,Ad,Ad),Ad
            OneMinusDestinationAlpha,   // (1-Ad,1-Ad,1-Ad),1-Ad
            ConstantColor,              // (Rc,Gc,Bc),Ac
            OneMinusConstantColor,      // (1-Rc,1-Gc,1-Bc),1-Ac
            ConstantAlpha,              // (Ac,Ac,Ac),Ac
            OneMinusConstantAlpha,      // (1-Ac,1-Ac,1-Ac),1-Ac
            NUM_ENTRIES,
        };

        // Enum describing equations to use when blending source and destination.
        enum class BlendEquation : unsigned char
        {
            // Factors are described as (R,G,B),A.
            // Rs, Gs, Bs, and As are the RGBA components of the source, respectively.
            // Rsf, Gsf, Bsf, and Asf are the RGBA components of the source blend factor, respectively.
            // Rd, Gd, Bd, and Ad are the RGBA components of the destination, respectively.
            // Rdf, Gdf, Bdf, and Adf are the RGBA components of the destination blend factor, respectively.
            Add,                // (Rs*Rsf+Rd*Rdf,Gs*Gsf+Gd*Gdf,Bs*Bsf+Bd*Bdf),As*Asf+Ad*Adf
            Subtract,           // (Rs*Rsf-Rd*Rdf,Gs*Gsf-Gd*Gdf,Bs*Bsf-Bd*Bdf),As*Asf-Ad*Adf
            ReverseSubtract,    // (Rd*Rdf-Rs*Rsf,Gd*Gdf-Gs*Gsf,Bd*Bdf-Bs*Bsf),Ad*Adf-As*Asf
            Minimum,            // (min(Rs,Rd),min(Gs,Gd),min(Bs,Bd)),min(As,Ad)
            Maximum,            // (max(Rs,Rd),max(Gs,Gd),max(Bs,Bd)),max(As,Ad)
            NUM_ENTRIES,
        };

        // Enum describing logical operations applied to source/destination when logically mixed.
        enum class LogicOperation : unsigned char
        {
            // Formulas are described as logical operations between s (source) and d (destination).
            Clear,          // 0
            Set,            // 1
            Copy,           // s
            CopyInverted,   // ~s
            Noop,           // d
            Invert,         // ~d
            And,            // s&d
            Nand,           // ~(s&d)
            Or,             // s|d
            Nor,            // ~(s|d)
            Xor,            // s^d
            Equivalent,     // ~(s^d)
            AndReverse,     // s&~d
            AndInverted,    // ~s&d
            OrReverse,      // s|~d
            OrInverted,     // ~s|d
            NUM_ENTRIES,
        };

        // Enum describing comparison operations used between source/reference and destination for depth/stencil.
        enum class Comparison : unsigned char
        {
            Never,          // Never passes.
            LessOrEqual,    // Passes if the source/reference value is less than or equal to the destination value.
            Less,           // Passes if the source/reference value is less than the destination value.
            Equal,          // Passes if the source/reference value is equal to the destination value.
            NotEqual,       // Passes if the source/reference value is not equal to the destination value.
            GreaterOrEqual, // Passes if the source/reference value is greater than or equal to the destination value.
            Greater,        // Passes if the source/reference value is greater than the destination value.
            Always,         // Always passes.
            NUM_ENTRIES,
        };

        // Enum describing stencil buffer operations.
        enum class StencilOperation : unsigned char
        {
            Keep,           // Stencil buffer value is kept.
            Zero,           // Stencil buffer value set to zero.
            Replace,        // Stencil buffer value set to the stencil comparison reference value.
            Increment,      // Stencil buffer value incremented. Clamped to the maximum representable unsigned value.
            IncrementWrap,  // Stencil buffer value incremented. Wraps to zero if above the maximum representable unsigned value.
            Decrement,      // Stencil buffer value decremented. Clamped to 0.
            DecrementWrap,  // Stencil buffer value decremented. Wraps to the maximum representable unsigned value if below zero.
            Invert,         // Stencil buffer value bitwise inverted.
            NUM_ENTRIES,
        };

        // Constructors/Destructors:
    public:
        MaterialRenderSettings(void);

        MaterialRenderSettings(const MaterialRenderSettings& other);

        MaterialRenderSettings(MaterialRenderSettings&& other) noexcept;

        MaterialRenderSettings& operator=(const MaterialRenderSettings& other);

        ~MaterialRenderSettings(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Reads material render settings from a stream.
        /// </summary>
        /// <param name="stream">The stream to read from.</param>
        void Read(StreamIn& stream);

        /// <summary>
        /// Gets the state for face culling.
        /// </summary>
        /// <returns>The current state used for face culling.</returns>
        FaceCull GetFaceCull() const;

        /// <summary>
        /// Sets the state for face culling.
        /// </summary>
        /// <param name="faceCull">The state to set face culling to.</param>
        void SetFaceCull(FaceCull faceCull);

        /// <summary>
        /// Gets the state for color masking (which color channels can be written/rendered to).
        /// </summary>
        /// <returns>A vector of bools representing enabled/disabled (true/false) for red, green, blue, and alpha, respectively.</returns>
        BVec4 GetColorMask() const;

        /// <summary>
        /// Sets the state for color masking (which color channels can be written/rendered to).
        /// </summary>
        /// <param name="mask">A vector of bools for enabling/disabling red, green, blue, and alpha, respectively.</param>
        void SetColorMask(const BVec4& mask);

#pragma region Mixing/Blending

        /// <summary>
        /// Gets the state of how source (rendered) and destination (buffer) are combined and written to destination (buffer).
        /// </summary>
        /// <returns>The current mix state.</returns>
        MixMode GetMixMode(void) const;

        /// <summary>
        /// Sets the state of how source (rendered) and destination (buffer) are combined and written to destination (buffer).
        /// </summary>
        /// <param name="blendMode">The mix state to use.</param>
        void SetMixMode(MixMode blendMode);

        /// <summary>
        /// Gets the factor applied to the source when blending.
        /// </summary>
        /// <typeparam name="alpha">True to get the value for the alpha channel, false to get the value for the RGB channels.</typeparam>
        /// <returns>The current factor applied to the source.</returns>
        template<bool alpha = false>
        BlendFactor GetBlendSourceFactor(void) const;

        /// <summary>
        /// Sets the factor applied to the source when blending.
        /// </summary>
        /// <typeparam name="alpha">True to set the value for the alpha channel, false to set the value for the RGB channels.</typeparam>
        /// <param name="blendFactor">The factor to apply to the source.</param>
        template<bool alpha = false>
        void SetBlendSourceFactor(BlendFactor blendFactor);

        /// <summary>
        /// Gets the factor applied to the destination when blending.
        /// </summary>
        /// <typeparam name="alpha">True to get the value for the alpha channel, false to get the value for the RGB channels.</typeparam>
        /// <returns>The current factor applied to the destination.</returns>
        template<bool alpha = false>
        BlendFactor GetBlendDestinationFactor(void) const;

        /// <summary>
        /// Sets the factor applied to the destination when blending.
        /// </summary>
        /// <typeparam name="alpha">True to set the value for the alpha channel, false to set the value for the RGB channels.</typeparam>
        /// <param name="blendFactor">The factor to apply to the destination.</param>
        template<bool alpha = false>
        void SetBlendDestinationFactor(BlendFactor blendFactor);

        /// <summary>
        /// Gets the equation that combines the source and destination.
        /// </summary>
        /// <typeparam name="alpha">True to get the value for the alpha channel, false to get the value for the RGB channels.</typeparam>
        /// <returns>The current equation that combines the source and destination.</returns>
        template<bool alpha = false>
        BlendEquation GetBlendEquation(void) const;

        /// <summary>
        /// Sets the equation that combines the source and destination.
        /// </summary>
        /// <typeparam name="alpha">True to set the value for the alpha channel, false to set the value for the RGB channels.</typeparam>
        /// <param name="blendEquation">The equation to use when combining the source and destination.</param>
        template<bool alpha = false>
        void SetBlendEquation(BlendEquation blendEquation);

        /// <summary>
        /// Gets the constant color used when constant blend factors are used.
        /// </summary>
        /// <returns>The current constant blend color.</returns>
        const Vec4& GetBlendConstant(void) const;

        /// <summary>
        /// Sets the constant color used when constant blend factors are used.
        /// </summary>
        /// <param name="blendConstant">The color to set the constant blend factors to.</param>
        void SetBlendConstant(const Vec4& blendConstant);

        /// <summary>
        /// Gets the logic operation used when logically mixing source and destination.
        /// </summary>
        /// <returns>The current logic operation used.</returns>
        LogicOperation GetLogicOperation(void) const;

        /// <summary>
        /// Sets the logic operation used when logically mixing source and destination.
        /// </summary>
        /// <param name="logicOperation">The logic operation to use.</param>
        void SetLogicOperation(LogicOperation logicOperation);

#pragma endregion Mixing/Blending

#pragma region Depth

        /// <summary>
        /// Gets the state for depth-testing.
        /// </summary>
        /// <returns>True if depth-testing is enabled, false otherwise.</returns>
        bool GetDepthTestEnable(void) const;

        /// <summary>
        /// Sets the state for depth-testing.
        /// </summary>
        /// <param name="depthTestEnable">True to enable depth-testing, false to disable.</param>
        void SetDepthTestEnable(bool depthTestEnable);

        /// <summary>
        /// Gets the state for writing to the depth buffer.
        /// </summary>
        /// <returns>True if writing to the depth buffer is enabled, false otherwise.</returns>
        bool GetDepthWriteEnable(void) const;

        /// <summary>
        /// Sets the state for writing to the depth buffer.
        /// </summary>
        /// <param name="depthWriteEnable">True to enable writing to the depth buffer, false to disable.</param>
        void SetDepthWriteEnable(bool depthWriteEnable);

        /// <summary>
        /// Gets the comparison operation used for depth-testing.
        /// </summary>
        /// <returns>The comparison operation used.</returns>
        Comparison GetDepthComparison(void) const;

        /// <summary>
        /// Sets the comparison operation used for depth-testing.
        /// </summary>
        /// <param name="depthComparison">The comparison operation to use.</param>
        void SetDepthComparison(Comparison depthComparison);

#pragma endregion Depth

#pragma region Stencil

        /// <summary>
        /// Gets the state for stencil-testing.
        /// </summary>
        /// <returns>True if stencil-testing is enabled, false otherwise.</returns>
        bool GetStencilTestEnable(void) const;

        /// <summary>
        /// Sets the state for stencil-testing.
        /// </summary>
        /// <param name="stencilTestEnable">True to enable stencil-testing, false to disable.</param>
        void SetStencilTestEnable(bool stencilTestEnable);

        /// <summary>
        /// Gets the mask used when writing to the stencil buffer.
        /// </summary>
        /// <typeparam name="back">True to get the value for back-facing polygons, false to get the value for front-facing polygons (and other primitives).</typeparam>
        /// <returns>The current write mask used.</returns>
        template<bool back = false>
        StencilMask_t GetStencilWriteMask(void) const;

        /// <summary>
        /// Sets the mask used when writing to the stencil buffer.
        /// </summary>
        /// <typeparam name="back">True to set the value for back-facing polygons, false to set the value for front-facing polygons (and other primitives).</typeparam>
        /// <param name="mask">The write mask to use.</param>
        template<bool back = false>
        void SetStencilWriteMask(StencilMask_t mask);

        /// <summary>
        /// Gets the mask used when comparing the stencil reference value to the stencil buffer.
        /// </summary>
        /// <typeparam name="back">True to get the value for back-facing polygons, false to get the value for front-facing polygons (and other primitives).</typeparam>
        /// <returns>The current comparison mask used.</returns>
        template<bool back = false>
        StencilMask_t GetStencilComparisonMask(void) const;

        /// <summary>
        /// Sets the mask used when comparing the stencil reference value to the stencil buffer.
        /// </summary>
        /// <typeparam name="back">True to set the value for back-facing polygons, false to set the value for front-facing polygons (and other primitives).</typeparam>
        /// <param name="mask">The comparison mask to use.</param>
        template<bool back = false>
        void SetStencilComparisonMask(StencilMask_t mask);

        /// <summary>
        /// Gets the stencil reference value used when comparing to the stencil buffer.
        /// </summary>
        /// <typeparam name="back">True to get the value for back-facing polygons, false to get the value for front-facing polygons (and other primitives).</typeparam>
        /// <returns>The current reference value used.</returns>
        template<bool back = false>
        StencilMask_t GetStencilComparisonReferenceValue(void) const;

        /// <summary>
        /// Sets the stencil reference value used when comparing to the stencil buffer.
        /// </summary>
        /// <typeparam name="back">True to set the value for back-facing polygons, false to set the value for front-facing polygons (and other primitives).</typeparam>
        /// <param name="value">The reference value to use.</param>
        template<bool back = false>
        void SetStencilComparisonReferenceValue(StencilMask_t value);

        /// <summary>
        /// Gets the comparison operation used between the stencil reference value and the stencil buffer.
        /// </summary>
        /// <typeparam name="back">True to get the value for back-facing polygons, false to get the value for front-facing polygons (and other primitives).</typeparam>
        /// <returns>The current comparison operation used.</returns>
        template<bool back = false>
        Comparison GetStencilComparison(void) const;

        /// <summary>
        /// Sets the comparison operation used between the stencil reference value and the stencil buffer.
        /// </summary>
        /// <typeparam name="back">True to set the value for back-facing polygons, false to set the value for front-facing polygons (and other primitives).</typeparam>
        /// <param name="comparison">The comparison operation to use.</param>
        template<bool back = false>
        void SetStencilComparison(Comparison comparison);

        /// <summary>
        /// Gets the operation performed on the stencil buffer when rendering passes the stencil test (and depth-test, if applicable).
        /// </summary>
        /// <typeparam name="back">True to get the value for back-facing polygons, false to get the value for front-facing polygons (and other primitives).</typeparam>
        /// <returns>The current operation used.</returns>
        template<bool back = false>
        StencilOperation GetStencilOperationPass(void) const;

        /// <summary>
        /// Sets the operation performed on the stencil buffer when rendering passes the stencil test (and depth test, if applicable).
        /// </summary>
        /// <typeparam name="back">True to set the value for back-facing polygons, false to set the value for front-facing polygons (and other primitives).</typeparam>
        /// <param name="stencilOperation">The operation to use.</param>
        template<bool back = false>
        void SetStencilOperationPass(StencilOperation stencilOperation);

        /// <summary>
        /// Gets the operation performed on the stencil buffer when rendering passes the stencil test but fails the depth test.
        /// </summary>
        /// <typeparam name="back">True to get the value for back-facing polygons, false to get the value for front-facing polygons (and other primitives).</typeparam>
        /// <returns>The current operation used.</returns>
        template<bool back = false>
        StencilOperation GetStencilOperationDepthFail(void) const;

        /// <summary>
        /// Sets the operation performed on the stencil buffer when rendering passes the stencil test but fails the depth test.
        /// </summary>
        /// <typeparam name="back">True to set the value for back-facing polygons, false to set the value for front-facing polygons (and other primitives).</typeparam>
        /// <param name="stencilOperation">The operation to use.</param>
        template<bool back = false>
        void SetStencilOperationDepthFail(StencilOperation stencilOperation);

        /// <summary>
        /// Gets the operation performed on the stencil buffer when rendering fails the stencil test.
        /// </summary>
        /// <typeparam name="back">True to get the value for back-facing polygons, false to get the value for front-facing polygons (and other primitives).</typeparam>
        /// <returns>The current operation used.</returns>
        template<bool back = false>
        StencilOperation GetStencilOperationFail(void) const;

        /// <summary>
        /// Sets the operation performed on the stencil buffer when rendering fails the stencil test.
        /// </summary>
        /// <typeparam name="back">True to set the value for back-facing polygons, false to set the value for front-facing polygons (and other primitives).</typeparam>
        /// <param name="stencilOperation">The operation to use.</param>
        template<bool back = false>
        void SetStencilOperationFail(StencilOperation stencilOperation);

#pragma endregion Stencil

        // Public Event Handlers
    public:

        // Protected Constants, Enums, and Structs:
    protected:

#define READKEY_DUAL(name, first, second) static const StreamKey ReadKey##name, ReadKey##name##first, ReadKey##name##second
#define READKEY_COLOR_ALPHA(name) READKEY_DUAL(Blend##name, Color, Alpha)
#define READKEY_FRONT_BACK(name) READKEY_DUAL(Stencil##name, Front, Back)

        static const StreamKey ReadKeyFaceCull, ReadKeyMixMode, ReadKeyBlendConstant;
        READKEY_COLOR_ALPHA(Source);
        READKEY_COLOR_ALPHA(Destination);
        READKEY_COLOR_ALPHA(Equation);
        static const StreamKey ReadKeyLogicOperation;
        static const StreamKey ReadKeyDepthComparison, ReadKeyDepthTest, ReadKeyDepthWrite;
        static const StreamKey ReadKeyColorMask;
        static const StreamKey ReadKeyStencilTest;
        READKEY_FRONT_BACK(WriteMask);
        READKEY_FRONT_BACK(CompareMask);
        READKEY_FRONT_BACK(CompareReferenceValue);
        READKEY_FRONT_BACK(Comparison);
        READKEY_FRONT_BACK(OperationPass);
        READKEY_FRONT_BACK(OperationDepthFail);
        READKEY_FRONT_BACK(OperationFail);

#undef READKEY_FRONT_BACK
#undef READKEY_COLOR_ALPHA
#undef READKEY_DUAL

        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(FaceCull);
        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(MixMode);
        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(BlendFactor);
        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(BlendEquation);
        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(LogicOperation);
        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(Comparison);
        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(StencilOperation);

        // Protected Static Functions:
    protected:

        // Protected Functions:
    protected:
        /// <summary>
        /// Applies all render settings to the active rendering process.
        /// </summary>
        void SetRenderData(void) const;

        // Protected Static Variables:
    protected:

        // Protected Variables:
    protected:
        Vec4 blendConstant_;
        MixMode mixMode_;
        LogicOperation blendLogic_;
        BlendFactor blendSrcColor_, blendSrcAlpha_, blendDstColor_, blendDstAlpha_;
        BlendEquation blendEquationColor_, blendEquationAlpha_;

        FaceCull faceCull_;

        Comparison depthComp_;
        bool depthTest_;
        bool depthWrite_;

        bool colorMaskR_, colorMaskG_, colorMaskB_, colorMaskA_;

        bool stencilTest_;
        StencilMask_t stencilWriteMaskFront_, stencilWriteMaskBack_;
        StencilMask_t stencilCompMaskFront_, stencilCompMaskBack_;
        StencilMask_t stencilCompRefFront_, stencilCompRefBack_;
        Comparison stencilCompFront_, stencilCompBack_;
        StencilOperation stencilOpPassFront_, stencilOpPassBack_;
        StencilOperation stencilOpDepthFailFront_, stencilOpDepthFailBack_;
        StencilOperation stencilOpFailFront_, stencilOpFailBack_;
    };

}	// namespace
