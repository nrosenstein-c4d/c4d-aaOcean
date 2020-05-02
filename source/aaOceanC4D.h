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

#pragma once
#include "c4d.h"

class aaOcean;

class aaOceanC4D
{
private:
  aaOcean* _oc;

public:
  enum ArrayType {
    Heightfield,
    ChopX,
    ChopZ,
    Foam,            // require foam = true
    EigenplusX,      // require foam = true
    EigenplusZ,      // require foam = true
    EigenMinusX,     // require foam = true
    EigenMinusZ,     // require foam = true
    Spectrum
  };

  aaOceanC4D();
  ~aaOceanC4D();

  Bool Init(BaseContainer const& bc, Float time);

  void Free();

  Float GetOceanData(Float uCoord, Float vCoord, ArrayType type) const;

  Bool IsChoppy();
};

Bool RegisterAaOceanDescription();
