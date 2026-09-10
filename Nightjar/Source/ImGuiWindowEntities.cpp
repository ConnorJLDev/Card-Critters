//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowEntities.cpp
// Author(s):	Jonathan Sandquist (jonathan.s), Barend Brink (barend.brink), Connor Lariviere (connor.lariviere)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays entities in each active scene.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowEntities.h"

#include "ImGuiInclude.h"

#include "ImGuiWindowBase.inl"

#include "SceneSystem.h"
#include "Scene.h"
#include "EntityContainer.h"
#include "Entity.h"
#include "Transform.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

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

    constexpr string_view_t ImGuiWindowEntities::WindowName = "Entities";

    static constexpr size_t InvalidIndex = static_cast<size_t>(-1);

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

    ImGuiWindowEntities::ImGuiWindowEntities(void)
        : ImGuiWindowBase(WindowName, 0/*ImGuiWindowFlags_MenuBar*/)
        , searchFilter_("")
        , visibleEntities_()
        , selectedEntityIndex_(InvalidIndex)
        , selectedEntityShiftIndex_(InvalidIndex)
        , selectedEntityState_()
    {
    }

    ImGuiWindowEntities::ImGuiWindowEntities(ImGuiWindowEntities&& other) noexcept
        : ImGuiWindowBase(std::move(other))
        , searchFilter_(std::move(other.searchFilter_))
        , visibleEntities_(std::move(other.visibleEntities_))
        , selectedEntityIndex_(std::move(other.selectedEntityIndex_))
        , selectedEntityShiftIndex_(std::move(other.selectedEntityShiftIndex_))
        , selectedEntityState_(std::move(other.selectedEntityState_))
    {
    }

    //--------------------------------------------------------------------------

    ImGuiWindowEntities::~ImGuiWindowEntities(void)
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

    void ImGuiWindowEntities::DisplayImpl(void)
    {
        //DisplayMenuBar();

        DisplayScenes();
    }

    ImGuiWindowBase::ReadError ImGuiWindowEntities::ReadKeyValue([[maybe_unused]] string_view_arg key, [[maybe_unused]] string_view_arg value)
    {
        //bool parseSuccess;
        //switch (READKEY_HASH(key))
        //{
        //READKEY_CHECK("Dummy")
        //    READKEY_VALUE(dummyValue_);
        //    return READKEY_RETURN;
        //default:
        //    break;
        //}
        return ReadError::InvalidKey;
    }

    void ImGuiWindowEntities::ReadFinish(void)
    {
    }

    void ImGuiWindowEntities::WriteAll([[maybe_unused]] string_t& buffer)
    {
    }

    void ImGuiWindowEntities::DisplayMenuBar(void)
    {
        if (!ImGui::BeginMenuBar())
        {
            return;
        }

        if (ImGui::BeginMenu("Settings"))
        {


            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    void ImGuiWindowEntities::DisplayScenes(void)
    {
        searchFilter_.Draw("Search", 0.0f);

        static constexpr ImGuiTableFlags tblFlags =
            ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_RowBg;

        //if (!ImGui::BeginTable("##entities", 1, tblFlags))
        //{
        //    return;
        //}

        //ImGui::TableSetupScrollFreeze(0, 1);
        //static constexpr ImGuiTableFlags flagsColumn = ImGuiTableColumnFlags_WidthFixed;
        //ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);

        //ImGui::TableHeadersRow();

        const auto slotCount = SceneSystem::GetSceneSlotCount();
        for (unsigned i = 0; i < slotCount; ++i)
        {
            Scene* scene = SceneSystem::GetScene(i);
            if (scene != nullptr)
            {
                DisplayEntities(i);
                if (ImGui::Button("Save Changes To File"))
                {
                    scene->Write(scene->GetName());
                    TraceInfo("Scene {} Has Been Updated Successfully!", scene->GetName());
                }
            }
        }

        //ImGui::EndTable();
    }
    
    void ImGuiWindowEntities::DisplayEntities(unsigned sceneIndex)
    {
        Scene* scene = SceneSystem::GetScene(sceneIndex);
        assert(scene != nullptr);
        EntityContainer* container = scene->FindContainer();
        assert(container != nullptr);

        visibleEntities_.clear();
        visibleEntities_.reserve(std::bit_ceil(container->Count()));
        selectedEntityIndex_ = InvalidIndex;

        // If the search filter is empty, display in hierarchy mode.
        if (strnlen_s(searchFilter_.InputBuf, 1) <= 0)
        {
            for (unsigned i = 0; i < container->Count(); ++i)
            {
                // Only show the trees of entities that don't have parents, as those that do will be displayed by their parent.
                Entity* ent = container->GetFromIndex(i);
                Transform* tfm = ent->Has(Transform);
                if (tfm == nullptr || tfm->GetParentTransform() == nullptr)
                {
                    DisplayEntityChildren(ent);
                }
            }
        }
        else 
        {
            for (unsigned i = 0; i < container->Count(); ++i)
            {
                Entity* entity = container->GetFromIndex(i);
                const char* name = entity->GetName().c_str();
                if (!searchFilter_.PassFilter(name, name + entity->GetName().size()))
                {
                    continue;
                }

                //ImGui::TableNextColumn();

                ImGui::PushID(entity); // stable ID by pointer
                const bool entitySelected = PrepareEntitySelection(entity);

                if (ImGui::Selectable(name, entitySelected, ImGuiSelectableFlags_SpanAllColumns))
                {
                    if (selectedEntityIndex_ == InvalidIndex)
                    {
                        selectedEntityIndex_ = visibleEntities_.size() - 1;
                        selectedEntityState_ = entitySelected;
                    }
                }
                ImGui::PopID();
            }
        }

        if (selectedEntityIndex_ != InvalidIndex)
        {
            ProcessEntitySelection();
        }

        auto& ctx = ImGuiContextData::GetContext();
        const bool buttonsDisabled = ctx.selectedEntities_.empty();

        if (buttonsDisabled)
        {
            ImGui::BeginDisabled();
        }
        if (ImGui::Button("Remove"))
        {
            for (auto* entity : ctx.selectedEntities_)
            {
                entity->Destroy();
            }
            ctx.selectedEntities_.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button("Clone"))
        {
            vector_t<Entity*> entityClones{};
            for (auto* entity : ctx.selectedEntities_)
            {
                entityClones.emplace_back(entity->Clone());
            }
            ctx.selectedEntities_.clear();
            ctx.selectedEntities_.insert(entityClones.begin(), entityClones.end());
        }
        if (buttonsDisabled)
        {
            ImGui::EndDisabled();
        }
    }

    void ImGuiWindowEntities::DisplayEntityChildren(Entity* entity)
    {
        static constexpr ImGuiTreeNodeFlags nodeFlagsDefault{ ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth };
        ImGuiTreeNodeFlags flags{ nodeFlagsDefault };
        const Transform* tfm = entity->Has(Transform);

        const bool entitySelected = PrepareEntitySelection(entity);

        // Highlight selected entity.
        if (entitySelected)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        // Hide dropdown arrow if there are no children.
        if (tfm == nullptr || tfm->GetChildren().size() <= 0)
        {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        ImGui::PushID(entity);
        bool nodeOpen{ ImGui::TreeNodeEx(entity->GetName().c_str(), flags) };

        if (ImGui::IsItemToggledOpen())
        {
        }
        // If the node is clicked (but not the arrow to show children).
        else if (ImGui::IsItemClicked())
        {
            if (selectedEntityIndex_ == InvalidIndex)
            {
                selectedEntityIndex_ = visibleEntities_.size() - 1;
                selectedEntityState_ = entitySelected;
            }
        }

        // If the node is open, show the children and pop the node.
        if (nodeOpen)
        {
            if (tfm != nullptr)
            {
                for (const auto* child : tfm->GetChildren())
                {
                    DisplayEntityChildren(child->GetParent());
                }
            }
            ImGui::TreePop();
        }

        ImGui::PopID();
    }

    void ImGuiWindowEntities::ProcessEntitySelection(void)
    {
        auto& ctx = ImGuiContextData::GetContext();
        const auto& io = ImGui::GetIO();
        Entity* entity = visibleEntities_[selectedEntityIndex_];
        ctx.selectedEntity_ = entity;

        if (io.KeyShift && selectedEntityShiftIndex_ != InvalidIndex)
        {
            if (!(io.KeyCtrl || io.KeyAlt))
            {
                ctx.selectedEntities_.clear();
            }
            const auto indexMin = static_cast<long long>(MathMin(selectedEntityShiftIndex_, selectedEntityIndex_));
            const auto indexMax = static_cast<long long>(MathMax(selectedEntityShiftIndex_, selectedEntityIndex_) + 1);
            const auto iterEnd = visibleEntities_.cbegin() + indexMax;
            if (io.KeyAlt)
            {
                for (auto iter = visibleEntities_.cbegin() + indexMin; iter != iterEnd; ++iter)
                {
                    ctx.selectedEntities_.erase(*iter);
                }
            }
            else
            {
                ctx.selectedEntities_.insert(visibleEntities_.cbegin() + indexMin, iterEnd);
            }
        }
        else if (io.KeyCtrl)
        {
            selectedEntityShiftIndex_ = selectedEntityIndex_;
            if (selectedEntityState_)
            {
                ctx.selectedEntities_.erase(entity);
            }
            else
            {
                ctx.selectedEntities_.insert(entity);
            }
        }
        else if (io.KeyAlt)
        {
            selectedEntityShiftIndex_ = selectedEntityIndex_;
            ctx.selectedEntities_.erase(entity);
        }
        else
        {
            ctx.selectedEntities_.clear();
            ctx.selectedEntities_.insert(entity);
            selectedEntityShiftIndex_ = selectedEntityIndex_;
        }
        if (ctx.selectedEntities_.empty())
        {
            ctx.selectedEntity_ = nullptr;
            selectedEntityShiftIndex_ = InvalidIndex;
        }
    }

    bool ImGuiWindowEntities::PrepareEntitySelection(Entity* entity)
    {
        auto& ctx = ImGuiContextData::GetContext();
        visibleEntities_.push_back(entity);
        return ctx.selectedEntities_.contains(entity);
    }

#pragma endregion Private Functions

}	// namespace
