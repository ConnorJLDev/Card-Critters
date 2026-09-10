//------------------------------------------------------------------------------
//
// File Name:	ImGuiTexture.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Texture.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "GraphicsSystem.h"
#include GRAPHICS_INCLUDE

#include "Texture.h"
#include "Sampler.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

#define dimensions3_ baseMap_.dimensions3
#define dimensions2_ baseMap_.dimensions2
#define dimensions1_ baseMap_.dimensions1
#define data_ baseMap_.data

    STATIC_VECTOR((std::pair<ResourceID, int>), DrawCallbacks);

    static void AddRenderCall(ResourceID id, int level)
    {
        DrawCallbacks().emplace_back(id, level);
        static size_t CallbackIndex{ 0 };
        CallbackIndex = 0;
        ImGui::GetWindowDrawList()->AddCallback([](const ImDrawList* parent_list, const ImDrawCmd* cmd)
        {
            UNREFERENCED_PARAMETER(parent_list);
            UNREFERENCED_PARAMETER(cmd);
            assert(CallbackIndex < DrawCallbacks().size());
            glBindTexture(GL_TEXTURE_2D, DrawCallbacks()[CallbackIndex].first);
            OpenGLCheckError();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, DrawCallbacks()[CallbackIndex].second);
            OpenGLCheckError();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, DrawCallbacks()[CallbackIndex].second);
            OpenGLCheckError();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            OpenGLCheckError();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            OpenGLCheckError();
            if (++CallbackIndex >= DrawCallbacks().size())
            {
                DrawCallbacks().clear();
            }
        }, nullptr);
    }

    struct DrawLOD
    {
        ResourceID id;
        Vec3 lods;
        ResourceEnum min, mag;
        array_t<ResourceEnum, 4> swizzle;
    };

    STATIC_VECTOR((DrawLOD), DrawLODCallbacks);

    static void AddRenderCallLOD(ResourceID id, Vec3 lods, ResourceEnum min, ResourceEnum mag, array_t<ResourceEnum, 4> swizzle)
    {
        DrawLODCallbacks().emplace_back(id, lods, min, mag, swizzle);
        static size_t CallbackIndex{ 0 };
        CallbackIndex = 0;
        ImGui::GetWindowDrawList()->AddCallback([](const ImDrawList* parent_list, const ImDrawCmd* cmd)
        {
            UNREFERENCED_PARAMETER(parent_list);
            UNREFERENCED_PARAMETER(cmd);
            assert(CallbackIndex < DrawLODCallbacks().size());
            const auto& lodData = DrawLODCallbacks()[CallbackIndex];
            glBindTexture(GL_TEXTURE_2D, lodData.id);
            OpenGLCheckError();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)lodData.min);
            OpenGLCheckError();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)lodData.mag);
            OpenGLCheckError();
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, (GLfloat)lodData.lods.x);
            OpenGLCheckError();
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, (GLfloat)lodData.lods.y);
            OpenGLCheckError();
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, (GLfloat)lodData.lods.z);
            OpenGLCheckError();
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, reinterpret_cast<const GLint*>(lodData.swizzle.data()));
            OpenGLCheckError();
            if (++CallbackIndex >= DrawLODCallbacks().size())
            {
                DrawLODCallbacks().clear();
            }
        }, nullptr);
    }

	void Texture::DisplayIMGUI()
	{
        ImGui::Text("Size: ");
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
        ImGui::SameLine();
        ImGui::Text("%d x %d", dimensions3_.x, dimensions3_.y);
        ImGui::PopStyleColor();

        static int ViewLevel = -1;
        static float ViewFactor = 1.0f;
        static const Texture* LastTex = nullptr;
        static bool ScaleView = false, ShowAll = false, RelativeScale = true, LODView = false, ChannelView = false;
        ImGui::Checkbox("Scaled View", &ScaleView);
        ImGui::Checkbox("Show All", &ShowAll);
        ImGui::Checkbox("Relative Scale", &RelativeScale);
        ImGui::Checkbox("LOD View", &LODView);
        ImGui::Checkbox("Channel View", &ChannelView);
        static Sampler::FilterMinification lodMin = Sampler::FilterMinification::Nearest;
        static Sampler::FilterMagnification lodMag = Sampler::FilterMagnification::Nearest;
        static Vec3 lodVals(-1000.0f, 1000.0f, 0.0f);
        Vec2 size = (Vec2)dimensions2_;
        if (RelativeScale)
        {
            float contentWidth = ImGui::GetContentRegionAvail().x;
            size.y *= contentWidth / size.x;
            size.x = contentWidth;
        }
        if (LODView)
        {
            ImGui::CreateEnumSelector("Min", lodMin, ENUM_WRITE(Sampler::FilterMinification));
            ImGui::CreateEnumSelector("Mag", lodMag, ENUM_WRITE(Sampler::FilterMagnification));
            ImGui::DragFloat3("LodValues", &lodVals.x, 0.1f);
            auto lodMinRaw = Sampler::FilterMinificationValues[(unsigned int)lodMin], lodMagRaw = Sampler::FilterMagnificationValues[(unsigned int)lodMag];
            array_t<ResourceEnum, 4> swizzle;
            swizzle = 
            {
                GL_RED,
                GL_GREEN,
                GL_BLUE,
                GL_ALPHA,
            };
            AddRenderCallLOD(id_, lodVals, lodMinRaw, lodMagRaw, swizzle);
            ImGui::Image(this, size);
            if (ChannelView)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                size *= 0.5f;
                swizzle =
                {
                    GL_RED,
                    GL_RED,
                    GL_RED,
                    GL_ONE,
                };
                AddRenderCallLOD(id_, lodVals, lodMinRaw, lodMagRaw, swizzle);
                ImGui::Image(this, size);
                ImGui::SameLine();
                swizzle =
                {
                    GL_GREEN,
                    GL_GREEN,
                    GL_GREEN,
                    GL_ONE,
                };
                AddRenderCallLOD(id_, lodVals, lodMinRaw, lodMagRaw, swizzle);
                ImGui::Image(this, size);
                swizzle =
                {
                    GL_BLUE,
                    GL_BLUE,
                    GL_BLUE,
                    GL_ONE,
                };
                AddRenderCallLOD(id_, lodVals, lodMinRaw, lodMagRaw, swizzle);
                ImGui::Image(this, size);
                ImGui::SameLine();
                swizzle =
                {
                    GL_ALPHA,
                    GL_ALPHA,
                    GL_ALPHA,
                    GL_ONE,
                };
                AddRenderCallLOD(id_, lodVals, lodMinRaw, lodMagRaw, swizzle);
                ImGui::Image(this, size);
                ImGui::PopStyleVar();
            }
            return;
        }
        if (LastTex != this)
        {
            ViewLevel = -1;
            ViewFactor = 1.0f;
            LastTex = this;
        }
        if (!ShowAll)
        {
            if (ImGui::SliderInt("Mipmap", &ViewLevel, -1, mipmapMax_))
            {
                ViewFactor = ViewLevel >= mipmapMin_ ? 1.0f / (float)(1 << (ViewLevel - mipmapMin_)) : 1.0f;
            }
        }
        BindBuffers();

        if (ShowAll)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            for (int level = mipmapMin_; level <= mipmapMax_; ++level)
            {
                AddRenderCall(id_, level);
                Vec2 scaledSize = size;
                if (ScaleView)
                {
                    scaledSize *= 1.0f / (float)(1 << (level - mipmapMin_));
                    if (level > mipmapMin_ + 1)
                    {
                        ImGui::SameLine();
                    }
                }
                ImGui::Image(this, scaledSize);
            }
            ImGui::PopStyleVar();
            return;
        }
        else if (ViewLevel >= mipmapMin_)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, ViewLevel);
            OpenGLCheckError();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, ViewLevel);
            OpenGLCheckError();

            ImGui::Text("Mipmap Size: ");
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
            ImGui::SameLine();
            UVec3 mipmapSize;
            if (ViewLevel == mipmapMin_)
            {
                mipmapSize = dimensions3_;
            }
            else
            {
                mipmapSize = mipmaps_[(size_t)ViewLevel - mipmapMin_ - 1].dimensions3;
            }
            ImGui::Text("%d x %d", mipmapSize.x, mipmapSize.y);
            ImGui::PopStyleColor();
        }

        Vec2 scaledSize = size;
        if (ScaleView)
        {
            scaledSize *= ViewFactor;
        }

        if (ViewLevel >= mipmapMin_)
        {
            AddRenderCall(id_, ViewLevel);
        }

        ImGui::Image(this, scaledSize);
	}

#pragma endregion Public Functions

}	// namespace
