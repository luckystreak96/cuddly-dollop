#include "hudComponent.h"
#include <iostream>

int HudComponent::counter = 0;

HudComponent::HudComponent()
{
	//std::cout << ++counter << " HudComponents exist." << std::endl;
}

HudComponent::~HudComponent()
{
	//std::cout << --counter << " HudComponents exist." << std::endl;
}


void HudComponent::Update()
{

}

void HudComponent::Destroy()
{
	_done = true;
}

void HudComponent::SetRender()
{
}
