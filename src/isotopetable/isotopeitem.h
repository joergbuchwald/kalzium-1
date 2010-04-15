#ifndef ISOTOPEITEM_H
#define ISOTOPEITEM_H
/***************************************************************************
 *   Copyright (C) 2007, 2008 by Carsten Niehaus                           *
 *   cniehaus@kde.org                                                      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <QFont>
#include <QGraphicsRectItem>

class Isotope;

/**
 * The class represtens the items which is drawn on the QGraphicsScene. Each such item represents on
 * Isotope.
 * @author Carsten Niehaus
 */
class IsotopeItem : public QAbstractGraphicsShapeItem
{
	public:
            /**
             * there are several types of decay for an isotope.
             */
            enum IsotopeType { alpha, ec, multiple, bplus, bminus, stable };

            enum { Type = UserType + 1 };

            /**
             * @param isotope The Isotope represented
             */
            IsotopeItem(Isotope * isotope,  qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0);

            /**
             * @return the Isotope the item represents
             */
            Isotope* isotope() const{
                return m_isotope;
            }

            QRectF boundingRect() const {
                return m_rect;
            }

            /**
             * @return the Type of the item
             */
            int type() const{
                return Type;
            }

            void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );


        private:
            IsotopeType m_type;
            Isotope* m_isotope;
            QRectF m_rect;
            QFont m_symbolFont;
            QFont m_otherFont;

            /**
             * @return the IsotopeType of the Isotope
             */
            IsotopeType getType( Isotope * );

        protected:
            void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // ISOTOPEITEM_H
