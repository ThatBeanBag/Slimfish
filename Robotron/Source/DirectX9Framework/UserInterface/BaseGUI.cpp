// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BaseGUI.cpp
// Description	: CBaseGUI implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "BaseGUI.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"
#include "..\Resource\XmlResource.h"
#include "Menu.h"
#include "EditBox.h"
#include "StaticText.h"

CBaseGUI::CBaseGUI(TGUICallback _guiEventCallback, bool _bIsModal, bool _bIsFull)
	:m_guiCallback(_guiEventCallback),
	m_bIsModal(_bIsModal),
	m_bIsFull(_bIsFull)
{

}


CBaseGUI::CBaseGUI(CBaseGUI&& _rrOther)
{
	Move(std::forward<CBaseGUI>(_rrOther));
}

CBaseGUI::~CBaseGUI()
{

}


CBaseGUI& CBaseGUI::operator=(CBaseGUI&& _rrOther)
{
	Move(std::forward<CBaseGUI>(_rrOther));

	return *this;
}

bool CBaseGUI::HandleEvents(const TAppMsg& _krMsg)
{
	for (unsigned int i = 0; i < m_vecGUIElements.size(); ++i) {
		if (m_vecGUIElements[i] && m_vecGUIElements[i]->IsActive() && m_vecGUIElements[i]->IsVisible()) {
			if (m_vecGUIElements[i]->HandleEvents(_krMsg)) {
				return true;
			}
		}
	}

	return false;
}

void CBaseGUI::Update(float _fDeltaTime)
{
	for (unsigned int i = 0; i < m_vecGUIElements.size(); ++i) {
		if (m_vecGUIElements[i] && m_vecGUIElements[i]->IsActive() && m_vecGUIElements[i]->IsVisible()) {
			m_vecGUIElements[i]->Update(_fDeltaTime);
		}
	}
}

void CBaseGUI::Render()
{
	for (unsigned int i = 0; i < m_vecGUIElements.size(); ++i) {
		if (m_vecGUIElements[i] && m_vecGUIElements[i]->IsVisible()) {
			m_vecGUIElements[i]->Render();
		}
	}
}


bool CBaseGUI::CreateGUIFromXML(const std::string& _krstrXMLFileName)
{
	shared_ptr<CXmlResource> pXmlDoc = CResourceManager<CXmlResource>::GetInstance()->Load(_krstrXMLFileName);

	if (!pXmlDoc) {
		return false;
	}

	TiXmlElement* pRoot = pXmlDoc->GetRoot();

	if (!pRoot) {
		// Failed to get the root element.
		DEBUG_ERROR("ERROR: Failed to create GUI from resource \"" + _krstrXMLFileName + "\"");

		return false;
	}
	else {
		for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) {
			std::string strValue = pNode->Value();
			if (strValue == "Button") {
				unique_ptr<CButton> pButton = CreateButtonFromXML(pNode);
				if (pButton) {
					AddButton(std::move(pButton));
				}
			}
			else if (strValue == "StaticImage") {
				unique_ptr<CStaticImage> pStaticImage = CreateStaticImageFromXML(pNode);
				if (pStaticImage) {
					AddStaticImage(std::move(pStaticImage));
				}
			}
			else if (strValue == "Menu") {
				unique_ptr<CMenu> pMenu = CreateMenuFromXML(pNode);
				if (pMenu) {
					AddMenu(std::move(pMenu));
				}
			}
			else if (strValue == "EditBox") {
				unique_ptr<CEditBox> pEditBox = CreateEditBoxFromXML(pNode);
				if (pEditBox) {
					AddEditBox(std::move(pEditBox));
				}
			}
			else if (strValue == "StaticText") {
				unique_ptr<CStaticText> pStaticText = CreateStaticTextFromXML(pNode);
				if (pStaticText) {
					AddStaticText(std::move(pStaticText));
				}
			}
			else {
				DEBUG_WARNING("Skipping \"" + strValue + "\" when creating GUI from resrouce \"" + _krstrXMLFileName + "\"");
			}
		}
	}

	return true;
}

