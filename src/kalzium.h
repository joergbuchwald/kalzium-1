/***************************************************************************

                           kalzium.h  -  description
                             -------------------
    begin                : Die Dez  4 17:59:34 CET 2001
    copyright            : (C) 2001, 2002, 2003 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _KALZIUM_H
#define _KALZIUM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qtimer.h>

#include <kmainwindow.h>

#define KALZIUM_VERSION "0.9.3"

/** Kalzium is the base class of the project */


class QGridLayout;
class QLayout;
class QLabel;
class QLCDNumber;
class QSlider;
class QString;
class QButton;

class KConfig;
class KSelectAction;
class KToggleAction;
class KPushButton;

class CalcDialog;
class ElementKP;
class Fastinfo;
class SettingsDialog;
class KalziumLegend;
class StateOfMatterDlg;

#include "detailinfodlg.h"

typedef ElementKP* PElementKP;
typedef QLabel* PQLabel;

class Kalzium : public KMainWindow
{
	Q_OBJECT

	public:
		Kalzium();
		~Kalzium();

		int numofquestions;
		bool showFastInfo;

		KSelectAction *numerationmenu, 
		*psestylemenu, 
		*colorschememenu, 
		*weblookupmenu;

		PElementKP element[118];
		CalcDialog *calculationdialog;
		StateOfMatterDlg *templookup;
		KalziumLegend *legend;
		Fastinfo *fastinfo;
		DetailedTab *dtab;

		const QFont& generalFont() const {return generalKPFont;};
		const QFont& generalBoldFont() const {return generalKPBoldFont;};

    void restartLeaveTimer();
    void stopLeaveTimer();

	private:
		KConfig *main_config;
		KToggleAction   *timelineToggleAction, 
		*quickinfoToggleAction,
		*legendToggleAction;
		PQLabel labels[18];

		// Fonts for ElementKP
		QFont generalKPFont;
		QFont generalKPBoldFont;

		QGridLayout *maingrid;
		QLCDNumber *dateLCD;
		QSlider *dateS;
		QVBoxLayout *mainlayout;
		QWidget *main_window;		
		SettingsDialog *setDlg;
    QTimer mLeaveTimer;

		/**
		 * Called before the window is closed, will save the menusettings.
		 */
		bool queryClose();

		void setupActions();

		/**
		 *  Creates all 118 buttons
		 */
		void setupAllElementKPButtons();

		void setupCaption();

		/**
		 * This method sets the menu for kalzium using XMLGUI. 
		 */
		void setupConfig();

		void setupTimeline();

		/**
		 * numeration theme
		 */
		void showCAS() const;

		/**
		 * numeration theme
		 */
		void showIUPAC() const;

		/**
		 * numeration theme
		 */
		void showOldIUPAC() const;


		/**
		 * Called after setupActions , it sets the main window look 
		 * according to the menu settings
		 * @see Kalzium::setupActions
		 */
		void updateMainWindow();


		//******* Slots ******************************************************

	public slots:

		void changeTheLegend(int); //hopefully a dummyslot

		void changeColorScheme(int id=-1);

	private slots:

		void changeNumeration(int) const;

		void newToolbarConfig();
		void optionsConfigureKeys();
		void optionsConfigureToolbars();

		void setFont();

		void showPseStyle(int);

		/**
		 * This slot will open a window in which you can do some calculations.
		 */
		void slotCalculations();

		/**
		 * The quiz will start.
		 */
		void slotKnowledge();

		/**
		 * This slot gives you information if the element tends to
		 * built acid, bases, does neither or is amphoter.
		 */
		void slotShowAcidBeh();

		/**
		 * This slots shows all elements.
		 */
		void slotShowAll();

		/*
		 * Shows you the 4 different blocks.
		 */
		void slotShowBlocks();

		/**
		 * This slot gives you the information in which group
		 * the element is (1 to 8).
		 */
		void slotShowGroups();

		/**
		 * This slot hides all elements which have not been known
		 * when Mendelejew created the first PSE
		 */
		void slotShowMendelejew();

		/**
		 * This slot shows the users in what state the element
		 * is at 20 degree and 1013 h-pascal.
		 */
		void slotShowStateOfMatter();

		void slotPlotData();

		void slotSearchData();

		void updateColorMenu(int);

		void updateNumMenu(int);

		void slotShowTimeline(bool);
		void slotShowQuickinfo(bool);
		void slotShowLegend(bool);

		void hideSettingsDialog();

		void showSettingsDialog();

		/**
		 * Which element has been known when? A slider will
		 * appear and you can check it out.
		 */
		void timeline();

    void leaveTimeout();
};
#endif
