#include <mapnik/offset_converter.hpp>
#include <mapnik/geometry.hpp>
#include <mapnik/ctrans.hpp>
#include "agg_conv_clip_polyline.h"


namespace mapnik
{

    static double explement_reflex_angle(double angle)
    {
        if (angle > pi)
        {
            return angle - 2 * pi;
        }
        else if (angle < -pi)
        {
            return angle + 2 * pi;
        }
        else
        {
            return angle;
        }
    }

    static bool intersection(vertex2d const& u1, vertex2d const& u2, double* ut,
                             vertex2d const& v1, vertex2d const& v2, double* vt)
    {
        double const dx = v1.x - u1.x;
        double const dy = v1.y - u1.y;
        double const ux = u2.x - u1.x;
        double const uy = u2.y - u1.y;
        double const vx = v2.x - v1.x;
        double const vy = v2.y - v1.y;

        // the first line is not vertical
        if (ux < -1e-6 || ux > 1e-6)
        {
            double const up = ux * dy - dx * uy;
            double const dn = vx * uy - ux * vy;

            if (dn > -1e-6 && dn < 1e-6)
            {
                return false; // they are parallel
            }

            *vt = up / dn;
            *ut = (*vt * vx + dx) / ux;
            return true;
        }

        // the first line is not horizontal
        if (uy < -1e-6 || uy > 1e-6)
        {
            double const up = uy * dx - dy * ux;
            double const dn = vy * ux - uy * vx;

            if (dn > -1e-6 && dn < 1e-6)
            {
                return false; // they are parallel
            }

            *vt = up / dn;
            *ut = (*vt * vy + dy) / uy;
            return true;
        }

        // the first line is too short
        return false;
    }

    template <typename T>
    offset_converter<T>::offset_converter(T & geom)
        : geom_(geom)
        , offset_(0.0)
        , threshold_(12.0)
        , half_turn_segments_(32)
        , status_(initial)
        , pre_first_(vertex2d::no_init)
        , pre_(vertex2d::no_init)
        , cur_(vertex2d::no_init)
        {}

    template <typename T>
    unsigned offset_converter<T>::output_vertex(double* px, double* py)
    {
        *px = cur_.x;
        *py = cur_.y;
        return cur_.cmd;
    }

    template <typename T>
    void offset_converter<T>::displace(vertex2d & v, vertex2d const& u, double a) const
    {
        v.x = u.x + offset_ * std::sin(a);
        v.y = u.y - offset_ * std::cos(a);
        v.cmd = u.cmd;
    }

    template <typename T>
    void offset_converter<T>::push_vertex(vertex2d const& v)
    {
        #ifdef MAPNIK_LOG
        MAPNIK_LOG_DEBUG(ctrans) << "offset_converter: " << v;
        #endif

        if (vertices_.size()) {
            vertex2d const& last = vertices_.at(vertices_.size()-1);
            if (!((std::fabs(last.x - v.x) < 1e-1) && (std::fabs(last.y - v.y) < 1e-1)))
            {
                vertices_.push_back(v);
            }
        } else {
            vertices_.push_back(v);
        }
    }

