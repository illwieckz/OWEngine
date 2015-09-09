////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
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
//  File name:   bloom.frag
//  Version:     v1.00
//  Created:     
//  Compilers:   
//  Description: 
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

varying vec3 pos;

void main()
{
    float r = fract ( sin ( dot ( pos.xy ,vec2 ( 11.3713,67.3219 ) ) ) * 2351.3718 );

    float offset = ( 0.5 - r ) * gl_TexCoord[0].x * 0.045;

    float intensity = min ( 1.0, cos ( ( length ( pos * 2.0 ) + offset ) / gl_TexCoord[0].x) );
    float gradient  = intensity * smoothstep( 0.0, 2.0, intensity );

    gradient *= smoothstep ( 1.0,0.67+r*0.33, 1.0-intensity );

    gl_FragColor = gl_Color * gradient;
}