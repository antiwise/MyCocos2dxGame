//
//  AlertPopupUI.cpp
//  Voxel_Explorer
//
//  Created by 创李 on 15/12/10.
//
//

#include "AlertPopupUI.hpp"
#include "UtilityHelper.h"
USING_NS_CC;
AlertPopupUI::AlertPopupUI()
{
    m_nIsBlankClose     = false;
    m_pOk               = nullptr;
    m_pCancel           = nullptr;
    m_pTitle            = nullptr;
    m_pMessage          = nullptr;
    m_pNegativeFun      = nullptr;
    m_pPositiveFun      = nullptr;
    
}
AlertPopupUI::~AlertPopupUI()
{
    
}
bool AlertPopupUI::initUi()
{
    if(!PopupUILayer::initUi())
        return false;
    if(!load("alertLayer.csb",false))
        return false;
    return true;
}
bool AlertPopupUI::addEvents()
{
    if(!PopupUILayer::addEvents())
        return false;
    m_pOk = dynamic_cast<ui::Button*>(UtilityHelper::seekNodeByName(m_pRootNode, "alert_layer_ok"));
    if (!m_pOk)
        return false;
    m_pCancel = dynamic_cast<ui::Button*>(UtilityHelper::seekNodeByName(m_pRootNode, "alert_layer_cancel"));
    if (!m_pCancel)
        return false;
    m_pTitle = dynamic_cast<ui::Text*>(UtilityHelper::seekNodeByName(m_pRootNode, "alert_layer_title_text"));
    if (!m_pTitle)
        return false;
    m_pMessage = dynamic_cast<ui::Text*>(UtilityHelper::seekNodeByName(m_pRootNode, "alert_layer_message"));
    if (!m_pMessage)
        return false;
    
    m_pTitle->setFontName(UtilityHelper::getLocalString("FONT_NAME"));
    m_pMessage->setFontName(UtilityHelper::getLocalString("FONT_NAME"));
    
    m_pOk->setTitleFontName(UtilityHelper::getLocalString("FONT_NAME"));
    m_pOk->setTitleFontSize(36);
    m_pOk->getTitleRenderer()->setScale(0.3);
    m_pOk->setTitleText(UtilityHelper::getLocalStringForUi("BTN_TEXT_OK"));

    m_pCancel->setTitleFontName(UtilityHelper::getLocalString("FONT_NAME"));
    m_pCancel->setTitleFontSize(36);
    m_pCancel->getTitleRenderer()->setScale(0.3);
    m_pCancel->setTitleText(UtilityHelper::getLocalStringForUi("BTN_TEXT_CANCEL"));
    
    m_pOk->addClickEventListener(CC_CALLBACK_1(AlertPopupUI::onClickOk, this));
    m_pCancel->addClickEventListener(CC_CALLBACK_1(AlertPopupUI::onClickCancel,this));
    
    refreshUIView();
    
    return true;
}
void AlertPopupUI::onClickOk(cocos2d::Ref *ref)
{
    if (m_pPositiveFun) {
        m_pPositiveFun(this);
    }
    closePopup();
}
void AlertPopupUI::onClickCancel(cocos2d::Ref *ref)
{
    if (m_pNegativeFun) {
        m_pNegativeFun(this);
    }
    closePopup();
}
void AlertPopupUI::refreshUIView()
{
    if (m_pNegativeFun && m_pPositiveFun) {
        m_pOk->setVisible(true);
        m_pCancel->setVisible(true);
        m_pOk->setPosition(cocos2d::Vec2(m_pRootNode->getContentSize().width*0.26,m_pOk->getPositionY()));
        m_pCancel->setPosition(cocos2d::Vec2(m_pOk->getPositionX()+m_pOk->getContentSize().width+5,m_pCancel->getPositionY()));
        
    }
    
    if (m_pNegativeFun&&!m_pPositiveFun) {
        m_pOk->setVisible(false);
        m_pCancel->setPosition(cocos2d::Vec2(m_pRootNode->getContentSize().width*0.5,m_pCancel->getPositionY()));
    }
    
    if (!m_pNegativeFun&&m_pPositiveFun) {
        m_pCancel->setVisible(false);
        m_pOk->setPosition(cocos2d::Vec2(m_pRootNode->getContentSize().width*0.5,m_pCancel->getPositionY()));
    }
}
void AlertPopupUI::setNegativeListerner(onClickListener listener,std::string str)
{
    m_pNegativeFun = listener;
    if (!str.empty()) {
        m_pCancel->setTitleText(str);
    }
    refreshUIView();
}
void AlertPopupUI::setPositiveListerner(onClickListener listener,std::string str)
{
    m_pPositiveFun = listener;
    if (!str.empty()) {
        m_pOk->setTitleText(str);
    }
    refreshUIView();
}
void AlertPopupUI::setTitle( std::string title)
{
    if (m_pTitle) {
        m_pTitle->setString(title);
    }
}
void AlertPopupUI::setMessage( std::string message)
{
    if (!m_pMessage)
        return;
    cocos2d::Size fonSize = UtilityHelper::getSingleStrFontSize(m_pMessage, message);
    int charCount  = (int)(m_pRootNode->getContentSize().width/fonSize.width);
    UtilityHelper::getLineStr(message, charCount);
    m_pMessage->setString(message);
 }

