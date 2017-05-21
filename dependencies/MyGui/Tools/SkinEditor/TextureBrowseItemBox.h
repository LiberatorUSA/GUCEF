/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __TEXTUREBROWSEITEM_BOX_H__
#define __TEXTUREBROWSEITEM_BOX_H__

#include <MyGUI.h>
#include "ItemBox/BaseItemBox.h"
#include "TextureBrowseCell.h"

namespace demo
{

	class TextureBrowseItemBox :
		public wraps::BaseItemBox<tools::TextureBrowseCell>
	{
	public:
		TextureBrowseItemBox(MyGUI::Widget* _parent);
		virtual ~TextureBrowseItemBox();
	};

} // namespace demo

#endif // __TEXTUREBROWSEITEM_BOX_H__
