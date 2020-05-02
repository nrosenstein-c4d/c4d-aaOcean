/**
 * Port of Amaan Akram's aaOcean suite for Cinema 4D.
 * Copyright (C) 2017  Niklas Rosenstein
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

#include "aaOceanC4D.h"
#include "aaOceanClass.cpp"
#include "DaaOceanC4D.h"

aaOceanC4D::aaOceanC4D()
: _oc(nullptr)
{

}

aaOceanC4D::~aaOceanC4D() {
  DeleteObj(_oc);
}

Bool aaOceanC4D::Init(BaseContainer const& bc, Float time)
{
    if (!_oc)
    {
        _oc = NewObjClear(aaOcean);
        if (!_oc)
        {
            return false;
        }
    }
    
  _oc->input(
    bc.GetInt32(AAOCEANC4D_RESOLUTION),
    bc.GetInt32(AAOCEANC4D_SPECTRUM),
    bc.GetInt32(AAOCEANC4D_SEED),
    bc.GetFloat(AAOCEANC4D_OCEANSCALE),
    bc.GetFloat(AAOCEANC4D_OCEANDEPTH),
    bc.GetFloat(AAOCEANC4D_SURFACETENSION),
    bc.GetFloat(AAOCEANC4D_VELOCITY),
    bc.GetFloat(AAOCEANC4D_CUTOFF),
    RadToDeg(bc.GetFloat(AAOCEANC4D_WINDDIRECTION)),
    bc.GetInt32(AAOCEANC4D_WINDALIGN),
    bc.GetFloat(AAOCEANC4D_DAMP),
    bc.GetFloat(AAOCEANC4D_WAVESPEED),
    bc.GetFloat(AAOCEANC4D_WAVEHEIGHT),
    bc.GetFloat(AAOCEANC4D_CHOPAMOUNT),
    bc.GetFloat(AAOCEANC4D_TIME) + time,
    bc.GetFloat(AAOCEANC4D_REPEATTIME),
    bc.GetBool(AAOCEANC4D_FOAM),
    bc.GetFloat(AAOCEANC4D_RANDWEIGHT),
    bc.GetFloat(AAOCEANC4D_SPECTRUMWEIGHT),
    bc.GetFloat(AAOCEANC4D_PMWAVESIZE),
    bc.GetFloat(AAOCEANC4D_JSWPFETCH),
    bc.GetFloat(AAOCEANC4D_SWELL)
  );
  // FIXME: Check if initialization was successful.
  return true;
}

void aaOceanC4D::Free()
{
  // FIXME
  //_oc->clearArrays();
}

Float aaOceanC4D::GetOceanData(Float uCoord, Float vCoord, ArrayType type) const
{
  return _oc->getOceanData(uCoord, vCoord, (aaOcean::arrayType) type);
}

Bool aaOceanC4D::IsChoppy() {
  return _oc->isChoppy();
}

Bool RegisterAaOceanDescription() {
  return RegisterDescription(DaaOceanC4D_Group, "DaaOceanC4D"_s);
}
