/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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
#ifndef MOLECULEVIEW_H
#define MOLECULEVIEW_H

#include <kdialog.h>

#include "kalziumglwidget.h"
#include "ui_moleculeviewerwidget.h"

class KComboBox;
class KPushButton;

class QListWidget;

/**
 * @author Carsten Niehaus
 */
class MoleculeDialog : public KDialog
{
	Q_OBJECT

	public:
		MoleculeDialog( QWidget  * parent );
		~MoleculeDialog();

	private:
		QString m_path;///to store the path were the molecules are located

		Ui::moleculeViewerForm ui;	
		void fillList();

	public slots:
		void slotLoadMolecule();
};

#endif // MOLECULEVIEW_H