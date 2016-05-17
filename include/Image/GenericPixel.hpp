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

#ifndef JIMLIB_GENERICPIXEL_HPP
#define JIMLIB_GENERICPIXEL_HPP

#include <cstdint>
#include <cassert>
namespace jimlib
{
    template<typename T, uint8_t PlantsAmount = 1>
    class GenericPixel
    {
    public:
        GenericPixel();
        GenericPixel(T * Buffer);
        GenericPixel(const GenericPixel &p);
        void CopyTo(GenericPixel &p);
        virtual ~GenericPixel();
        T &operator[](uint8_t Plant);
        const T &operator[](uint8_t Plant) const;
        static const uint8_t Plants = PlantsAmount;
        static const uint32_t SizeOfPixel = sizeof(T) * PlantsAmount;
        typedef T Type;
        typedef GenericPixel<T, PlantsAmount> ParentType;
        T * m_Buffer;
    private:
        bool m_Allocated;
    };

// =======================================================

    template<typename T, uint8_t PlantsAmount>
    GenericPixel<T, PlantsAmount>::GenericPixel()
    : m_Buffer(nullptr), m_Allocated(true)
    {
        m_Buffer = new T[PlantsAmount];
    }

    template<typename T, uint8_t PlantsAmount>
    GenericPixel<T, PlantsAmount>::GenericPixel(T * Buffer)
    : m_Buffer(Buffer), m_Allocated(false)
    {
    }

    template<typename T, uint8_t PlantsAmount>
    GenericPixel<T, PlantsAmount>::GenericPixel(const GenericPixel<T, PlantsAmount> &p)
    : m_Buffer(p.m_Buffer), m_Allocated(false)
    {
    }

    template<typename T, uint8_t PlantsAmount>
    GenericPixel<T, PlantsAmount>::~GenericPixel()
    {
        if (m_Allocated && m_Buffer != nullptr)
        {
            delete[] m_Buffer;
        }
    }

    template<typename T, uint8_t PlantsAmount>
    void GenericPixel<T, PlantsAmount>::CopyTo(GenericPixel &p)
    {
        memcpy(p.m_Buffer, m_Buffer, sizeof(T) * PlantsAmount);
    }
    template<typename T, uint8_t PlantsAmount>
    T &GenericPixel<T, PlantsAmount>::operator[](uint8_t Plant)
    {
        assert(Plant < PlantsAmount);
        return m_Buffer[Plant];
    }

    template<typename T, uint8_t PlantsAmount>
    const T &GenericPixel<T, PlantsAmount>::operator[](uint8_t Plant) const
    {
        assert(Plant < PlantsAmount);
        return m_Buffer[Plant];
    }
};
#endif //JIMLIB_GENERICPIXEL_HPP