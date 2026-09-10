//------------------------------------------------------------------------------
//
// File Name:	ImGuiLightSource.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for LightSource.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "LightSource.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	bool LightSource::DisplayIMGUIIndex()
	{
		int newIndex = index_;
		bool changed = ImGui::SliderInt("Index", &newIndex, 0, 7);
		if (changed)
		{
			Vec4 oldColor = color_;
			color_ = Vec4(0, 0, 0, 0);
			UpdateGraphics();
			color_ = oldColor;
			index_ = newIndex;
		}
		return changed;
	}

	bool LightSource::DisplayIMGUIColor()
	{
		return ImGui::ColorEdit4("Color", &color_.x);
	}

	bool LightSource::DisplayIMGUITransform()
	{
		ImGui::Text("Translation: ");
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
		ImGui::SameLine();
		ImGui::Text("%.3f, %.3f, %.3f", pos_.x, pos_.y, pos_.z);
		ImGui::PopStyleColor();
		ImGui::Text("Normal: ");
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
		ImGui::SameLine();
		ImGui::Text("%.3f, %.3f, %.3f", normal_.x, normal_.y, normal_.z);
		ImGui::PopStyleColor();
		return false;
	}

	bool LightSource::DisplayIMGUIAttenuation()
	{
		ImGui::Text("Attenuation");
		// Distance
		bool changed = ImGui::DragFloat3("Distance", &attenuationDistance_.x, 0.1f, 0.0f, 0.0f, "%.3f");
		// Angle
		changed |= ImGui::DragFloat3("Angle", &attenuationAngle_.x, 0.1f, 0.0f, 0.0f, "%.3f");
		bool active = ImGui::CollapsingHeader("Angle Details");
		if (!active)
		{
			return changed;
		}
		static Vec3 anglesValue;
		static array_t<float, 91> angles;
		static DVec2 anglesVertex;
		static double anglesFalloff;
		static DVec2 anglesLinear;
		bool modified = false;
		constexpr float epsilon = 0.001f;
		if (glm::abs(attenuationAngle_.z) > epsilon)
		{
			Vec2 aV = (Vec2)anglesVertex;
			modified |= ImGui::DragFloat2("Curve Vertex", &aV.x, 0.01f, 0.0f, 0.0f, "%.3f");
			anglesVertex = aV;
			float aF = (float)anglesFalloff;
			modified |= ImGui::DragFloat("Curve Falloff", &aF, 1.0f, 0.0f, 0.0f, "%.3f");
			anglesFalloff = aF;
			if (modified)
			{
				attenuationAngle_.z = (float)-anglesFalloff;
				if (glm::abs(anglesFalloff) > epsilon)
				{
					attenuationAngle_.y = (float)(anglesVertex.x * 2 * anglesFalloff);
					attenuationAngle_.x = (float)(anglesVertex.y + (anglesVertex.x * anglesVertex.x) * -anglesFalloff);
				}
				changed = true;
			}
		}
		else
		{
			Vec2 aL = (Vec2)anglesLinear;
			modified |= ImGui::DragFloat2("Curve Endpoints", &aL.x, 0.01f, 0.0f, 0.0f, "%.3f");
			anglesLinear = aL;
			if (modified)
			{
				attenuationAngle_.z = 0.0f;
				if (glm::abs(anglesLinear.y - anglesLinear.x) > 0.00001)
				{
					double slope = 1 / (anglesLinear.y - anglesLinear.x);
					attenuationAngle_.y = (float)slope;
					attenuationAngle_.x = (float)(-anglesLinear.x * slope);
				}
				changed = true;
			}
		}
		if (anglesValue != attenuationAngle_)
		{
			anglesValue = attenuationAngle_;

			anglesFalloff = -attenuationAngle_.z;
			anglesVertex.x = attenuationAngle_.y / (-2 * attenuationAngle_.z);
			anglesVertex.y = attenuationAngle_.x - (anglesVertex.x * anglesVertex.x) * attenuationAngle_.z;

			anglesLinear.x = -attenuationAngle_.x / attenuationAngle_.y;
			anglesLinear.y = anglesLinear.x + 1 / attenuationAngle_.y;

			constexpr float angleStep = glm::radians(1.0f);
			for (unsigned i = 0; i < angles.size(); ++i)
			{
				const float angle = glm::cos(angleStep * (float)i);
				const float val = attenuationAngle_.x + angle * attenuationAngle_.y + angle * angle * attenuationAngle_.z;
				angles[i] = std::clamp(val, 0.0f, 1.0f);
			}
			changed = true;
		}
		ImGui::PlotLines("Angle Graph", angles.data(), (int)angles.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
		return changed;
	}

#pragma endregion Public Functions

}	// namespace
