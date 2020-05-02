/**
 * Port of Amaan Akram's aaOcean suite for Cinema 4D.
 * Copyright (C) 2017  Niklas Rosenstein
 * Copyright (C) 2020  Kent Barber
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "c4d.h"
#include "aaOceanC4D.h"

extern Bool RegisterAaOceanDeformer();
extern Bool RegisterAaOceanShader();

Bool PluginStart() {
  GeConsoleOut("aaOcean by Amaan Akram -- Ported to C4D by Niklas Rosenstein. Converted to R21+ by Kent Barber."_s);
  RegisterAaOceanDescription();
  RegisterAaOceanDeformer();
  RegisterAaOceanShader();
  return true;
}

Bool PluginMessage(Int32 msg, void* pdata) {
  switch (msg) {
    case C4DPL_INIT_SYS:
          if (!g_resource.Init()) return false; // don't start plugin without resource
          return true;
  }
  return true;
}

void PluginEnd() {
}
