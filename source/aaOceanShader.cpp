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
#include "XaaOceanShader.h"
#include "aaOceanC4D.h"
#include "DaaOceanC4D.h"
#include "utils.h"


class aaOceanShader : public ShaderData
{
private:
  aaOceanC4D _ocean;
  Bool _foam;

public: // ShaderData overrides
  INITRENDERRESULT InitRender(BaseShader* bs, InitRenderStruct const& irs) override
  {
      if (!bs) return INITRENDERRESULT::UNKNOWNERROR;
    BaseContainer* bc = bs->GetDataInstance();
    if (!bc) {
        return INITRENDERRESULT::UNKNOWNERROR;
    }
    Float time = (irs.doc ? irs.doc->GetTime().Get() : 0.0);
    _ocean.Init(*bc, time);
    _foam = bc->GetBool(AAOCEANC4D_FOAM);
      return INITRENDERRESULT::OK;
  }

  void FreeRender(BaseShader* bs) override {
    _ocean.Free();
  }

  Vector Output(BaseShader* bs, ChannelData* cd) override {
    Vector const uv = cd->p;
    Vector color;

    if (_foam) {
      color = Vector(1.0 - _ocean.GetOceanData(uv.x, uv.y, aaOceanC4D::Foam));
    }
    else {
      color.y = _ocean.GetOceanData(uv.x, uv.y, aaOceanC4D::Heightfield);
      if (_ocean.IsChoppy()) {
        color.x = _ocean.GetOceanData(uv.x, uv.y, aaOceanC4D::ChopX);
        color.z = _ocean.GetOceanData(uv.x, uv.y, aaOceanC4D::ChopZ);
      }
      // TODO: Remapping of values to range 0..1
    }

    return color;
  }

public: // NodeData Overrides
  Bool Init(GeListNode* node) override {
    if (!node) return false;
    utils::InitContainerDefaults((BaseList2D*)node);
    return true;
  }

public:
  static NodeData* Alloc()
  {
    return NewObjClear(aaOceanShader);
  }
};


Bool RegisterAaOceanShader() {
  Int32 const info = 0;
  return RegisterShaderPlugin(
    XaaOceanShader,
    "aaOceanShader"_s,
    info,
    aaOceanShader::Alloc,
    "XaaOceanShader"_s,
    0);
}
