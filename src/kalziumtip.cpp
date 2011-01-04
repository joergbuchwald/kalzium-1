/***************************************************************************
 *   Copyright (C) 2005 by Zack Rusin <zrusin@kde.org>			   *
 *  Sami Kyostil <skyostil@kempele.fi>			 		   *
 *   Aaron J. Seigo <aseigo@kde.org>		 			   *
 *		     Martin Pfeiffer <hubipete@gmx.net>                    *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include "kalziumtip.h"
#include "kalziumutils.h"
#include "element.h"

#include <tqapplication.h>
#include <tqpixmap.h>
#include <tqimage.h>
#include <tqsimplerichtext.h>
#include <tqpainter.h>
#include <tqdesktopwidget.h>

#include <kdialog.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kstandarddirs.h>

KalziumTip::KalziumTip( TQWidget * parent, const char * name, WFlags f ) : TQWidget( parent, name, f ) 
{
	setFocusPolicy(NoFocus); //the widget don't get the keyboard focus
	setBackgroundMode(NoBackground); // widget has no background
	resize(0,0);
	hide(); //initailly hide it
	m_richText = 0;
	m_noElemIcon = KGlobal::iconLoader()->loadIcon( "orbits", KIcon::NoGroup, 64 );
	setMouseTracking(true); // receice mouse move events
	
	connect(&m_frameTimer, TQT_SIGNAL(timeout()), TQT_SLOT(internalUpdate()));
}

void KalziumTip::showTip( TQPoint mouse, Element* element, int visibleWidth, int visibleHeight )
{
	TQWidget *p = 0;
        if ( dynamic_cast<TQWidget*>( parent() ) )
                p = static_cast<TQWidget*>( parent() );

        if ( p )
        {
	 	if ( mouse.y() + size().height() > visibleHeight )
                        mouse.setY( mouse.y() - size().height() );
               	if ( mouse.x() + size().width() > visibleWidth )
                       	mouse.setX( mouse.x() - size().width() );
	
		if ( p->width() < visibleWidth )
		{	 
                	if ( mouse.x() + size().width() > p->width() )
                        	mouse.setX( mouse.x() - size().width() );
		}

		if ( p->height() < visibleHeight ) 
		{
			if ( mouse.y() + size().height() > p->height() )
                        	mouse.setY( mouse.y() - size().height() );
		}
        }
	
	m_mousePointer = mouse;
	
	if( element == m_tippedElement )
	{
		// Avoid moving out of the current screen
		if (m_mousePointer.x()+width() > tqApp->desktop()->width())
				m_mousePointer.setX(tqApp->desktop()->width() - width());

		move(m_mousePointer); //do not paint again if already painted
	}
	else
	{
		if ( element == 0)
			return;	
	
		m_tippedElement = element;
		
		loadIcon(); //load icon	
		display();
	}
}

void KalziumTip::paintEvent(TQPaintEvent* e)
{
	if (m_dirty)
    	{
        	displayInternal();
        	m_dirty = false;
    	}

	TQPainter p(this);
   	p.drawPixmap(e->rect().topLeft(), m_pixmap, e->rect());
}

void KalziumTip::mouseMoveEvent(TQMouseEvent * e)
{
	// delegate the mouse move event to the parent (actually the elements table)
	// so that this tooltip doesn't stop to be updated
	e->ignore();
}

void KalziumTip::display()
{
	if( !m_tippedElement )
		return;

	delete m_richText;
	
	TQString elementname = m_tippedElement->elname();
	
	TQString number = i18n( "Number: %1" )
			.arg( TQString::number(m_tippedElement->number()) );
	
	TQString mass = i18n( "Mass: %1" )
			.arg( KalziumUtils::localizedValue(m_tippedElement->mass(), 6) );

	m_richText = new TQSimpleRichText("<qt><h1>" + elementname + "</h1><p>"
						    + number + "</p><p>"
						    + mass  +"</p></qt>", font());

	m_richText->setWidth(400);

	m_tqmaskEffect = isVisible() ? Plain : Dissolve;
    	m_dissolveSize = 24;
    	m_dissolveDelta = -1;

    	displayInternal();
	m_frameTimer.start(500/30);	

	move(m_mousePointer);
	show();
}

void KalziumTip::displayInternal()
{
    	if (!m_richText)
        	return;
    	

	// determine text rectangel sizes
	TQRect textRect(0,0,0,0);
	textRect.setWidth(m_richText->widthUsed());
	textRect.setHeight(m_richText->height());

	//moves the rect
	textRect.moveBy(-textRect.left(), -textRect.top());
	// add the new coordinates
	textRect.addCoords(0,0,2,2);

	int margin = KDialog::marginHint();
	int height = QMAX(m_icon.height(), textRect.height()) + 2 * margin;
    	int textX = 2 + m_icon.width() + 2 * margin;
    	int width = textX + textRect.width() + margin;
    	int textY = (height - textRect.height()) / 2;

	//resize pixmap, tqmask and widget
	m_tqmask.resize(width, height);
	m_pixmap.resize(width, height);
	resize(width, height);

	// create and set transparency tqmask
    	switch(m_tqmaskEffect)
    	{
        	case Plain:
            		plainMask();
            		break;

        	case Dissolve:
            		dissolveMask();
            		break;
    	}

	// draw background
    	TQPainter bufferPainter(&m_pixmap);
    	bufferPainter.setPen(Qt::black);
    	bufferPainter.setBrush(backgroundColor());
    	bufferPainter.drawRoundRect(0, 0, width, height,
                                1600 / width, 1600 / height);

    	// draw icon if present
    	if (!m_icon.isNull())
    	{
        	bufferPainter.drawPixmap(margin,
                                 	margin,
                                 	m_icon, 0, 0,
                                 	m_icon.width(), m_icon.height());
    	}
	
	// draw text shadow
        TQColorGroup cg = tqcolorGroup();
        cg.setColor(TQColorGroup::Text, cg.background().dark(115));
        int shadowOffset = TQApplication::reverseLayout() ? -1 : 1;
        m_richText->draw(&bufferPainter, 5 + textX + shadowOffset, textY + 1, TQRect(), cg);

        // draw text
        cg = tqcolorGroup();
        m_richText->draw(&bufferPainter, 5 + textX, textY, rect(), cg);
}

void KalziumTip::dissolveMask()
{
    TQPainter tqmaskPainter(&m_tqmask);

    m_tqmask.fill(Qt::black);

    tqmaskPainter.setBrush(Qt::white);
    tqmaskPainter.setPen(Qt::white);
    tqmaskPainter.drawRoundRect(m_tqmask.rect(), 1600 / m_tqmask.rect().width(),
                              1600 / m_tqmask.rect().height());

    m_dissolveSize += m_dissolveDelta;

    if (m_dissolveSize > 0)
    {
        tqmaskPainter.setRasterOp(Qt::EraseROP);

        int x, y, s;
        const int size = 16;
	const int heightsize = size + height();

        for (y = 0; y < heightsize; y += size)
        {
            x = width();
            s = m_dissolveSize * x / 128;
            for (; x > -size; x -= size, s -= 2)
            {
                if (s < 0)
                {
                    s = 0;
                }
                tqmaskPainter.drawEllipse(x - s / 2, y - s / 2, s, s);
            }
        }
    }
    else if (m_dissolveSize < 0)
    {
        m_frameTimer.stop();
        m_dissolveDelta = 1;
    }

    setMask(m_tqmask);
}

void KalziumTip::hide()
{
	m_frameTimer.stop();
    	m_tippedElement = 0;
    	TQWidget::hide();
}

void KalziumTip::plainMask()
{
    	TQPainter tqmaskPainter(&m_tqmask);

    	m_tqmask.fill(Qt::black);

    	tqmaskPainter.setBrush(Qt::white);
    	tqmaskPainter.setPen(Qt::white);
    	tqmaskPainter.drawRoundRect(m_tqmask.rect(), 1600 / m_tqmask.rect().width(),
                              1600 / m_tqmask.rect().height());
    	setMask(m_tqmask);
    	m_frameTimer.stop();
}

void KalziumTip::internalUpdate()
{
	m_dirty = true;
    	update();
}

void KalziumTip::loadIcon()
{
	TQString iconpath = locate(  "data" , "kalzium/elempics/" + m_tippedElement->symbol() + ".jpg" );
	if ( !iconpath.isEmpty() )
	{
		TQImage img ( iconpath, "JPEG" );
		img = img.smoothScale ( 128, 128, TQImage::ScaleMin );
		m_icon.convertFromImage( img );

	}
	else
	{
		m_icon = m_noElemIcon;
	}
}

#include "kalziumtip.moc"