bool CBaseGUI::AddButton(unique_ptr<CButton> _pButton)
{
	DEBUG_ASSERT(_pButton);

	auto findIter = m_buttons.find(_pButton->GetName());
	if (findIter != m_buttons.end()) {
		DEBUG_ERROR("Attempting to add a button with a conflicting name.");
		return false;
	}

	// Add the button to the map of buttons.
	m_buttons[_pButton->GetName()] = _pButton.get();
	AddGUIElement(std::move(_pButton));

	return true;
}

bool CBaseGUI::AddStaticImage(unique_ptr<CStaticImage> _pStaticImage)
{
	DEBUG_ASSERT(_pStaticImage);

	auto findIter = m_staticImages.find(_pStaticImage->GetName());
	if (findIter != m_staticImages.end()) {
		DEBUG_ERROR("Attempting to add a static image with a conflicting name.");
		return false;
	}

	// Add the static image to the map of images.
	m_staticImages[_pStaticImage->GetName()] = _pStaticImage.get();
	AddGUIElement(std::move(_pStaticImage));

	return true;
}

bool CBaseGUI::AddEditBox(unique_ptr<CEditBox> _pEditBox)
{
	DEBUG_ASSERT(_pEditBox);

	auto findIter = m_editBoxes.find(_pEditBox->GetName());
	if (findIter != m_editBoxes.end()) {
		DEBUG_ERROR("Attempting to add an edit box with a conflicting name.");
		return false;
	}

	// Add the edit box to the map of edit boxes.
	m_editBoxes[_pEditBox->GetName()] = _pEditBox.get();
	AddGUIElement(std::move(_pEditBox));

	return true;
}

bool CBaseGUI::AddMenu(unique_ptr<CMenu> _pMenu)
{
	DEBUG_ASSERT(_pMenu);

	auto findIter = m_menus.find(_pMenu->GetName());
	if (findIter != m_menus.end()) {
		DEBUG_ERROR("Attempting to add a menu with a conflicting name.");
		return false;
	}

	// Add the menu to the map of menus.
	m_menus[_pMenu->GetName()] = _pMenu.get();
	AddGUIElement(std::move(_pMenu));

	return true;
}

bool CBaseGUI::AddStaticText(unique_ptr<CStaticText> _pStaticText)
{
	DEBUG_ASSERT(_pStaticText);

	auto findIter = m_staticTexts.find(_pStaticText->GetName());
	if (findIter != m_staticTexts.end()) {
		DEBUG_ERROR("Attempting to add a menu with a conflicting name.");
		return false;
	}

	// Add the static text to the map of texts.
	m_staticTexts[_pStaticText->GetName()] = _pStaticText.get();
	AddGUIElement(std::move(_pStaticText));

	return true;
}

bool CBaseGUI::RemoveElement(const std::string& _krstrName)
{
	bool _bFound = false;

	for (auto iter = m_vecGUIElements.begin(); iter != m_vecGUIElements.end(); ++iter) {
		if ((*iter)->GetName() == _krstrName) {
			m_vecGUIElements.erase(iter);
			_bFound = true;
			break;
		}
	}

	if (!_bFound) {
		return false;
	}

	auto findIterStaticText = m_staticTexts.find(_krstrName);
	if (findIterStaticText != m_staticTexts.end()) {
		m_staticTexts.erase(findIterStaticText);
		return true;
	}

	auto findIterButton = m_buttons.find(_krstrName);
	if (findIterButton != m_buttons.end()) {
		m_buttons.erase(findIterButton);
		return true;
	}

	auto findIterStaticImage = m_staticImages.find(_krstrName);
	if (findIterStaticImage != m_staticImages.end()) {
		m_staticImages.erase(findIterStaticImage);
		return true;
	}

	auto findIterMenu = m_menus.find(_krstrName);
	if (findIterMenu != m_menus.end()) {
		m_menus.erase(findIterMenu);
		return true;
	}

	auto findIterEditBox = m_editBoxes.find(_krstrName);
	if (findIterEditBox != m_editBoxes.end()) {
		m_editBoxes.erase(findIterEditBox);
		return true;
	}

	return _bFound;
}

