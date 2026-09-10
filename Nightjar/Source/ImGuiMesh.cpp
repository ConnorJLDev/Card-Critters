//------------------------------------------------------------------------------
//
// File Name:	ImGuiMesh.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Mesh.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "Mesh.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void Mesh::DisplayIMGUI()
	{
        bool immediate = immediateDrawType_ != DrawType::Invalid;
        if (ImGui::Checkbox("Immediate", &immediate) && immediate)
        {
            immediateDrawType_ = DrawType::Triangles;
        }
        if (immediate)
        {
            ImGui::CreateEnumSelector("Draw Type", immediateDrawType_, ENUM_WRITE(DrawType));
        }
        FORMAT_MAKE(headerAttrs, "Attributes ({})\0", attributes_.size());
        unsigned int attrCount = (unsigned int)attributes_.size();
        constexpr ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;
        if (ImGui::CollapsingHeader(headerAttrs.c_str()))
        {
            if (ImGui::BeginTable("Attrs", (int)attrCount + 1, flags))
            {
                ImGui::TableSetupColumn("Vert", ImGuiTableColumnFlags_WidthStretch);
                for (unsigned attrIndex = 0; attrIndex < attrCount; ++attrIndex)
                {
                    ImGui::TableSetupColumn(ENUM_WRITE_VALUE(DataType, attributes_[attrIndex].dataType).data(), ImGuiTableColumnFlags_WidthStretch);
                    //ImGui::TableSetupColumn(ENUM_WRITE_VALUE(Attribute, attributes_[attrIndex].attribute).data(), ImGuiTableColumnFlags_WidthStretch);
                }
                ImGui::TableHeadersRow();
                ImGui::PushItemWidth(-1.0f);
                for (unsigned vertIndex = 0; vertIndex < vertexCount_; ++vertIndex)
                {
                    ImGui::PushID((int)vertIndex);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", vertIndex);
                    for (unsigned attrIndex = 0; attrIndex < attrCount; ++attrIndex)
                    {
                        ImGui::TableSetColumnIndex((int)attrIndex + 1);
                        attributes_[attrIndex].DisplayIMGUIElement(vertIndex);
                    }
                    ImGui::PopID();
                }
                ImGui::PopItemWidth();
                ImGui::EndTable();
            }
        }
        string_t headerPrimitives{};
        if (immediateDrawType_ == DrawType::Invalid)
        {
            FORMAT_TO(headerPrimitives, "Primitives ({})\0", primitives_.size());
        } 
        else
        {
            headerPrimitives = "Primitives (Immediate)\0";
        }
        if (ImGui::CollapsingHeader(headerPrimitives.c_str()))
        {
            //if (immediateDrawType_ != DrawType::Invalid)
            //{
            //    // Just show the sequence.
            //}
            //else
            {
                // Show primitives.
                for (unsigned i = 0; i < primitives_.size(); ++i)
                {
                    auto& primitive = primitives_[i];
                    ImGui::PushID((int)i);
                    FORMAT_MAKE(primitiveName, "{}: {} ({} verts)\0", i + 1, ENUM_WRITE_VALUE(DrawType, primitive.type), primitive.vertexCount);
                    if (ImGui::CollapsingHeader(primitiveName.c_str()))
                    {
                        if (ImGui::BeginTable("ParticleEmitProperties", (int)attrCount + 2, flags))
                        {
                            ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthStretch);
                            ImGui::TableSetupColumn("Vert", ImGuiTableColumnFlags_WidthStretch);
                            for (unsigned attrIndex = 0; attrIndex < attrCount; ++attrIndex)
                            {
                                ImGui::TableSetupColumn(ENUM_WRITE_VALUE(DataType, attributes_[attrIndex].dataType).data(), ImGuiTableColumnFlags_WidthStretch);
                            }
                            ImGui::TableHeadersRow();
                            ImGui::PushItemWidth(-1.0f);
                            for (unsigned primVertIndex = 0; primVertIndex < primitive.vertexCount; ++primVertIndex)
                            {
                                ImGui::PushID((int)primVertIndex);
                                unsigned& vertIndex = primitive.indices[primVertIndex];
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::Text("%d", primVertIndex);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::InputInt("##Index", reinterpret_cast<int*>(&vertIndex));
                                for (unsigned attrIndex = 0; attrIndex < attrCount; ++attrIndex)
                                {
                                    ImGui::TableSetColumnIndex((int)attrIndex + 2);
                                    attributes_[attrIndex].DisplayIMGUIElement(vertIndex);
                                }
                                ImGui::PopID();
                            }
                            ImGui::PopItemWidth();
                            ImGui::EndTable();
                        }
                    }
                    ImGui::PopID();
                }
            }
        }
	}

    bool Mesh::Attribute::DisplayIMGUIElement(size_t elementIndex)
    {
        ImGui::PushID(this);
        unsigned typeSize = (unsigned)DataTypeInfos[(unsigned)dataType].size * dataCount;
        size_t dataIndex = elementIndex * typeSize;
        char* indexPtr = &reinterpret_cast<char*>(data)[dataIndex];
        bool success;
#define INVALID_IMGUI_TEXT ImGui::Text("Error type: %s %d", ENUM_WRITE_VALUE(DataType, dataType), dataCount); success = false;
        switch (dataType)
        {
        case DataType::Int:
        case DataType::UInt:
            switch (dataCount)
            {
            case 1:
                success = ImGui::InputInt("", reinterpret_cast<int*>(indexPtr));
                break;
            case 2:
                success = ImGui::InputInt2("", reinterpret_cast<int*>(indexPtr));
                break;
            case 3:
                success = ImGui::InputInt3("", reinterpret_cast<int*>(indexPtr));
                break;
            case 4:
                success = ImGui::InputInt4("", reinterpret_cast<int*>(indexPtr));
                break;
            default:
                INVALID_IMGUI_TEXT;
                break;
            }
            break;
        case DataType::Single:
            switch (dataCount)
            {
            case 1:
                success = ImGui::InputFloat("", reinterpret_cast<float*>(indexPtr));
                break;
            case 2:
                success = ImGui::InputFloat2("", reinterpret_cast<float*>(indexPtr));
                break;
            case 3:
                if (attribute == Attributes::Color0 || attribute == Attributes::Color1)
                {
                    success = ImGui::ColorEdit3("", reinterpret_cast<float*>(indexPtr));
                }
                else
                {
                    success = ImGui::InputFloat3("", reinterpret_cast<float*>(indexPtr));
                }
                break;
            case 4:
                if (attribute == Attributes::Color0 || attribute == Attributes::Color1)
                {
                    success = ImGui::ColorEdit4("", reinterpret_cast<float*>(indexPtr));
                }
                else
                {
                    success = ImGui::InputFloat4("", reinterpret_cast<float*>(indexPtr));
                }
                break;
            default:
                INVALID_IMGUI_TEXT;
                break;
            }
            break;
        default:
            INVALID_IMGUI_TEXT;
            break;
        }
        ImGui::PopID();
        return success;
    }

#pragma endregion Public Functions

}	// namespace
