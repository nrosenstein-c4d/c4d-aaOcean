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
#include "OaaOceanDeformer.h"
#include "aaOceanC4D.h"
#include "utils.h"


class aaOceanDeformer : public ObjectData
{
private:
  aaOceanC4D _ocean;
  BaseTime _lastTime;

public: // ObjectData overrides
  void CheckDirty(BaseObject* op, BaseDocument* doc) override
  {
    if (_lastTime != doc->GetTime())
        op->SetDirty(DIRTYFLAGS::DATA);
  }

  Bool ModifyObject(BaseObject* mod, BaseDocument* doc, BaseObject* op, const Matrix& op_mg,
                    const Matrix& mod_mg, Float lod, Int32 flags, BaseThread* thread) override
  {
    if (!op->IsInstanceOf(Opolygon)) return true;

    auto* bc = mod->GetDataInstance();
    auto* uvwTag = (UVWTag*) op->GetTag(Tuvw);
    if (!uvwTag || !bc) return true;

    auto pointCount = ToPoly(op)->GetPointCount();
    auto* points = ToPoly(op)->GetPointW();
    auto polyCount = ToPoly(op)->GetPolygonCount();
    auto* polygons = ToPoly(op)->GetPolygonR();
    if (!polygons || polyCount <= 0) return true;

    Neighbor nb;
    if (!nb.Init(pointCount, polygons, polyCount, nullptr)) return false;

    _lastTime = doc->GetTime();
    _ocean.Init(*bc, _lastTime.Get());

    //bool foam = bc->GetBool(AAOCEANC4D_FOAM);
    //bool invert = false;  // TODO: Add parameter
    auto uvwHandle = uvwTag->GetDataAddressR();
    auto const transform = mod->GetMg();

    // Deform point positions.
    {
      Int32* dadr;
      Int32 dcnt;
      UVWStruct uvw;
      Vector worldPos;

      #pragma omp parallel for private (worldPos, dadr, dcnt, uvw)
      for (Int32 i = 0; i < pointCount; ++i) {
        // Figure out the UV coordinate of the point.
        nb.GetPointPolys(i, &dadr, &dcnt);
        if (dcnt <= 0) continue;

        auto& poly = polygons[dadr[0]];
        UVWTag::Get(uvwHandle, dadr[0], uvw);
        Vector uv;

        if (poly.a == i) uv = uvw.a;
        else if (poly.b == i) uv = uvw.b;
        else if (poly.c == i) uv = uvw.c;
        else if (poly.d == i) uv = uvw.d;
        else continue;

        worldPos.y = _ocean.GetOceanData(uv.x, uv.y, aaOceanC4D::Heightfield);
        if (_ocean.IsChoppy()) {
          worldPos.x = _ocean.GetOceanData(uv.x, uv.y, aaOceanC4D::ChopX);
          worldPos.z = _ocean.GetOceanData(uv.x, uv.y, aaOceanC4D::ChopZ);
        }
        points[i] += transform * worldPos;
      }
    }

    return true;
  }

public:
  Bool Init(GeListNode* node) override
  {
    if (!node || !ObjectData::Init(node)) return false;
    utils::InitContainerDefaults((BaseObject*)node);
    return true;
  }

public:
  static NodeData* Alloc()
  {
    return NewObjClear(aaOceanDeformer);
  }
};


Bool RegisterAaOceanDeformer()
{
  return RegisterObjectPlugin(
    OaaOceanDeformer,
    "aaOceanDeformer"_s,
    OBJECT_MODIFIER,
    aaOceanDeformer::Alloc,
    "OaaOceanDeformer"_s,
    AutoBitmap("aaoceandeformer.png"_s),
    0);
}