CButton* CBaseGUI::GetButton(const std::string& _krstrName)
{
	auto findIter = m_buttons.find(_krstrName);
	if (findIter != m_buttons.end()) {
		return findIter->second;
	}

	return nullptr;
}

CMenu* CBaseGUI::GetMenu(const std::string& _krstrName)
{
	auto findIter = m_menus.find(_krstrName);
	if (findIter != m_menus.end()) {
		return findIter->second;
	}

	return nullptr;
}

CStaticImage* CBaseGUI::GetStaticImage(const std::string& _krstrName)
{
	auto findIter = m_staticImages.find(_krstrName);
	if (findIter != m_staticImages.end()) {
		return findIter->second;
	}

	return nullptr;
}

CEditBox* CBaseGUI::GetEditBox(const std::string& _krstrName)
{
	auto findIter = m_editBoxes.find(_krstrName);
	if (findIter != m_editBoxes.end()) {
		return findIter->second;
	}

	return nullptr;
}

CStaticText* CBaseGUI::GetStaticText(const std::string& _krstrName)
{
	auto findIter = m_staticTexts.find(_krstrName);
	if (findIter != m_staticTexts.end()) {
		return findIter->second;
	}

	return nullptr;
}

bool CBaseGUI::IsModal() const
{
	return m_bIsModal;
}

bool CBaseGUI::IsFull() const
{
	return m_bIsFull;
}

void CBaseGUI::CallGUIEventCallback(EGUIEvent _eEvent, AGUIElement* _pGUIElement)
{
	if (m_guiCallback != nullptr) {
	// Is there a callback to call?
		// Call the event callback.
		m_guiCallback(_eEvent, _pGUIElement);
	}
}

unique_ptr<CButton> CBaseGUI::CreateButtonFromXML(TiXmlElement* _pXmlData)
{
	DEBUG_ASSERT(_pXmlData);

	unique_ptr<CStaticImage> pStaticImage = CreateStaticImageFromXML(_pXmlData);

	if (!pStaticImage) {
		DEBUG_ERROR("Failed to create button");
		return nullptr;
	}

	bool bToggled = false;

	TiXmlElement* pToggled = _pXmlData->FirstChildElement("Toggled");
	if (pToggled) {
		bToggled = true;
	}

	unique_ptr<CButton> pButton(new CButton((*pStaticImage), bToggled));

	if (!pButton) {
		return nullptr;
	}

	// Get the masks for the states of the button.
	TiXmlElement* pOutMask = _pXmlData->FirstChildElement("OutMask");
	if (pOutMask) {
		CRect outMask = CreateBoundingBoxFromXML(pOutMask, pButton->GetBoundingBox());
		pButton->SetMask(CButton::STATE_OUT, outMask);
	}
	else {
		DEBUG_WARNING("Couldn't find out mask when creating button from XML.");
	}

	TiXmlElement* pHoverMask = _pXmlData->FirstChildElement("HoverMask");
	if (pHoverMask) {
		CRect hoverMask = CreateBoundingBoxFromXML(pHoverMask, pButton->GetBoundingBox());
		pButton->SetMask(CButton::STATE_HOVER, hoverMask);
	}
	else {
		DEBUG_WARNING("Couldn't find hover mask when creating button from XML.");
	}

	TiXmlElement* pPressedMask = _pXmlData->FirstChildElement("PressedMask");
	if (pPressedMask) {
		CRect pressedMask = CreateBoundingBoxFromXML(pPressedMask, pButton->GetBoundingBox());
		pButton->SetMask(CButton::STATE_PRESSED, pressedMask);
	}
	else {
		DEBUG_WARNING("Couldn't find pressed mask when creating button from XML, using hover mask.");

		// It's useful to just set the pressed mask to the same the as the hover as often
		// it is not necessary to have a pressed state.
		pButton->SetMask(CButton::STATE_PRESSED, pButton->GetMask(CButton::STATE_HOVER));
	}


	return pButton;
}

