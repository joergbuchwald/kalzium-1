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

#include "gamesimplementation.h"

#include <time.h>

#include <kdebug.h>
#include <klocale.h>

//RAgame and RAField
RAGame::RAField::RAField( int x, int y )
	: Field( x, y )
{
	m_size_x = x;
	m_size_y = y;
}

void RAGame::start()
{
	random.setSeed( time(0) );

	//fill the field with x*y white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
	{
		for ( int y = 0; y < m_field->ySize() ; ++y )
		{
			m_field->addStone( new Stone( Stone::White, QPoint( x, y ) ) );
		}
	}
}

RAGame* RAGame::instance()
{
	static RAGame g;
	return &g;
}

QByteArray RAGame::name() const
{
	return "RAGame";
}

QString RAGame::description() const
{
	return i18n( "Radioactive decay" );
}

QString RAGame::rules() const
{
	return i18n( "The simulation starts with an all white board. In each turn, a Stone on the board will be selected randomly. If the stone is white, it will turn black. This represents the radioactive decay.  The game becomes interesting if you alter the number of turns it runs. The probablilty that a stone swap the colour from white to black decreases with each turn. It is proportional to the halflife period.");
}

void RAGame::RAField::moveStoneTo( Stone* stone, const QPoint& newPosition )
{
}

void RAGame::RAField::addStone( Stone* stone )
{
	m_stones.append( stone );
}

void RAGame::rollDice()
{
	m_number++;
	
	//generating two random numbers
	const int x = ( int ) ( random.getLong( m_field->xSize() ) );
	const int y = ( int ) ( random.getLong( m_field->ySize() ) );

	QPoint point( x, y );

	Stone* stone = m_field->stoneAtPosition( point );

	//increase if both dices have the same value. This
	//simulates the halflifetime
	if ( x == y )
		m_counter++;

	if ( !stone )
		return;

	//if there is a white stone make it black
	if ( stone->player() == Stone::White )
		stone->swap();
}

RAGame::RAGame()
	: Game()
{
	m_field = new RAField( 7,7 );

	setField( m_field );
	
	m_counter = 0;
	m_number = 0;

	start();
}


//////////////////////////////////////////////////////////////////////
//CrystallizationField
CrystallizationGame::CrystallizationField::CrystallizationField( int x, int y )
	: Field( x, y )
{
	m_size_x = x;
	m_size_y = y;
}

CrystallizationGame* CrystallizationGame::instance()
{
	static CrystallizationGame g;
	return &g;
}

QByteArray CrystallizationGame::name() const
{
	return "CrystallizationGame";
}

QString CrystallizationGame::description() const
{
	return i18n( "Crystallization" );
}

QString CrystallizationGame::rules() const
{
	return i18n( "" );
}

void CrystallizationGame::CrystallizationField::moveStoneTo( Stone* stone, const QPoint& newPosition )
{
}

void CrystallizationGame::CrystallizationField::addStone( Stone* stone )
{
	m_stones.append( stone );
}

void CrystallizationGame::rollDice()
{
	m_number++;

	//generating two random numbers
	const int x = ( int ) random.getLong( m_field->xSize() );
	const int y = ( int ) random.getLong( m_field->ySize() );
	
	//the propability
	const int w = ( int ) random.getLong( 2 );

	QPoint point( x, y );

	Stone* stone = m_field->stoneAtPosition( point );

	if ( !stone )
		return;

	int numTeam = neighboursTeam( stone );
	int totalNum = neighboursNum( stone );
	int numOtherTeam = totalNum - numTeam;
	
	if ( numTeam < numOtherTeam )
	{
		//do nothing
	}
	else if ( numTeam > numOtherTeam )
	{//exange two stones (one from each team)
			exchangeStones( point );
	}
	else if ( numTeam == numOtherTeam )
	{//with a probability of 50% do as in the if-condition above
		if ( w%2 )
			exchangeStones( point );
	}
	else
		kdDebug() << "error" << endl;
	
}

void CrystallizationGame::exchangeStones( const QPoint& point )
{
	Stone* stone = m_field->stoneAtPosition( point );
	
	QList<Stone*> otherTeamStones;
	
	Stone* stone1 = m_field->stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = m_field->stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = m_field->stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = m_field->stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 && stone1->player() != stone->player() )
		otherTeamStones << stone1;
	if ( stone2 && stone2->player() != stone->player()  )
		otherTeamStones << stone2;
	if ( stone3 && stone3->player() != stone->player()  )
		otherTeamStones << stone3;
	if ( stone4 && stone4->player() != stone->player()  )
		otherTeamStones << stone4;
	
	if ( otherTeamStones.count() < 1 )//well, there is nothing to exchange...
		return;

	//the stone to exchange...
	Stone *chosenStone = 0;
	
	if ( otherTeamStones.count() == 1 ) {
		chosenStone = otherTeamStones[ 0 ];//take the first (and only) Stone
	}
	else{
		const int choice = ( int ) ( random.getLong( otherTeamStones.count() ) );

		chosenStone = otherTeamStones[ choice ];
	}

	chosenStone->swap();
	stone->swap();
}

int CrystallizationGame::neighboursNum( Stone* stone )
{
	QPoint point( stone->position() );
	
	QList<Stone*> Stones;
	
	Stone* stone1 = m_field->stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = m_field->stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = m_field->stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = m_field->stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 ) Stones << stone1;
	if ( stone2 ) Stones << stone2;
	if ( stone3 ) Stones << stone3;
	if ( stone4 ) Stones << stone4;

	return Stones.count();
}

int CrystallizationGame::neighboursTeam( Stone* stone )
{
	QPoint point( stone->position() );
	
	QList<Stone*> TeamStones;
	
	Stone* stone1 = m_field->stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = m_field->stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = m_field->stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = m_field->stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 && stone1->player() == stone->player() )
		TeamStones << stone1;
	if ( stone2 && stone2->player() == stone->player()  )
		TeamStones << stone2;
	if ( stone3 && stone3->player() == stone->player()  )
		TeamStones << stone3;
	if ( stone4 && stone4->player() == stone->player()  )
		TeamStones << stone4;
	
	return TeamStones.count();
}

//Crystallizationgame
CrystallizationGame::CrystallizationGame()
	: Game()
{
	m_field = new CrystallizationField( 6,6 );
	setField( m_field );

	m_number = 0;

	start();
}

void CrystallizationGame::start()
{
	random.setSeed( time(0) );
	
	//fill the field with x*y black and white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
	{
		for ( int y = 0; y < m_field->ySize() ; ++y )
		{
			if ( y < m_field->ySize()/2 )
				m_field->addStone( new Stone( Stone::White, QPoint( x, y ) ) );
			else
				m_field->addStone( new Stone( Stone::Black, QPoint( x, y ) ) );
		}
	}
}
