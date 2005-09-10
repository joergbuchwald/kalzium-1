/***************************************************************************
 *   Copyright (C) 2005      by Pino Toscano,    toscano.pino@tiscali.it   *
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "crystalmoviewidget.h"

#include <qdir.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qregexp.h>

#include <kdebug.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kpushbutton.h>

CrystalMovieWidget::CrystalMovieWidget( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
	m_picpath = ".";

	init();
}

CrystalMovieWidget::CrystalMovieWidget( const QString& picturePath, QWidget *parent, const char *name )
    : QWidget( parent, name )
{
	m_picpath = picturePath.isEmpty() ? "." : picturePath;

	init();
}

void CrystalMovieWidget::init()
{
	KIconLoader *il = KGlobal::iconLoader();

	QVBoxLayout *mainlay = new QVBoxLayout( this );

	m_pix = new QLabel( this );
	m_pix->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	m_pix->setLineWidth( 1 );
	mainlay->addWidget( m_pix );
	
	QHBoxLayout *hlay = new QHBoxLayout( 0L, 0, 5 );

	m_prev = new KPushButton( this );
	m_prev->setIconSet( il->loadIconSet( "player_start", KIcon::NoGroup, 16 ) );
	hlay->addWidget( m_prev );

	m_start = new KPushButton( this );
	m_start->setIconSet( il->loadIconSet( "player_play", KIcon::NoGroup, 16 ) );
	hlay->addWidget( m_start );

	m_pause = new KPushButton( this );
	m_pause->setIconSet( il->loadIconSet( "player_pause", KIcon::NoGroup, 16 ) );
	m_pause->setEnabled( false );
	hlay->addWidget( m_pause );

	m_next = new KPushButton( this );
	m_next->setIconSet( il->loadIconSet( "player_end", KIcon::NoGroup, 16 ) );
	hlay->addWidget( m_next );
	hlay->addItem( new QSpacerItem( 5, 5, QSizePolicy::Expanding, QSizePolicy::Fixed ) );

	mainlay->addLayout( hlay );

	m_currentPictureId = 0;

	connect( m_prev, SIGNAL( clicked() ), this, SLOT( prevFrame() ) );
	connect( m_start, SIGNAL( clicked() ), this, SLOT( start() ) );
	connect( m_pause, SIGNAL( clicked() ), this, SLOT( pause() ) );
	connect( m_next, SIGNAL( clicked() ), this, SLOT( nextFrame() ) );
	connect( &m_timer, SIGNAL( timeout() ), this, SLOT( animate() ) );

	reloadImages();
}

void CrystalMovieWidget::setPicturePath( const QString& path )
{
	if ( path.isEmpty() )
		return;

	m_picpath = path;

	reloadImages();
}

void CrystalMovieWidget::reloadImages()
{
	m_pics.clear();
	QDir dir( m_picpath );
	QStringList tmplist = dir.entryList();
	QRegExp imgre( "[0-9]+\\.([Jj][Pp][Ee]?[Gg]|[Pp][Nn][Gg])" );
	int i;
	for ( i = 0; i < tmplist.size(); i++ )
	{
		if ( imgre.search( tmplist[i] ) > -1 )
			m_pics << tmplist[i];
	}
	if ( m_pics.isEmpty() )
		return;
	
	uint maxlength = 0;
	for ( i = 0; i < m_pics.size(); i++ )
	{
		if ( m_pics[i].length() > static_cast<int>( maxlength ) )
			maxlength = static_cast<uint>( m_pics[i].length() );
	}
	for ( i = 0; i < m_pics.size(); i++ )
	{
		m_pics[i] = m_pics[i].rightJustify( maxlength, '0' );
	}
kdDebug() << "OK: " << m_pics << endl;
	if ( m_pics.size() > 0 )
	{
		m_pics.sort();
		loadImage( 0 );
	}
}

void CrystalMovieWidget::loadImage( int id )
{
	if ( ( id < 0 ) || ( id >= m_pics.size() ) )
		return;

	QPixmap tmp( m_picpath + m_pics[id] );
	m_pix->setPixmap( tmp );
	m_pix->resize( tmp.size() );
}

void CrystalMovieWidget::prevFrame()
{
	if ( m_timer.isActive() )
		return;

	if ( ( --m_currentPictureId ) < 0 )
		m_currentPictureId = m_pics.size() - 1;
	loadImage( m_currentPictureId );
}

void CrystalMovieWidget::start()
{
	m_prev->setEnabled( false );
	m_start->setEnabled( false );
	m_next->setEnabled( false );
	m_pause->setEnabled( true );
	m_timer.start( 300 );
}

void CrystalMovieWidget::pause()
{
	m_timer.stop();
	m_prev->setEnabled( true );
	m_start->setEnabled( true );
	m_next->setEnabled( true );
	m_pause->setEnabled( false );
}

void CrystalMovieWidget::nextFrame()
{
	if ( m_timer.isActive() )
		return;

	animate();
}

void CrystalMovieWidget::animate()
{
	if ( ( ++m_currentPictureId ) >= m_pics.size() )
		m_currentPictureId = 0;
	loadImage( m_currentPictureId );
}

#include "crystalmoviewidget.moc"