    template <typename T>
    typename offset_converter<T>::status offset_converter<T>::init_vertices()
    {
        if (status_ != initial) // already initialized
        {
            return status_;
        }

        vertex2d v1(vertex2d::no_init);
        vertex2d v2(vertex2d::no_init);
        vertex2d w(vertex2d::no_init);

        v1.cmd = geom_.vertex(&v1.x, &v1.y);
        v2.cmd = geom_.vertex(&v2.x, &v2.y);

        if (v2.cmd == SEG_END) // not enough vertices in source
        {
            return status_ = process;
        }

        double angle_a = 0;
        double angle_b = std::atan2((v2.y - v1.y), (v2.x - v1.x));
        double joint_angle;

        // first vertex
        displace(v1, angle_b);
        push_vertex(v1);

        // Sometimes when the first segment is too short, it causes ugly
        // curls at the beginning of the line. To avoid this, we make up
        // a fake vertex two offset-lengths before the first, and expect
        // intersection detection smoothes it out.
        pre_first_ = v1;
        displace(pre_first_, -2 * std::fabs(offset_), 0, angle_b);

        while ((v1 = v2, v2.cmd = geom_.vertex(&v2.x, &v2.y)) != SEG_END)
        {
            angle_a = angle_b;
            angle_b = std::atan2((v2.y - v1.y), (v2.x - v1.x));
            joint_angle = explement_reflex_angle(angle_b - angle_a);

            double half_turns = half_turn_segments_ * std::fabs(joint_angle);
            int bulge_steps = 0;

            if (offset_ < 0.0)
            {
                if (joint_angle > 0.0)
                {
                    joint_angle = joint_angle - 2 * pi;
                }
                else
                {
                    bulge_steps = 1 + static_cast<int>(std::floor((half_turns / pi)+.5));
                }
            }
            else
            {
                if (joint_angle < 0.0)
                {
                    joint_angle = joint_angle + 2 * pi;
                }
                else
                {
                    bulge_steps = 1 + static_cast<int>(std::floor((half_turns / pi)+.5));
                }
            }

            #ifdef MAPNIK_LOG
            if (bulge_steps == 0)
            {
                // inside turn (sharp/obtuse angle)
                MAPNIK_LOG_DEBUG(ctrans) << "offset_converter:"
                    << " Sharp joint [<< inside turn " << int(joint_angle*180/pi)
                    << " degrees >>]";
            }
            else
            {
                // outside turn (reflex angle)
                MAPNIK_LOG_DEBUG(ctrans) << "offset_converter:"
                    << " Bulge joint >)) outside turn " << int(joint_angle*180/pi)
                    << " degrees ((< with " << bulge_steps << " segments";
            }
            #endif

            if (bulge_steps == 0) {
                displace(w, v1, angle_a);
                push_vertex(w);
            } else {
                for (int s = 0; ++s < bulge_steps;)
                {
                    //std::clog << (joint_angle * s) / bulge_steps << "\n";
                    displace(w, v1, angle_a + (joint_angle * s) / bulge_steps);
                    push_vertex(w);
                }
            }
            displace(v1, angle_b);
            push_vertex(v1);
        }

        // last vertex
        displace(v1, angle_b);
        push_vertex(v1);

        // initialization finished
        return status_ = process;
    }

    template <typename T>
    unsigned offset_converter<T>::vertex(double * x, double * y)
    {
        if (offset_ == 0.0)
        {
            return geom_.vertex(x, y);
        }

        if (status_ == initial)
        {
            init_vertices();
        }

        if (pos_ >= vertices_.size())
        {
            return SEG_END;
        }

        pre_ = (pos_ ? cur_ : pre_first_);
        cur_ = vertices_.at(pos_++);

        if (pos_ == vertices_.size())
        {
            return output_vertex(x, y);
        }

        double const check_dist = offset_ * threshold_;
        double const check_dist2 = check_dist * check_dist;
        double t = 1.0;
        double vt, ut;

        for (size_t i = pos_; i+1 < vertices_.size(); ++i)
        {
            //break; // uncomment this to see all the curls

            vertex2d const& u0 = vertices_[i];
            vertex2d const& u1 = vertices_[i+1];
            double const dx = u0.x - cur_.x;
            double const dy = u0.y - cur_.y;

            if (dx*dx + dy*dy > check_dist2)
            {
                break;
            }

            if (!intersection(pre_, cur_, &vt, u0, u1, &ut))
            {
                continue;
            }

            if (vt < 0.0 || vt > t || ut < 0.0 || ut > 1.0)
            {
                continue;
            }

            t = vt;
            pos_ = i+1;
        }

        cur_.x = pre_.x + t * (cur_.x - pre_.x);
        cur_.y = pre_.y + t * (cur_.y - pre_.y);
        return output_vertex(x, y);
    }

    template struct offset_converter<agg::conv_clip_polyline<mapnik::geometry_type> >;
    template struct offset_converter<geometry_type>;
    template struct offset_converter<coord_transform<CoordTransform,mapnik::geometry_type> >;
    template struct offset_converter<coord_transform<CoordTransform,agg::conv_clip_polyline<mapnik::geometry_type> > >;

}