unique_ptr<CMenu> CBaseGUI::CreateMenuFromXML(TiXmlElement* _pXmlData)
{
	DEBUG_ASSERT(_pXmlData);

	// Get the gap (space between each button).
	double dGap = 0.0;
	_pXmlData->Attribute("gap", &dGap);
	int iGap = static_cast<int>(dGap);

	// Get the alignment.
	CMenu::EAlignment eAlignment = CMenu::ALIGN_LEFT;	// Default to left/top.
	const char* pcAlignment = _pXmlData->Attribute("align");
	if (pcAlignment) {
		std::string strAlignment(pcAlignment);
		StringToLower(strAlignment);

		if (strAlignment == "left" || strAlignment == "top") {
			eAlignment = CMenu::ALIGN_LEFT;
		}
		else if (strAlignment == "right" || strAlignment == "bottom") {
			eAlignment = CMenu::ALIGN_RIGHT;
		}
		else if (strAlignment == "middle" || strAlignment == "mid") {
			eAlignment = CMenu::ALIGN_MIDDLE;
		}
		else {
			DEBUG_WARNING("Couldn't recognise alignment, using left alignment");
		}
	}
	else {
		DEBUG_WARNING("Couldn't find attribute \"align\" using left aligment");
	}

	// Get the direction.
	CMenu::EDirection eDirection = CMenu::DIR_VERTICAL;	// Default to vertical.
	const char* pcDirection = _pXmlData->Attribute("direction");
	if (pcDirection) {
		std::string strDirection(pcDirection);
		StringToLower(strDirection);

		if (strDirection == "vertical") {
			eDirection = CMenu::DIR_VERTICAL;
		}
		else if (strDirection == "horizontal") {
			eDirection = CMenu::DIR_HORIZONTAL;
		}
		else {
			DEBUG_WARNING("Couldn't recognise direction, using vertical");
		}
	}
	else {
		DEBUG_WARNING("Couldn't find attribute \"direction\" using vertical");
	}

	// Get the parameters for the base GUI element.
	std::string strName;
	CRect boundingBox;
	unsigned char ucAlpha;
	TAnchor anchor;
	bool bIsVisible = true;

	if (!CreateGUIElementFromXML(_pXmlData, strName, boundingBox, ucAlpha, anchor, bIsVisible)) {
		return nullptr;
	}

	// Create the menu.
	unique_ptr<CMenu> pMenu(new CMenu(strName, boundingBox.GetX(), boundingBox.GetY(), iGap, eAlignment, eDirection));

	if (!pMenu) {
		return nullptr;
	}

	// Set the parameters.
	pMenu->SetAnchor(anchor);
	pMenu->SetAlpha(ucAlpha);
	pMenu->SetVisibility(bIsVisible);

	if (m_vecGUIElements.empty()) {
		pMenu->SetSelected(true);
	}

	// Add elements to the menu.
	for (TiXmlElement* pNode = _pXmlData->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) {
		std::string strValue = pNode->Value();
		if (strValue == "Button") {
			// Create the button.
			unique_ptr<CButton> pButton = CreateButtonFromXML(pNode);
			if (pButton) {
				// Add the button to the menu.
				pMenu->AddGUIElement(pButton.get());

				// Add the button to the GUI.
				AddButton(std::move(pButton));
			}
		}
		else if (strValue == "StaticImage") {
			unique_ptr<CStaticImage> pStaticImage = CreateStaticImageFromXML(pNode);
			if (pStaticImage) {
				// Add the StaticImage to the menu.
				pMenu->AddGUIElement(pStaticImage.get());

				AddStaticImage(std::move(pStaticImage));
			}
		}
		else if (strValue == "Menu") {
			unique_ptr<CMenu> pSubMenu = CreateMenuFromXML(pNode);
			if (pSubMenu) {
				// Add the Menu to the menu.
				pMenu->AddGUIElement(pSubMenu.get());

				AddMenu(std::move(pSubMenu));
			}
		}
		else if (strValue == "EditBox") {
			unique_ptr<CEditBox> pEditBox = CreateEditBoxFromXML(pNode);
			if (pEditBox) {
				// Add the EditBox to the menu.
				pMenu->AddGUIElement(pEditBox.get());

				AddEditBox(std::move(pEditBox));
			}
		}
		else if (strValue == "StaticText") {
			unique_ptr<CStaticText> pStaticText = CreateStaticTextFromXML(pNode);
			if (pStaticText) {
				// Add the StaticText to the menu.
				pMenu->AddGUIElement(pStaticText.get());

				AddStaticText(std::move(pStaticText));
			}
		}
		else {
			DEBUG_WARNING("Skipping \"" + strValue + "\" when creating menu from XML.");
		}
	}

	return pMenu;
}

