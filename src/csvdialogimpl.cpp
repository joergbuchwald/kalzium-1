/***************************************************************************
                         csvdialogimpl.cpp  -  description
                             -------------------
    begin                : June 2003
	copyright            : (C) 2003 by Carsten Niehaus                     
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

//KDE-Includes
#include <kdebug.h>
#include <kcombobox.h>
#include <kurlrequester.h>

//QT-Includes
#include <qcheckbox.h>

#include "csvdialogimpl.h"

csvdialogImpl::csvdialogImpl ( bool SelOnly, int Export, QString& url, QChar& del, QWidget *parent, const char *name )  : csvdialog (parent,name)
{
	requestedFile = url;
	selectionOnly = SelOnly;
	whatToExport = Export;
	m_delimiter = del;
}

void csvdialogImpl::slotStartExport()
{
	requestedFile = urlReq->url();
	selectionOnly = m_selectionOnly->isChecked();
	whatToExport = kComboBox1->currentItem();
}

void csvdialogImpl::delimiterClicked( int id )
{
	switch ( id )
	{
		case 0: // comma
			m_delimiter = ',';
			break;
		case 1: // semicolon
			m_delimiter = ';';
			break;
		case 2: // tab
			m_delimiter = '\t';
			break;
		case 3: // space
			m_delimiter = ' ';
			break;
		case 4: // other
			m_delimiter = m_delimiterEdit->text().at(0);
			break;
	}
}

#include "csvdialogimpl.moc"
