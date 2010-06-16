/**	@file	PhysicalComponent.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::PhysicalComponent'
*/

#include <Athena-Physics/PhysicalComponent.h>

using namespace Athena;
using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace std;


/************************************** CONSTANTS **************************************/

///< Name of the type of component
const std::string PhysicalComponent::TYPE = "Athena/Physics/PhysicalComponent";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

PhysicalComponent::PhysicalComponent(const std::string& strName, ComponentsList* pList)
: Component(strName, pList)
{
	m_id.type = COMP_PHYSICAL;
}

//-----------------------------------------------------------------------

PhysicalComponent::~PhysicalComponent()
{
}

//-----------------------------------------------------------------------

PhysicalComponent* PhysicalComponent::create(const std::string& strName, ComponentsList* pList)
{
	return new PhysicalComponent(strName, pList);
}

//-----------------------------------------------------------------------

PhysicalComponent* PhysicalComponent::cast(Component* pComponent)
{
	return dynamic_cast<PhysicalComponent*>(pComponent);
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* PhysicalComponent::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Component::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool PhysicalComponent::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return PhysicalComponent::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool PhysicalComponent::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

	// Destroy the value
	delete pValue;

	return true;
}
