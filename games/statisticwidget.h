/***************************************************************************
 *   Copyright (C) 2006      by Carsten Niehaus,    cniehaus@kde.org       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/
#ifndef STATISTICWIDGET_H
#define STATISTICWIDGET_H
#include <QWidget>

class QPaintEvent;
class Game;

class StatisticWidget : public QWidget
{
	Q_OBJECT

	public:
		StatisticWidget( QWidget * parent );

		void setGame( Game * game ){
			m_game = game;
		}

	public slots:
		/**
		 * update the statistics
		 */
		void updateData();

	protected:
		virtual void paintEvent( QPaintEvent * e );

	private:
		int num;

		int black;

		int white;

		Game * m_game;
};
#endif // STATISTICWIDGET_H