unique_ptr<CStaticImage> CBaseGUI::CreateStaticImageFromXML(TiXmlElement* _pXmlData)
{
	DEBUG_ASSERT(_pXmlData);

	// Get the parameters for the base GUI element.
	std::string strName;
	CRect boundingBox;
	unsigned char ucAlpha;
	TAnchor anchor;
	bool bIsVisible = true;

	if (!CreateGUIElementFromXML(_pXmlData, strName, boundingBox, ucAlpha, anchor, bIsVisible)) {
		return nullptr;
	}
	
	std::string strResource;

	TiXmlElement* pImage = _pXmlData->FirstChildElement("Image");
	if (pImage && pImage->FirstChild()) {
		strResource = pImage->FirstChild()->Value();
	}
	else {
		DEBUG_ERROR("Couldn't find \"Image\" element for StaticImage.");
		return nullptr;
	}

	shared_ptr<ISprite> pSprite = CResourceManager<ISprite>::GetInstance()->Load(strResource);
	if (!pSprite) {
		DEBUG_ERROR("Unable to load image for StaticImage.");
		return false;
	}

	unique_ptr<CStaticImage> pStaticImage(new CStaticImage(strName, boundingBox, pSprite));

	if (!pStaticImage) {
		return nullptr;
	}

	// Set the parameters and return.
	pStaticImage->SetAnchor(anchor);
	pStaticImage->SetAlpha(ucAlpha);
	pStaticImage->SetVisibility(bIsVisible);
	return pStaticImage;
}

unique_ptr<CEditBox> CBaseGUI::CreateEditBoxFromXML(TiXmlElement* _pXmlData)
{
	unique_ptr<CStaticImage> pStaticImage = CreateStaticImageFromXML(_pXmlData);

	if (!pStaticImage) {
		DEBUG_ERROR("Failed to create edit box.");
		return nullptr;
	}

	int iMaxCharacters = 256;
	int iHeight = 12;
	TColour colour = g_kCOLOUR_WHITE;

	TiXmlElement* pMaxCharacters = _pXmlData->FirstChildElement("MaxCharacters");
	if (pMaxCharacters && pMaxCharacters->FirstChild()) {
		std::string strMaxCharacters = pMaxCharacters->FirstChild()->Value();

		iMaxCharacters = atoi(strMaxCharacters.c_str());
	}
	else {
		DEBUG_ERROR("Couldn't find \"MaxCharacters\" when creating edit box.");
		return nullptr;
	}

	TiXmlElement* pFontHeight = _pXmlData->FirstChildElement("FontHeight");
	if (pFontHeight && pFontHeight->FirstChild()) {
		std::string strFontHeight = pFontHeight->FirstChild()->Value();

		iHeight = atoi(strFontHeight.c_str());
	}
	else {
		DEBUG_ERROR("Couldn't find \"FontHeight\" when creating edit box.");
		return nullptr;
	}

	TiXmlElement* pFontColour = _pXmlData->FirstChildElement("FontColour");
	if (pFontColour) {
		int iR = colour.m_r;
		int iG = colour.m_g;
		int iB = colour.m_b;
		int iA = colour.m_a;

		pFontColour->Attribute("r", &iR);
		pFontColour->Attribute("g", &iG);
		pFontColour->Attribute("b", &iB);
		pFontColour->Attribute("a", &iA);

		colour = CreateColourARGB(iA, iR, iG, iB);
	}
	else {
		DEBUG_ERROR("Couldn't find \"FontColour\" when creating edit box.");
		return nullptr;
	}

	CRect textBox = pStaticImage->GetBoundingBox();
	textBox.SetX(0);
	textBox.SetY(0);

	TiXmlElement* pTextBox = _pXmlData->FirstChildElement("TextBox");
	if (pTextBox) {
		textBox = CreateBoundingBoxFromXML(pTextBox, textBox);
	}

	unique_ptr<CEditBox> pEditBox(new CEditBox((*pStaticImage), textBox, iMaxCharacters, iHeight, colour));
	return pEditBox;
}

