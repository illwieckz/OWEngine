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
//  File name:   autoCmd.cpp
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Automatic cmd system implementation
//               NOTE: This .cpp file must be included in EVERY module
//                     which is using automatic cmds.
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#include "autoCmd.h"
#include <api/coreAPI.h>

static aCmd_c* module_autoCmds = 0;

aCmd_c::aCmd_c( const char* newName, consoleCommandFunc_t newFunc )
{
    this->cmdName = newName;
    this->function = newFunc;
    
    this->nextModuleCMD = module_autoCmds;
    module_autoCmds = this;
}
void AUTOCMD_RegisterAutoConsoleCommands()
{
    aCmd_c* c = module_autoCmds;
    while( c )
    {
        g_core->Cmd_AddCommand( c->cmdName, c->function );
        c = c->nextModuleCMD;
    }
}
void AUTOCMD_UnregisterAutoConsoleCommands()
{
    aCmd_c* c = module_autoCmds;
    while( c )
    {
        g_core->Cmd_RemoveCommand( c->cmdName );
        c = c->nextModuleCMD;
    }
}