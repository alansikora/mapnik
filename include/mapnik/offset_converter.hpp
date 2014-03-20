/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2012 Artem Pavlenko
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

#ifndef MAPNIK_OFFSET_CONVERTER_HPP
#define MAPNIK_OFFSET_CONVERTER_HPP

#ifdef MAPNIK_LOG
#include <mapnik/debug.hpp>
#endif
#include <mapnik/config.hpp>
#include <mapnik/box2d.hpp>
#include <mapnik/vertex.hpp>
#include <mapnik/proj_transform.hpp>

// boost
#include <boost/math/constants/constants.hpp>

// stl
#include <cmath>

namespace mapnik
{

const double pi = boost::math::constants::pi<double>();

template <typename Geometry>
struct MAPNIK_DECL offset_converter
{
    typedef std::size_t size_type;

    offset_converter(Geometry & geom);

    enum status
    {
        initial,
        process
    };

    double get_offset() const
    {
        return offset_;
    }

    double get_threshold() const
    {
        return threshold_;
    }

    void set_offset(double value)
    {
        if (offset_ != value)
        {
            offset_ = value;
            reset();
        }
    }

    void set_threshold(double value)
    {
        threshold_ = value;
        // no need to reset(), since threshold doesn't affect
        // offset vertices' computation, it only controls how
        // far will we be looking for self-intersections
    }

    unsigned vertex(double * x, double * y);

    void reset()
    {
        geom_.rewind(0);
        vertices_.clear();
        status_ = initial;
        pos_ = 0;
    }

    void rewind(unsigned)
    {
        pos_ = 0;
    }

private:

    /**
     *  @brief  Translate (vx, vy) by rotated (dx, dy).
     */
    static void displace(vertex2d & v, double dx, double dy, double a)
    {
        v.x += dx * std::cos(a) - dy * std::sin(a);
        v.y += dx * std::sin(a) + dy * std::cos(a);
    }

    /**
     *  @brief  Translate (vx, vy) by rotated (0, -offset).
     */
    void displace(vertex2d & v, double a) const
    {
        v.x += offset_ * std::sin(a);
        v.y -= offset_ * std::cos(a);
    }

    /**
     *  @brief  (vx, vy) := (ux, uy) + rotated (0, -offset)
     */
    void displace(vertex2d & v, vertex2d const& u, double a) const;

    void displace2(vertex2d & v, double a, double b) const
    {
        double sa = offset_ * std::sin(a);
        double ca = offset_ * std::cos(a);
        double h = std::tan(0.5 * (b - a));
        v.x = v.x + sa + h * ca;
        v.y = v.y - ca + h * sa;
    }

    status init_vertices();

    unsigned output_vertex(double* px, double* py);

    unsigned output_vertex(double* px, double* py, status st)
    {
        status_ = st;
        return output_vertex(px, py);
    }

    void push_vertex(vertex2d const& v);

    Geometry &              geom_;
    double                  offset_;
    double                  threshold_;
    unsigned                half_turn_segments_;
    status                  status_;
    size_t                  pos_;
    std::vector<vertex2d>   vertices_;
    vertex2d                pre_first_;
    vertex2d                pre_;
    vertex2d                cur_;
};

}

#endif // MAPNIK_OFFSET_CONVERTER_HPP
