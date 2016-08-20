/**
 * @file Blitter.cpp
 * @author Minmin Gong
 *
 * @section DESCRIPTION
 *
 * This source file is part of KlayGE
 * For the latest info, see http://www.klayge.org
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * You may alternatively use this source under the terms of
 * the KlayGE Proprietary License (KPL). You can obtained such a license
 * from http://www.klayge.org/licensing/.
 */

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/RenderEffect.hpp>

#include <KlayGE/Blitter.hpp>

namespace KlayGE
{
	Blitter::Blitter()
	{
		RenderFactory& rf = Context::Instance().RenderFactoryInstance();
		frame_buffer_ = rf.MakeFrameBuffer();
		rl_ = rf.RenderEngineInstance().PostProcessRenderLayout();

		effect_ = SyncLoadRenderEffect("Blitter.fxml");
		blit_point_2d_tech_ = effect_->TechniqueByName("BlitPoint2D");
		blit_point_2d_array_tech_ = effect_->TechniqueByName("BlitPoint2DArray");
		blit_linear_2d_tech_ = effect_->TechniqueByName("BlitLinear2D");
		blit_linear_2d_array_tech_ = effect_->TechniqueByName("BlitLinear2DArray");
		src_tex_param_ = effect_->ParameterByName("src_tex");
		src_tex_array_param_ = effect_->ParameterByName("src_tex_array");
		src_array_index_param_ = effect_->ParameterByName("src_array_index");
		src_level_param_ = effect_->ParameterByName("src_level");
		src_offset_param_ = effect_->ParameterByName("src_offset");
		src_scale_param_ = effect_->ParameterByName("src_scale");
		dst_offset_param_ = effect_->ParameterByName("dst_offset");
		dst_scale_param_ = effect_->ParameterByName("dst_scale");
	}

	void Blitter::Blit(TexturePtr const & dst, uint32_t dst_array_index, uint32_t dst_level,
		uint32_t dst_x_offset, uint32_t dst_y_offset, uint32_t dst_width, uint32_t dst_height,
		TexturePtr const & src, uint32_t src_array_index, uint32_t src_level,
		uint32_t src_x_offset, uint32_t src_y_offset, uint32_t src_width, uint32_t src_height,
		bool linear)
	{
		RenderFactory& rf = Context::Instance().RenderFactoryInstance();
		RenderEngine& re = rf.RenderEngineInstance();

		frame_buffer_->Attach(FrameBuffer::ATT_Color0, rf.Make2DRenderView(*dst, dst_array_index, 1, dst_level));

		uint32_t const src_w = src->Width(src_level);
		uint32_t const src_h = src->Height(src_level);
		uint32_t const dst_w = dst->Width(dst_level);
		uint32_t const dst_h = dst->Height(dst_level);

		*src_array_index_param_ = static_cast<int32_t>(src_array_index);
		*src_level_param_ = static_cast<int32_t>(src_level);
		*src_offset_param_ = float2(static_cast<float>(src_x_offset) / src_w, static_cast<float>(src_y_offset) / src_h);
		*src_scale_param_ = float2(static_cast<float>(src_width) / src_w, static_cast<float>(src_height) / src_h);

		float2 dst_offset(static_cast<float>(dst_x_offset) / dst_w, static_cast<float>(dst_y_offset) / dst_h);
		float2 dst_scale(static_cast<float>(dst_width) / dst_w, static_cast<float>(dst_height) / dst_h);
		*dst_offset_param_ = (dst_scale + dst_offset * 2 - 1) * float2(1, re.RequiresFlipping() ? -1.0f : +1.0f);
		*dst_scale_param_ = dst_scale;

		RenderTechnique* tech;
		if (src->ArraySize() > 1)
		{
			*src_tex_array_param_ = src;
			tech = linear ? blit_linear_2d_array_tech_ : blit_point_2d_array_tech_;
		}
		else
		{
			BOOST_ASSERT(0 == src_array_index);

			*src_tex_param_ = src;
			tech = linear ? blit_linear_2d_tech_ : blit_point_2d_tech_;
		}

		FrameBufferPtr curr_fb = re.CurFrameBuffer();
		re.BindFrameBuffer(frame_buffer_);
		re.Render(*effect_, *tech, *rl_);
		re.BindFrameBuffer(curr_fb);
	}

	void Blitter::Blit(TexturePtr const & dst, uint32_t dst_array_index, uint32_t dst_level,
		uint32_t dst_x_offset, uint32_t dst_y_offset, uint32_t dst_z_offset, uint32_t dst_width, uint32_t dst_height, uint32_t dst_depth,
		TexturePtr const & src, uint32_t src_array_index, uint32_t src_level,
		uint32_t src_x_offset, uint32_t src_y_offset, uint32_t src_z_offset, uint32_t src_width, uint32_t src_height, uint32_t src_depth,
		bool linear)
	{
		KFL_UNUSED(dst);
		KFL_UNUSED(dst_array_index);
		KFL_UNUSED(dst_level);
		KFL_UNUSED(dst_x_offset);
		KFL_UNUSED(dst_y_offset);
		KFL_UNUSED(dst_z_offset);
		KFL_UNUSED(dst_width);
		KFL_UNUSED(dst_height);
		KFL_UNUSED(dst_depth);
		KFL_UNUSED(src);
		KFL_UNUSED(src_array_index);
		KFL_UNUSED(src_level);
		KFL_UNUSED(src_x_offset);
		KFL_UNUSED(src_y_offset);
		KFL_UNUSED(src_z_offset);
		KFL_UNUSED(src_width);
		KFL_UNUSED(src_height);
		KFL_UNUSED(src_depth);
		KFL_UNUSED(linear);
	}

	void Blitter::Blit(TexturePtr const & dst, uint32_t dst_array_index, Texture::CubeFaces dst_face, uint32_t dst_level,
		uint32_t dst_x_offset, uint32_t dst_y_offset, uint32_t dst_width, uint32_t dst_height,
		TexturePtr const & src, uint32_t src_array_index, Texture::CubeFaces src_face, uint32_t src_level,
		uint32_t src_x_offset, uint32_t src_y_offset, uint32_t src_width, uint32_t src_height,
		bool linear)
	{
		KFL_UNUSED(dst);
		KFL_UNUSED(dst_array_index);
		KFL_UNUSED(dst_face);
		KFL_UNUSED(dst_level);
		KFL_UNUSED(dst_x_offset);
		KFL_UNUSED(dst_y_offset);
		KFL_UNUSED(dst_width);
		KFL_UNUSED(dst_height);
		KFL_UNUSED(src);
		KFL_UNUSED(src_array_index);
		KFL_UNUSED(src_face);
		KFL_UNUSED(src_level);
		KFL_UNUSED(src_x_offset);
		KFL_UNUSED(src_y_offset);
		KFL_UNUSED(src_width);
		KFL_UNUSED(src_height);
		KFL_UNUSED(linear);
	}
}
