// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SAHepler.h
// Description	: CSAHepler declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "SAHelper.h"

// Local Includes

const float CSAHelper::s_kfDEF_INITIAL_TEMP = 100.0f;

CSAHelper::CSAHelper()
	:m_fInitialTemperature(s_kfDEF_INITIAL_TEMP),
	m_heightMap(0, 0)
{

}

CSAHelper::~CSAHelper()
{
	if (m_thread.joinable()) {
		m_thread.join();
	}
}

CPoint CSAHelper::Anneal(CImage _heightMap, float _fInitialTemp, float _fFinalTemp, int _iIterMax)
{
	if (m_thread.joinable()) {
		{
			std::lock_guard<std::recursive_mutex> lockGuard(m_mutex);
			m_bStopWaiting = true;
		}

		m_thread.join();
	}

	m_heightMap = std::move(_heightMap);

	CPoint initialPoint(0, 0);

	if (m_heightMap.GetSize() <= 0) {
		return initialPoint;
	}

	// Choose a random starting location.
	initialPoint.m_iX = g_pApp->GetRandomNumber(0, m_heightMap.GetWidth());
	initialPoint.m_iY = g_pApp->GetRandomNumber(0, m_heightMap.GetHeight());

	auto energyFunction = [this](const CPoint& _krPoint) { return GetHeight(_krPoint); };
	auto scheduleFunction = [this](float _fTemperature) { return ScheduleTemperature(_fTemperature); };
	auto perturbFunction = [this](const CPoint& _krPoint) { return GetNext(_krPoint); };

	m_thread = std::thread(SimulatedAnnealing<CPoint, float, decltype(energyFunction), decltype(scheduleFunction), decltype(perturbFunction)>, 
						   initialPoint, _fInitialTemp, _fFinalTemp, energyFunction, scheduleFunction, perturbFunction, _iIterMax);

	std::lock_guard<std::recursive_mutex> lockGuard(m_mutex);
	return m_currentState;
}

float CSAHelper::GetHeight(const CPoint& _krImagePoint) const
{
	std::lock_guard<std::recursive_mutex> lockGuard(m_mutex);

	if (m_heightMap.GetSize() <= 0) {
		return 0;
	}

	TColour pixel = m_heightMap.GetPixel(_krImagePoint.m_iX, _krImagePoint.m_iY);
	float fHeight = (static_cast<float>(pixel.m_r) / 255.0f) - 0.5f;

	return fHeight;
}

float CSAHelper::ScheduleTemperature(float _fTemperature)
{
	// Cooling schedule.
	return _fTemperature * 0.99f;
}

CPoint CSAHelper::GetNext(const CPoint& _krPoint)
{
	CPoint newPoint;
	bool bWait = false;

	// Lock mutex scope.
	{	
		std::lock_guard<std::recursive_mutex> lockGuard(m_mutex);

		if (m_currentState != _krPoint) {
			m_currentState = _krPoint;

			if (!m_bStopWaiting) {
				bWait = true;
			}
		}

		// Choose a random new location.
		newPoint.m_iX = g_pApp->GetRandomNumber(0, m_heightMap.GetWidth());
		newPoint.m_iY = g_pApp->GetRandomNumber(0, m_heightMap.GetHeight());

		 // Jump somewhere random.
		/*int iJumpX = g_pApp->GetRandomNumber(-50, 50);
		int iJumpY = g_pApp->GetRandomNumber(-50, 50);

		newPoint = CPoint(_krPoint.m_iX + iJumpX, _krPoint.m_iY + iJumpY);

		// Make sure the new point is on the image.
		if (newPoint.m_iX < 0) {
			newPoint.m_iX = 0;
		}
		else if (newPoint.m_iX >= m_heightMap.GetWidth()) {
			newPoint.m_iX = m_heightMap.GetWidth() - 1;
		}

		if (newPoint.m_iY < 0) {
			newPoint.m_iY = 0;
		}
		else if (newPoint.m_iY >= m_heightMap.GetHeight()) {
			newPoint.m_iY = m_heightMap.GetHeight() - 1;
		}*/
	}	// Unlock mutex.

	if (bWait) {
		int m_iSleepTime = 16;
		// Wait.
		std::this_thread::sleep_for(std::chrono::milliseconds(m_iSleepTime));
	}

	return newPoint;
}

CVec3 CSAHelper::GetUnitPosition(const CPoint& _krImagePoint) const
{
	std::lock_guard<std::recursive_mutex> lockGuard(m_mutex);

	if (m_heightMap.GetSize() <= 0) {
		return CVec3();
	}

	CVec3 vec3Position;
	vec3Position.x = static_cast<float>(_krImagePoint.m_iX) / static_cast<float>(m_heightMap.GetWidth()) - 0.5f;
	vec3Position.y = GetHeight(_krImagePoint);
	vec3Position.z = static_cast<float>(_krImagePoint.m_iY) / static_cast<float>(m_heightMap.GetHeight()) - 0.5f;

	return vec3Position;
}

CVec3 CSAHelper::GetCurrentPosition() const
{
	std::lock_guard<std::recursive_mutex> lockGuard(m_mutex);
	if (m_heightMap.GetSize() <= 0) {
		return CVec3();
	}

	return GetUnitPosition(m_currentState);
}