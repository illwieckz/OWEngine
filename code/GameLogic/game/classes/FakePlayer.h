////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2013 V.
//  Copyright (C) 2015 Dusan Jocic <dusanjocic@msn.com>
// 
//  OWEngine source code is free software; you can redistribute it
//  and/or modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//  
//  OWEngine source code is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA,
//  or simply visit <http://www.gnu.org/licenses/>.
// -------------------------------------------------------------------------
//  File name:   FakePlayer.h
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __FAKEPLAYER_H__
#define __FAKEPLAYER_H__

#include "Player.h"

class FakePlayer : public Player {
	safePtr_c<Player> leader;
	// navigation component
	class navigator_c *nav;
public:
	FakePlayer();
	~FakePlayer();

	DECLARE_CLASS( FakePlayer );

	virtual void runFrame();

	void setLeader(class Player *newLeader);

	void onBulletHit(const vec3_c &hitPosWorld, const vec3_c &dirWorld, int damageCount);
};

#endif // __FAKEPLAYER_H__
