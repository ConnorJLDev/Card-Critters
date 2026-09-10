//------------------------------------------------------------------------------
//
// File Name:	ImGuiParticleEmitter.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for ParticleEmitter.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "ParticleEmitter.h"
#include "Particle.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void ParticleEmitter::DisplayIMGUI()
	{
		static int spawnCount = 10;
		constexpr ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;
		if (ImGui::BeginTable("ParticleEmitProperties", 3, flags))
		{
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Initial", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Random Range", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();
#define ROW_PROP_BASE(name, init, dim, suffix)\
			ImGui::TableNextRow();\
			ImGui::TableSetColumnIndex(0);\
			ImGui::Text(name);\
			ImGui::TableSetColumnIndex(1);\
			ImGui::PushItemWidth(-1.0f);\
			ImGui::DragFloat##dim("##"#init, &particleArchetype_.init##suffix, 0.1f, 0.0f, 0.0f, "%.2f");\
			ImGui::PopItemWidth();\
			ImGui::TableSetColumnIndex(2);\
			ImGui::PushItemWidth(-1.0f);\
			ImGui::DragFloat##dim("##"#init"Random", &particleArchetypeRandom_.init##suffix, 0.1f, 0.0f, 0.0f, "%.2f");\
			ImGui::PopItemWidth();
#define ROW_PROP_FLOAT(name, init) ROW_PROP_BASE(name, init, , )
#define ROW_PROP_VEC3(name, init) ROW_PROP_BASE(name, init, 3, .x)

			ROW_PROP_FLOAT("Lifetime", lifetimeRemaining_);
			ROW_PROP_VEC3("Translation", translation3_);
			ROW_PROP_VEC3("Rotation", rotation3_);
			ROW_PROP_VEC3("Scale", scale3_);
			ROW_PROP_VEC3("Velocity", velocity3_);
			ROW_PROP_VEC3("Acceleration", acceleration3_);
			ROW_PROP_VEC3("RotationalVelocity", rotationalVelocity3_);
			ROW_PROP_FLOAT("Drag", drag_);
			ROW_PROP_FLOAT("RotationalDrag", rotationalDrag_);

			ImGui::EndTable();
		}
		ImGui::DragFloat("Speed", &speed_, 0.01f, 0.0f, 20.0f, "%.2f");
		ImGui::Checkbox("Emission Interval Active", &emitIntervalActive_);
		if (ImGui::DragFloat("Emission Interval", &emitInterval_, 0.02f, 0.0f, 0.0f, "%.6f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat))
		{
			emitIntervalCurrent_ = emitInterval_;
		}
		ImGui::Text("Time until next particle emission: ");
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
		ImGui::SameLine();
		ImGui::Text("%.3f", emitIntervalCurrent_);
		ImGui::PopStyleColor();

		ImGui::SliderInt("Spawn Particles Count", &spawnCount, 1, 100);
		if (ImGui::Button("Spawn Particles"))
		{
			Emit((unsigned)spawnCount);
		}
		ImGui::Text("Particle Count: ");
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
		ImGui::SameLine();
		ImGui::Text("%d", particlesCount_);
		ImGui::PopStyleColor();
		bool active = ImGui::CollapsingHeader("Particle Transforms");
		if (active)
		{
			for (unsigned i = 0; i < particlesCount_; ++i)
			{
				ImGui::PushID((int)i);
				Particle& particle = operator[](i);
				ImGui::Text("%d", i);
				particle.TransformComponents::DisplayIMGUI3D();
				ImGui::PopID();
			}
		}
	}

#pragma endregion Public Functions

}	// namespace
