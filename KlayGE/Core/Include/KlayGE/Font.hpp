// Font.hpp
// KlayGE Font类 头文件
// Ver 3.6.0
// 版权所有(C) 龚敏敏, 2003-2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// 增加了Rect对齐的方式 (2007.6.5)
//
// 3.3.0
// 支持渲染到3D位置 (2006.5.20)
//
// 2.8.0
// 增加了pool (2005.8.10)
//
// 2.3.0
// 使用FreeType实现字体读取 (2004.12.26)
// 
// 2.0.0
// 初次建立 (2003.8.18)
//
// 修改记录
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FONT_HPP
#define _FONT_HPP

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>

#include <KlayGE/Rect.hpp>

#include <list>
#include <map>
#include <vector>

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4127)
#endif
#include <boost/pool/pool_alloc.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

namespace KlayGE
{
	// 在3D环境中画出文字
	/////////////////////////////////////////////////////////////////////////////////
	class Font
	{
	public:
		// 字体建立标志
		enum FontStyle
		{
			FS_Bold			= 1UL << 0,
			FS_Italic		= 1UL << 1,
			FS_Underline	= 1UL << 2,
			FS_Strikeout	= 1UL << 3,

			FS_TwoSided		= 1UL << 4,
			FS_Cullable		= 1UL << 5
		};

		enum FontAlign
		{
			FA_Hor_Left		= 1UL << 0,
			FA_Hor_Center	= 1UL << 1,
			FA_Hor_Right	= 1UL << 2,

			FA_Ver_Top		= 1UL << 3,
			FA_Ver_Middle	= 1UL << 4,
			FA_Ver_Bottom	= 1UL << 5
		};

	public:
		Font(std::string const & fontName, uint32_t fontHeight = 16, uint32_t flags = 0);

		void RenderText(float x, float y, Color const & clr,
			std::wstring const & text);
		void RenderText(float x, float y, float z, float xScale, float yScale, Color const & clr, 
			std::wstring const & text);
		void RenderText(Rect const & rc, float z, float xScale, float yScale, Color const & clr, 
			std::wstring const & text, uint32_t align);
		void RenderText(float4x4 const & mvp, Color const & clr, std::wstring const & text);

		uint32_t FontHeight() const;

	private:
		RenderablePtr	font_renderable_;
		uint32_t		fso_attrib_;
	};
}

#endif		// _FONT_HPP
