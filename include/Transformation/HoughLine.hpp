/*
 *  jimlib -- generic image and-image algorithms library
 *  Copyright (C) 2015 Alexey Titov
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *
 *  Alexey Titov
 *  alex.justes@gmail.com
 *  https://github.com/alex-justes/jimlib
 */

#ifndef JIMLIB_HOUGHLINE_HPP
#define JIMLIB_HOUGHLINE_HPP

#include <cmath>
#include "Image/PixelTypes.hpp"
#include "Image/BinaryImage.hpp"
#include "Utils/Search.hpp"

namespace jimlib
{
    class HoughLine : public GenericImage<PixelType::Mono32>
    {
    public:
        void Calculate(const BinaryImage &Src, float min_angle, float max_angle, float angle_step, float min_distance, float max_distance, float distance_step, int32_t norm = 0);
    private:
        float deg2rad(float angle);
    };
    
    float HoughLine::deg2rad(float angle)
    {
        return (angle*M_PI/180.0);
    }
    
    void HoughLine::Calculate(const BinaryImage &Src, float min_angle, float max_angle, float angle_step, float min_distance, float max_distance, float distance_step, int32_t norm)
    {
        assert(max_angle > min_angle);
        assert(max_distance > min_distance);
        assert(min_distance > 0);
        int32_t W = Src.GetWidth();
        int32_t H = Src.GetHeight();
        float angles = max_angle - min_angle;
        float distances = max_distance - min_distance;
        int32_t angle_steps = (int32_t)(.5 + angles / angle_step);
        int32_t distance_steps = (int32_t)(.5 + distances / distance_step);
        this->Create(angle_steps, distance_steps, PixelType::Mono32(0));
        auto it_src = Src.begin();
        for (int32_t y = 0; y < H; ++y)
        {
            for (int32_t x = 0; x < W; ++x, ++it_src)
            {
                if (it_src[0] > 0)
                {
                    for (int32_t astep = 0; astep < angle_steps; ++astep)
                    {
                        float a = deg2rad(angle_step * astep + min_angle);
                        int32_t d = (int32_t) (.5 + (x * cos(a) + y * sin(a) - min_distance) * distance_step);
                        if (d > 0 && d < distance_steps)
                        {
                            this->GetColRow(astep, d)[0] += 1;
                        }
                    }
                }
            }
        }
        if (norm > 0)
        {
            int32_t max_ = max(*this);
            for (auto it = this->begin(); it != this->end(); ++it)
            {
                it[0] = (norm*it[0]/max_);
            }
        }
    }

}

#endif //JIMLIB_HOUGHLINE_HPP
