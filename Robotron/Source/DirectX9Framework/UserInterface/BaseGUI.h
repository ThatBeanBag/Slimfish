// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BaseGUI.h
// Description	: CBaseGUI declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __BASEGUI_H__
#define __BASEGUI_H__

// Library Includes

// Local Includes

// Forward Declaration.
class AGUIElement;
class CMenu;
class CButton;
class CStaticImage;
class CStaticText;
class CEditBox;
struct TAnchor;

enum EGUIEvent {
	GUIEVENT_BUTTON_PRESSED,
	GUIEVENT_BUTTON_RELEASED
};

class CBaseGUI {
	typedef std::function<void(EGUIEvent, AGUIElement*)> TGUICallback;
	typedef std::vector<unique_ptr<AGUIElement>> TGUIElements;
	typedef std::map<std::string, CButton*> TButtons;
	typedef std::map<std::string, CMenu*> TMenus;
	typedef std::map<std::string, CStaticImage*> TStaticImages;
	typedef std::map<std::string, CEditBox*> TEditBoxes;
	typedef std::map<std::string, CStaticText*> TStaticTexts;

	// Member Functions
public:
	CBaseGUI(TGUICallback _guiEventCallback, bool _bIsModal = false, bool _bIsFull = false);
	CBaseGUI(const CBaseGUI& _krOther) = delete;
	CBaseGUI(CBaseGUI&& _rrOther);
	virtual ~CBaseGUI();

	CBaseGUI& operator=(const CBaseGUI& _krOther) = delete;
	CBaseGUI& operator=(CBaseGUI&& _rrOther);

	bool HandleEvents(const TAppMsg& _krMsg);
	void Update(float _fDeltaTime);
	void Render();

	bool CreateGUIFromXML(const std::string& _krstrXMLFileName);

	bool AddButton(unique_ptr<CButton> _pButton);
	bool AddStaticImage(unique_ptr<CStaticImage> _pStaticImage);
	bool AddEditBox(unique_ptr<CEditBox> _pEditBox);
	bool AddMenu(unique_ptr<CMenu> _pMenu);
	bool AddStaticText(unique_ptr<CStaticText> _pStaticText);

	bool RemoveElement(const std::string& _krstrName);

	CButton* GetButton(const std::string& _krstrName);
	CMenu* GetMenu(const std::string& _krstrName);
	CStaticImage* GetStaticImage(const std::string& _krstrName);
	CEditBox* GetEditBox(const std::string& _krstrName);
	CStaticText* GetStaticText(const std::string& _kstrName);

	bool IsModal() const;
	bool IsFull() const;

	void CallGUIEventCallback(EGUIEvent _eEvent, AGUIElement* _pGUIElement);
protected:
private:
	// Creation Helper Functions For GUI Elements
	unique_ptr<CButton> CreateButtonFromXML(TiXmlElement* _pXmlData);
	unique_ptr<CMenu> CreateMenuFromXML(TiXmlElement* _pXmlData);
	unique_ptr<CStaticImage> CreateStaticImageFromXML(TiXmlElement* _pXmlData);
	unique_ptr<CEditBox> CreateEditBoxFromXML(TiXmlElement* _pXmlData);
	unique_ptr<CStaticText> CreateStaticTextFromXML(TiXmlElement* _pXmlData);

	bool CreateGUIElementFromXML(TiXmlElement* _pXmlData, std::string& _rstrName, CRect& _rRect, unsigned char& _rucAlpha, TAnchor& _rAnchor, bool& _rbIsVisible);
	CRect CreateBoundingBoxFromXML(TiXmlElement* _pXmlData, const CRect& _krRectDefaults = CRect());

	void AddGUIElement(unique_ptr<AGUIElement> _pGUIElement);

	void Move(CBaseGUI&& _rrOther);

	// Member Variables
public:
protected:
private:
	TGUIElements m_vecGUIElements;	// The master list, preferable for iteration through GUI elements.

	// The following are mappings from names to GUI elements of the specific types.
	// They are useful when retrieving certain GUI elements.
	TButtons m_buttons;
	TMenus m_menus;
	TStaticImages m_staticImages;
	TEditBoxes m_editBoxes;
	TStaticTexts m_staticTexts;

	TGUICallback m_guiCallback;	// The GUI event callback.

	bool m_bIsModal;
	bool m_bIsFull;
};

#endif	// __BASEGUI_H__