unique_ptr<CStaticText> CBaseGUI::CreateStaticTextFromXML(TiXmlElement* _pXmlData)
{
	// Get the parameters for the base GUI element.
	std::string strName;
	CRect boundingBox;
	unsigned char ucAlpha;
	TAnchor anchor;
	bool bIsVisible = true;

	if (!CreateGUIElementFromXML(_pXmlData, strName, boundingBox, ucAlpha, anchor, bIsVisible)) {
		return nullptr;
	}

	int iHeight = 12;
	TColour colour = g_kCOLOUR_WHITE;

	TiXmlElement* pFontHeight = _pXmlData->FirstChildElement("FontHeight");
	if (pFontHeight && pFontHeight->FirstChild()) {
		std::string strFontHeight = pFontHeight->FirstChild()->Value();

		iHeight = atoi(strFontHeight.c_str());
	}
	else {
		DEBUG_ERROR("Couldn't find \"FontHeight\" when creating edit box.");
		return nullptr;
	}

	TiXmlElement* pFontColour = _pXmlData->FirstChildElement("FontColour");
	if (pFontColour) {
		int iR = colour.m_r;
		int iG = colour.m_g;
		int iB = colour.m_b;
		int iA = colour.m_a;

		pFontColour->Attribute("r", &iR);
		pFontColour->Attribute("g", &iG);
		pFontColour->Attribute("b", &iB);
		pFontColour->Attribute("a", &iA);

		colour = CreateColourARGB(iA, iR, iG, iB);
	}
	else {
		DEBUG_ERROR("Couldn't find \"FontColour\" when creating edit box.");
		return nullptr;
	}

	std::string strText;
	TiXmlElement* pText = _pXmlData->FirstChildElement("Text");
	if (pText && pText->FirstChild()) {
		if (pText->FirstChild()->Value()) {
			strText = pText->FirstChild()->Value();
		}
	}
	else {
		DEBUG_ERROR("Couldn't find \"Text\" when creating edit box.");
		return nullptr;
	}

	unique_ptr<CStaticText> pStaticText(new CStaticText(strName, boundingBox, strText, iHeight, colour));
	pStaticText->SetAlpha(ucAlpha);
	pStaticText->SetAnchor(anchor);
	pStaticText->SetVisibility(bIsVisible);
	return pStaticText;
}

