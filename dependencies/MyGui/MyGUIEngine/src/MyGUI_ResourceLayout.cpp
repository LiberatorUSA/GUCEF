/*!
	@file
	@author		George Evmenov
	@date		06/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceLayout.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	ResourceLayout::ResourceLayout()
	{
	}

	ResourceLayout::ResourceLayout(xml::ElementPtr _node, const std::string& _fileName)
	{
		// FIXME hardcoded version
		deserialization(_node, Version(1, 0, 0));
		mResourceName = _fileName;
	}

	void ResourceLayout::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);

		mLayoutData.clear();

		xml::ElementEnumerator widget = _node->getElementEnumerator();
		while (widget.next("Widget"))
			mLayoutData.push_back(parseWidget(widget));
	}

	WidgetInfo ResourceLayout::parseWidget(xml::ElementEnumerator& _widget)
	{
		WidgetInfo widgetInfo;

		std::string tmp;

		_widget->findAttribute("type", widgetInfo.type);
		_widget->findAttribute("skin", widgetInfo.skin);
		_widget->findAttribute("layer", widgetInfo.layer);

		if (_widget->findAttribute("align", tmp)) widgetInfo.align = Align::parse(tmp);

		_widget->findAttribute("name", widgetInfo.name);

		if (_widget->findAttribute("style", tmp)) widgetInfo.style = WidgetStyle::parse(tmp);

		IntCoord coord;
		if (_widget->findAttribute("position", tmp))
		{
			widgetInfo.intCoord = IntCoord::parse(tmp);
			widgetInfo.positionType = WidgetInfo::Pixels;
		}
		else if (_widget->findAttribute("position_real", tmp))
		{
			widgetInfo.floatCoord = FloatCoord::parse(tmp);
			widgetInfo.positionType = WidgetInfo::Relative;
		}

		// ����� ����� � ��������
		xml::ElementEnumerator node = _widget->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Widget")
			{
				widgetInfo.childWidgetsInfo.push_back(parseWidget(node));
			}
			else if (node->getName() == "Property")
			{
				widgetInfo.properties.push_back(PairString(node->findAttribute("key"), node->findAttribute("value")));
			}
			else if (node->getName() == "UserString")
			{
				widgetInfo.userStrings[node->findAttribute("key")] = node->findAttribute("value");
			}
			else if (node->getName() == "Controller")
			{
				ControllerInfo controllerInfo;
				controllerInfo.type = node->findAttribute("type");

				xml::ElementEnumerator prop = node->getElementEnumerator();
				while (prop.next("Property"))
					controllerInfo.properties[prop->findAttribute("key")] = prop->findAttribute("value");

				widgetInfo.controllers.push_back(controllerInfo);
			}
		}

		return widgetInfo;
	}

	VectorWidgetPtr ResourceLayout::createLayout(const std::string& _prefix, Widget* _parent)
	{
		VectorWidgetPtr widgets;

		for (VectorWidgetInfo::iterator iter = mLayoutData.begin(); iter != mLayoutData.end(); ++iter)
		{
			Widget* widget = createWidget(*iter, _prefix, _parent);
			widgets.push_back(widget);
		}

		return widgets;
	}

	Widget* ResourceLayout::createWidget(const WidgetInfo& _widgetInfo, const std::string& _prefix, Widget* _parent, bool _template)
	{
		std::string widgetName = _widgetInfo.name;
		WidgetStyle style = _widgetInfo.style;
		std::string widgetLayer = _widgetInfo.layer;

		if (!widgetName.empty()) widgetName = _prefix + widgetName;

		if (_parent != nullptr && style != WidgetStyle::Popup) widgetLayer.clear();

		IntCoord coord;
		if (_widgetInfo.positionType == WidgetInfo::Pixels) coord = _widgetInfo.intCoord;
		else if (_widgetInfo.positionType == WidgetInfo::Relative)
		{
			if (_parent == nullptr || style == WidgetStyle::Popup)
				coord = CoordConverter::convertFromRelative(_widgetInfo.floatCoord, RenderManager::getInstance().getViewSize());
			else
				coord = CoordConverter::convertFromRelative(_widgetInfo.floatCoord, _parent->getClientCoord().size());
		}

		Widget* wid;
		if (nullptr == _parent)
			wid = Gui::getInstance().createWidgetT(_widgetInfo.type, _widgetInfo.skin, coord, _widgetInfo.align, widgetLayer, widgetName);
		else if (_template)
			wid = _parent->_createSkinWidget(style, _widgetInfo.type, _widgetInfo.skin, coord, _widgetInfo.align, widgetLayer, widgetName);
		else
			wid = _parent->createWidgetT(style, _widgetInfo.type, _widgetInfo.skin, coord, _widgetInfo.align, widgetLayer, widgetName);

		for (VectorStringPairs::const_iterator iter = _widgetInfo.properties.begin(); iter != _widgetInfo.properties.end(); ++iter)
		{
			wid->setProperty(iter->first, iter->second);
		}

		for (MapString::const_iterator iter = _widgetInfo.userStrings.begin(); iter != _widgetInfo.userStrings.end(); ++iter)
		{
			wid->setUserString(iter->first, iter->second);
			if (!_template)
				LayoutManager::getInstance().eventAddUserString(wid, iter->first, iter->second);
		}

		for (VectorWidgetInfo::const_iterator iter = _widgetInfo.childWidgetsInfo.begin(); iter != _widgetInfo.childWidgetsInfo.end(); ++iter)
		{
			createWidget(*iter, _prefix, wid);
		}

		for (std::vector<ControllerInfo>::const_iterator iter = _widgetInfo.controllers.begin(); iter != _widgetInfo.controllers.end(); ++iter)
		{
			MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(iter->type);
			if (item)
			{
				for (MapString::const_iterator iterProp = iter->properties.begin(); iterProp != iter->properties.end(); ++iterProp)
				{
					item->setProperty(iterProp->first, iterProp->second);
				}
				MyGUI::ControllerManager::getInstance().addItem(wid, item);
			}
			else
			{
				MYGUI_LOG(Warning, "Controller '" << iter->type << "' not found");
			}
		}

		return wid;
	}

	const VectorWidgetInfo& ResourceLayout::getLayoutData() const
	{
		return mLayoutData;
	}

} // namespace MyGUI
