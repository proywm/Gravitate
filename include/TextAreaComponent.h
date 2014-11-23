#pragma once
#include "GlobalHeader.h"
#include "ActorComponentInterface.h"
#include "Shape.h"

class TextAreaComponent : public ActorComponentInterface
{
public:
	TextAreaComponent(void);
	~TextAreaComponent(void);

	void init(XMLElement *componentElement);
	void update(double deltaMS);
	void CreateMap(XMLElement *componentElement);

	int TextArea[MAXROW][MAXCOL];
	int CurrentTextAreaRow;
	int CurrentTextAreaCol;
};
