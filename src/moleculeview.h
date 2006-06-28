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

#include "ui_moleculeviewerwidget.h"

class QTreeWidgetItem;
class QItemSelection;

#include <openbabel/mol.h>
#include <openbabel/obiter.h>

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
	
		/**
		 * Update the statistical information about the current molecule
		 */
		void updateStatistics();
	
	signals:
		/**
		 * the atoms in  @p atoms have been selected by the user
		 */
		void atomsSelected( QList<OpenBabel::OBAtom*> atoms );

	private slots:
		/**
		 * Load a molecule
		 */
		void slotLoadMolecule();

		/**
		 */
		void slotAtomsSelected();
};

#endif // MOLECULEVIEW_H
