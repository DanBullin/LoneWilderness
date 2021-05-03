/*! \file testScript.cpp
*
* \brief A script used in testing features
*
*/
#include "scripts/testing/testScript.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"

//! TestScript()
TestScript::TestScript()
{
}

//! ~TestScript()
TestScript::~TestScript()
{
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void TestScript::onPostUpdate(const float timestep, const float totalTime)
{
	ENGINE_INFO("gf");
}