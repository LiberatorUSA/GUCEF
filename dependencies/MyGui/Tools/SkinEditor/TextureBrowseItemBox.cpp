/*!
	@file
	@author		Albert Semenov
	@date		07/2008
*/
#include "Precompiled.h"
//#include "ItemBox.h"
#include "TextureBrowseItemBox.h"

namespace demo
{

	TextureBrowseItemBox::TextureBrowseItemBox(MyGUI::Widget* _parent) :
		wraps::BaseItemBox<tools::TextureBrowseCell>( _parent)
	{
	}

	TextureBrowseItemBox::~TextureBrowseItemBox()
	{
	}

} // namespace demo
