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
#include <mapnik/simplify.hpp>

namespace mapnik {

// tuple -> name, default value, enumeration to string converter lambda
static const property_meta_type key_meta[MAX_SYMBOLIZER_KEY] =
{
    { "gamma", 1.0, nullptr},
    { "gamma-method", static_cast<value_integer>(GAMMA_POWER), nullptr},
    { "opacity", 1.0, nullptr},
    { "alignment", enumeration_wrapper(LOCAL_ALIGNMENT),
      [](enumeration_wrapper e) { return enumeration<pattern_alignment_enum,pattern_alignment_enum_MAX>(pattern_alignment_enum(e.value)).as_string();}},
    { "offset", 0.0, nullptr},
    { "comp-op", enumeration_wrapper(src_over),
      [](enumeration_wrapper e) { return *comp_op_to_string(composite_mode_e(e.value)); }},
    { "clip", false, nullptr},
    { "fill", mapnik::color("gray"), nullptr},
    { "fill-opacity", 1.0 , nullptr},
    { "stroke", mapnik::color("black"), nullptr},
    { "stroke-width", 1.0 , nullptr},
    { "stroke-opacity", 1.0, nullptr},
    { "stroke-linejoin", enumeration_wrapper(MITER_JOIN),
      [](enumeration_wrapper e) { return enumeration<line_join_enum,line_join_enum_MAX>(line_join_enum(e.value)).as_string();}},
    { "stroke-linecap", enumeration_wrapper(BUTT_CAP),
      [](enumeration_wrapper e) { return enumeration<line_cap_enum,line_cap_enum_MAX>(line_cap_enum(e.value)).as_string();}},
    { "stroke-gamma", 1.0, nullptr},
    { "stroke-gamma-method",static_cast<value_integer>(GAMMA_POWER), nullptr},
    { "stroke-dashoffset", static_cast<value_integer>(0), nullptr},
    { "stroke-dasharray", false, nullptr},
    { "stroke-miterlimit", 4.0, nullptr},
    { "transform", false, nullptr},
    { "rasterizer-mode", enumeration_wrapper(RASTERIZER_FULL),
      [](enumeration_wrapper e) { return enumeration<line_rasterizer_enum,line_rasterizer_enum_MAX>(line_rasterizer_enum(e.value)).as_string();}},
    { "image-transform", false, nullptr},
    { "spacing", 0.0, nullptr},
    { "max-error", 0.0, nullptr},
    { "allow-overlap",false, nullptr},
    { "ignore-placement", false, nullptr},
    { "width",static_cast<value_integer>(0), nullptr},
    { "height",static_cast<value_integer>(0), nullptr},
    { "file", "", nullptr},
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
    { "simplify-algorithm", enumeration_wrapper(radial_distance),
      [](enumeration_wrapper e) { return *simplify_algorithm_to_string(simplify_algorithm_e(e.value));}},
    { "simplify-tolerance", 0.0, nullptr},
    { "halo-rasterizer", enumeration_wrapper(HALO_RASTERIZER_FULL),
      [](enumeration_wrapper e) { return enumeration<halo_rasterizer_enum,halo_rasterizer_enum_MAX>(halo_rasterizer_enum(e.value)).as_string();}},
    { "text-placements", false, nullptr},
    { "placement", enumeration_wrapper(MARKER_POINT_PLACEMENT),
      [](enumeration_wrapper e) { return enumeration<marker_placement_enum,marker_placement_enum_MAX>(marker_placement_enum(e.value)).as_string();}}, // FIXME - rename to "markers-placement-type"
    { "multi-policy", enumeration_wrapper(MARKER_EACH_MULTI),
      [](enumeration_wrapper e) { return enumeration<marker_multi_policy_enum,marker_multi_policy_enum_MAX>(marker_multi_policy_enum(e.value)).as_string();}}, // FIXME - better naming ^^
    { "point-placement-type", enumeration_wrapper(MARKER_POINT_PLACEMENT),
      [](enumeration_wrapper e) { return enumeration<marker_placement_enum,marker_placement_enum_MAX>(marker_placement_enum(e.value)).as_string();}},
    { "raster-colorizer", nullptr, nullptr }
};

property_meta_type const& get_meta(mapnik::keys key)
{
   return key_meta[key];
}

}