bool CBaseGUI::CreateGUIElementFromXML(TiXmlElement* _pXmlData, std::string& _rstrName, CRect& _rRect, unsigned char& _rucAlpha, TAnchor& _rAnchor, bool& _rbIsVisible)
{
	DEBUG_ASSERT(_pXmlData);

	if (_pXmlData) {
		// Get the name of the GUI element.
		const char* pcName = _pXmlData->Attribute("name");
		if (pcName) {
		// Does this element have a name?
			_rstrName = pcName;
		}
		else {
		// No name found, ALL gui elements require a name.
			DEBUG_ERROR("No name specified when creating gui element; ALL gui elements need a name.");
			return false;
		}

		// Get the bounding rectangle.
		_rRect = CreateBoundingBoxFromXML(_pXmlData);

		// Get the alpha value of the GUI element.
		int iAlpha = g_kcALPHA_OPAQUE;	// By default the alpha is opaque (255).
		_pXmlData->Attribute("alpha", &iAlpha);
		_rucAlpha = static_cast<unsigned char>(iAlpha);

		// Get the anchoring.
		const char* _pcAnchor = _pXmlData->Attribute("anchor");
		if (_pcAnchor) {
		// Is there an anchor parameter specified?
			std::string strAnchor = _pcAnchor;

			// Find the first space character to split the anchor text into the two components.
			// Example text: anchor="bottom right"
			unsigned int uiSpace = strAnchor.find_first_of(' ');

			if (uiSpace != std::string::npos) {
				std::string strVerticalAnchor = strAnchor.substr(0, uiSpace);
				strVerticalAnchor = StringToLower(strVerticalAnchor);

				if (strVerticalAnchor == "top") {
					_rAnchor.m_eVerticalAnchoring = ANCHOR_LEFT_TOP;
				}
				else if (strVerticalAnchor == "mid" || strVerticalAnchor == "middle") {
					_rAnchor.m_eVerticalAnchoring = ANCHOR_MIDDLE;
				}
				else if (strVerticalAnchor == "bottom") {
					_rAnchor.m_eVerticalAnchoring = ANCHOR_RIGHT_BOTTOM;
				}

				// Get the horizontal anchor string.
				std::string strHorizontalAnchor = strAnchor.substr(uiSpace + 1);
				strHorizontalAnchor = StringToLower(strHorizontalAnchor);

				if (strHorizontalAnchor == "left") {
					_rAnchor.m_eHorizontalAnchoring = ANCHOR_LEFT_TOP;
				}
				else if (strHorizontalAnchor == "mid" || strHorizontalAnchor == "middle") {
					_rAnchor.m_eHorizontalAnchoring = ANCHOR_MIDDLE;
				}
				else if (strHorizontalAnchor == "right") {
					_rAnchor.m_eHorizontalAnchoring = ANCHOR_RIGHT_BOTTOM;
				}
			}
		}

		TiXmlElement* pHidden = _pXmlData->FirstChildElement("Hidden");
		if (pHidden) {
			_rbIsVisible = false;
		}
		else {
			_rbIsVisible = true;
		}

		return true;
	}

	return false;
}

CRect CBaseGUI::CreateBoundingBoxFromXML(TiXmlElement* _pXmlData, const CRect& _krRectDefaults)
{
	DEBUG_ASSERT(_pXmlData);

	CRect boundingBox;

	if (_pXmlData) {
		// Get the X component of the position of the GUI element.
		int iX = _krRectDefaults.GetX();
		_pXmlData->Attribute("x", &iX);
		boundingBox.SetX(iX);

		// Get the Y component of the position of the GUI element.
		int iY = _krRectDefaults.GetY();
		_pXmlData->Attribute("y", &iY);
		boundingBox.SetY(iY);

		// Get the width of the GUI element.
		int iWidth = _krRectDefaults.GetWidth();
		_pXmlData->Attribute("width", &iWidth);
		boundingBox.SetWidth(iWidth);

		// Get the height of the GUI element.
		int iHeight = _krRectDefaults.GetHeight();
		_pXmlData->Attribute("height", &iHeight);
		boundingBox.SetHeight(iHeight);
	}

	return boundingBox;
}

void CBaseGUI::AddGUIElement(unique_ptr<AGUIElement> _pGUIElement)
{
	DEBUG_ASSERT(_pGUIElement);

	// The first element added is the selected element.
	if (m_vecGUIElements.empty()) {
		_pGUIElement->SetSelected(true);
	}

	_pGUIElement->SetOwner(this);
	// Add the button to the master list.
	m_vecGUIElements.push_back(std::move(_pGUIElement));
}

void CBaseGUI::Move(CBaseGUI&& _rrOther)
{
	m_vecGUIElements = std::move(_rrOther.m_vecGUIElements);
	m_buttons = std::move(_rrOther.m_buttons);
	m_menus = std::move(_rrOther.m_menus);
	m_editBoxes = std::move(_rrOther.m_editBoxes);
	m_staticImages = std::move(_rrOther.m_staticImages);
	m_staticTexts = std::move(_rrOther.m_staticTexts);
	m_guiCallback = std::move(_rrOther.m_guiCallback);
}
