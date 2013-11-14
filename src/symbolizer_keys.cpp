/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2013 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#include <mapnik/symbolizer.hpp>
#include <mapnik/color.hpp>
#include <mapnik/gamma_method.hpp>
#include <mapnik/stroke.hpp>
#include <mapnik/line_symbolizer.hpp>

namespace mapnik {

static const std::tuple<const char*, mapnik::symbolizer_base::value_type> key_meta[MAX_SYMBOLIZER_KEY] =
{
    { "gamma", 1.0},
    { "gamma-method", static_cast<value_integer>(GAMMA_POWER)},
    { "opacity", 1.0},
    { "alignment", static_cast<value_integer>(0)},
    { "offset", 0.0},
    { "comp-op", static_cast<value_integer>(src_over)},
    { "clip", false},
    { "fill", mapnik::color("gray")},
    { "fill-opacity", 1.0 },
    { "stroke", mapnik::color("black")},
    { "stroke-width", 1.0 },
    { "stroke-opacity", 1.0},
    { "stroke-linejoin", static_cast<value_integer>(MITER_JOIN)},
    { "stroke-linecap", static_cast<value_integer>(BUTT_CAP)},
    { "stroke-gamma", 1.0},
    { "stroke-gamma-method",static_cast<value_integer>(GAMMA_POWER)},
    { "stroke-dashoffset", static_cast<value_integer>(0)},
    { "stroke-dasharray", false},
    { "stroke-miterlimit", 4.0},
    { "transform", false},
    { "rasterizer-mode", static_cast<value_integer>(RASTERIZER_FULL)},
    { "image-transform", false},
    { "spacing", 0.0},
    { "max-error", 0.0},
    { "allow-overlap",false},
    { "ignore-placement", false},
    { "width",static_cast<value_integer>(0)},
    { "height",static_cast<value_integer>(0)},
    { "file", ""},
    { "filename", ""},
    { "shield-dx", 0.0},
    { "shield-dy", 0.0},
    { "unlock-image",false},
    { "text-opacity", 1.0},
    { "mode",false},
    { "scaling", 1.0},
    { "filter-factor", 1.0},
    { "mesh-size", static_cast<value_integer>(0)},
    { "premultiplied", false},
    { "smooth", false},
    { "simplify-algorithm", static_cast<value_integer>(0)},
    { "simplify-tolerance", 0.0},
    { "halo-rasterizer", false},
    { "text-placements", false}
};

std::tuple<const char*, mapnik::symbolizer_base::value_type> get_meta(mapnik::keys key)
{
   return key_meta[key];
}

}
