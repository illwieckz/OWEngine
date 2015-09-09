////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2012 V.
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
//  File name:   showNormalColors.frag
//  Version:     v1.00
//  Created:     
//  Compilers:   
//  Description: Used to debug normal vectors of models
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

// shader varying variables
varying vec3 v_vertNormal; 

void main() {
	// TODO: transform normals from entity space to world space?
	// (for entities only)
	//gl_FragColor.rgb = v_vertNormal;
	gl_FragColor.rgb = vec3(0.5,0.5,0.5) + normalize(v_vertNormal) * 0.5;
	gl_FragColor.a = 1.0;
}