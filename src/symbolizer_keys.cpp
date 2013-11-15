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

namespace mapnik {

// name, default value, enumeration to string converter lambda
static const std::tuple<char const*, symbolizer_base::value_type, std::function<std::string(value_integer)> > key_meta[MAX_SYMBOLIZER_KEY] =
{
    { "gamma", 1.0, nullptr},
    { "gamma-method", static_cast<value_integer>(GAMMA_POWER), nullptr},
    { "opacity", 1.0, nullptr},
    { "alignment", static_cast<value_integer>(0), nullptr},
    { "offset", 0.0, nullptr},
    { "comp-op", static_cast<value_integer>(src_over), [](value_integer val) { return *comp_op_to_string(composite_mode_e(val)); }},
    { "clip", false, nullptr},
    { "fill", mapnik::color("gray"), nullptr},
    { "fill-opacity", 1.0 , nullptr},
    { "stroke", mapnik::color("black"), nullptr},
    { "stroke-width", 1.0 , nullptr},
    { "stroke-opacity", 1.0, nullptr},
    { "stroke-linejoin", static_cast<value_integer>(MITER_JOIN),
      [](value_integer val) { return enumeration<line_join_enum,line_join_enum_MAX>(line_join_enum(val)).as_string();}},
    { "stroke-linecap", static_cast<value_integer>(BUTT_CAP),
      [](value_integer val) { return enumeration<line_cap_enum,line_cap_enum_MAX>(line_cap_enum(val)).as_string();}},
    { "stroke-gamma", 1.0, nullptr},
    { "stroke-gamma-method",static_cast<value_integer>(GAMMA_POWER), nullptr},
    { "stroke-dashoffset", static_cast<value_integer>(0), nullptr},
    { "stroke-dasharray", false, nullptr},
    { "stroke-miterlimit", 4.0, nullptr},
    { "transform", false, nullptr},
    { "rasterizer-mode", static_cast<value_integer>(RASTERIZER_FULL), nullptr},
    { "image-transform", false, nullptr},
    { "spacing", 0.0, nullptr},
    { "max-error", 0.0, nullptr},
    { "allow-overlap",false, nullptr},
    { "ignore-placement", false, nullptr},
    { "width",static_cast<value_integer>(0), nullptr},
    { "height",static_cast<value_integer>(0), nullptr},
    { "file", "", nullptr},
    { "filename", "", nullptr},
    { "shield-dx", 0.0, nullptr},
    { "shield-dy", 0.0, nullptr},
    { "unlock-image",false, nullptr},
    { "text-opacity", 1.0, nullptr},
    { "mode",false, nullptr},
    { "scaling", 1.0, nullptr},
    { "filter-factor", 1.0, nullptr},
    { "mesh-size", static_cast<value_integer>(0), nullptr},
    { "premultiplied", false, nullptr},
    { "smooth", false, nullptr},
    { "simplify-algorithm", static_cast<value_integer>(0), nullptr},
    { "simplify-tolerance", 0.0, nullptr},
    { "halo-rasterizer", false, nullptr},
    { "text-placements", false, nullptr}
};

std::tuple<const char*, mapnik::symbolizer_base::value_type, std::function<std::string(mapnik::value_integer)> > const& get_meta(mapnik::keys key)
{
   return key_meta[key];
}

}
