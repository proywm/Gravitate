#include "TextAreaComponent.h"
#include "DisplayManager.h"
#include "VisualComponent.h"
#include <iostream>
#include <algorithm>

TextAreaComponent::TextAreaComponent(void)
{
}
TextAreaComponent::~TextAreaComponent(void)
{
}

void TextAreaComponent::init(XMLElement *componentElement)
{
	ComponentID = componentElement->IntAttribute("ComponentType");
	XMLElement* shapeElement = componentElement->FirstChildElement("Structure");
 
	CurrentTextAreaRow = shapeElement->IntAttribute("NoOfRows");
	CurrentTextAreaCol = shapeElement->IntAttribute("NoOfCols");
	CreateMap(shapeElement);
}

void TextAreaComponent::update(double deltaMS)
{

}

void TextAreaComponent::CreateMap(XMLElement *componentElement)
{
	//ToDO: read new custom shapes from componentElement
	//simplified implementation for now
	for(int r=0;r<MAXROW;r++)
	{
		for(int c=0;c<MAXCOL;c++)
		{
			if(r<CurrentTextAreaRow && c<CurrentTextAreaCol)
				TextArea[r][c] = 1;
			else
				TextArea[r][c] = 0;
		}
	}
}
