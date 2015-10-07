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
 *    misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *
 *  Alexey Titov
 *  alex.justes@gmail.com
 *  https://github.com/alex-justes/jimlib
 */

/*!
 *  \file
 *  \brief GenericImage<PixelType> class specification.
 *  \author Alexey Titov
 *  \date September 2015
 *  \copyright zlib
 *  
 *  Full specification of the GenericImage<PixelType> class and underlying iterator.
 */

#ifndef GENERICIMAGE_H
#define GENERICIMAGE_H
#include <cassert>
#include <cstdint>
#include "Utils/CheckTypes.h"
#include "GenericPixel.h"

using namespace std;

/*!
  \brief Class GenericImage<PixelType> provides general-purpose generic interface for storing
  and processing different image types. 
   
  The main goal of the GenericImage<PixelType> is to provide generic, fast and as compile-time type safe
  as it could be interface for storing and processing different data which can be represented as 
  matrix of N-dimensions vectors.
  
  GenericImage<PixelType> provides dynamicaly allocated storage with static compile-time checks to
  ensure that all operations are type-safe (as much as possible).
  
  It uses no overhead in storing complicated data-types and provides simple iterator-like interface
  for accessing underlying data.
 
  Before speaking about in-memory representation let's define some things:
  1. Image is a set of Pixels.
  2. Pixel is a set of the fixed amount of the fixed-sized Plants.
  3. Plant is a fixed-sized storage for any data-type.
  4. Size of the Plant in bits should be divisible by 8 (integer amount of bytes).
  
  For example RGB24 pixel has 3 plants: R, G, B, each plant occupies 8 bits.
  
  Those definitions are not limiting you to use it only for image types:
  Consider you need to hold some complicated data in the matrix form, i.e. vectors with N dimensions.
  You can define Plant as a dimension of the vector(dimension could be any type, i.e. double or 
  any abstract type), so Pixel would hold the entire vector(N dimensions).
  
  The only restriction is the Size of the Plant. For example if you need less than a byte to hold
  value(i.e. binary image) you need to use the smallest sized plant - 1 byte plant.
  
  All needed memory allocated dynamicaly and in one piece. Images' in-memory representation is linear 
  row-based set of pixels. So, provided X and Y as coordinates of some pixel in the Image the desired
  location would be Y * Width * sizeof(Pixel) + X.
  
  The internal representaion of the underlying iterator is simple pointer to some location in the Image memory,
  so using iterators for a linear access shows no overhead.
  
  Random access is a little slower but still pretty fast (just because we need to compute Y * Width * sizeof(Pixel) + X 
  every time).
 */
 
template<typename Pixel>
class GenericImage
{
public:
    GenericImage();
    virtual ~GenericImage();
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    uint32_t GetPlants() const;
    const typename Pixel::Type GetPixel(uint32_t x, uint32_t y, uint8_t Plant) const;
    void SetPixel(uint32_t x, uint32_t y, uint8_t Plant, const typename Pixel::Type &Value);
    const Pixel GetPixel(uint32_t x, uint32_t y) const;
    void SetPixel(uint32_t x, uint32_t y, const Pixel &Value);
    void Create(uint32_t Width, uint32_t Height);
    void Create(uint32_t Width, uint32_t Height, const Pixel &Value);
    class iterator
    {
    public:
        iterator();
        iterator(uint8_t * pRawData, uint32_t SizeOfPixel, uint32_t idx);
        typename Pixel::Type * operator*();
        iterator &operator++();
        iterator &operator--();
        typename Pixel::Type &operator [] (uint8_t Plant);
        bool operator==(const iterator &it) const;
        bool operator!=(const iterator &it) const;
    private:
        uint32_t m_SizeOfPixel;
        uint8_t * m_RawData;
    };
    const iterator begin() const;
    const iterator end() const;
    const iterator GetRow(uint32_t Row) const;
    const iterator GetColRow(uint32_t Col, uint32_t Row) const;
protected:
    void CopyInternal(const GenericImage<Pixel> &Src);
    void AllocateRawData(uint32_t Width, uint32_t Height);
    void DeleteRawData();
    const uint32_t m_SizeOfPixel;
    const uint32_t m_SizeOfPlant;
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_BufSize;
    uint32_t m_Offset;
    uint8_t * m_RawData;
    const uint8_t m_Plants;
};

template <typename Pixel>
GenericImage<Pixel>::GenericImage()
: m_Width(0),
  m_Height(0),
  m_BufSize(0),
  m_Offset(0),
  m_SizeOfPlant(sizeof(typename Pixel::Type)),
  m_Plants(Pixel::Plants),
  m_SizeOfPixel(Pixel::Plants * sizeof(typename Pixel::Type)),
  m_RawData(nullptr)
{
    static_assert( CheckTypes<  GenericPixel<typename Pixel::Type, Pixel::Plants>, 
                                typename Pixel::ParentType >::areSame, 
                                "Type is not GenericPixel<T, unsigned int Plants>!" );
}
template <typename Pixel>
GenericImage<Pixel>::~GenericImage()
{
    if (m_RawData)
    {
        delete [] m_RawData;
    }
}
template <typename Pixel>
void GenericImage<Pixel>::Create(uint32_t Width, uint32_t Height)
{
    AllocateRawData(Width, Height);
}
template <typename Pixel>
void GenericImage<Pixel>::Create(uint32_t Width, uint32_t Height, const Pixel &Value)
{
    Create(Width, Height);
    typename GenericImage<Pixel>::iterator it = begin();
    for(; it != end(); ++it)
    {
        for(uint8_t p = 0; p < m_Plants; ++p)
        {
            it[p] = Value[p];
        }
    }
}
template <typename Pixel>
void GenericImage<Pixel>::CopyInternal(const GenericImage<Pixel> &Src)
{
    Create(Src.GetWidth(), Src.GetHeight());
    memcpy(Src.m_RawData, m_RawData, m_BufSize);
}
template <typename Pixel>
void GenericImage<Pixel>::AllocateRawData(uint32_t Width, uint32_t Height)
{
    uint32_t DataSize = Width * Height * m_Plants * m_SizeOfPlant;
    m_Width = Width;
    m_Height = Height;
    m_Offset = m_Width * m_SizeOfPixel;
    if (DataSize != m_BufSize)
    {
        DeleteRawData();
        m_BufSize = DataSize;
        m_RawData = new uint8_t [m_BufSize];
    }
}
template <typename Pixel>
void GenericImage<Pixel>::DeleteRawData()
{
    if (m_RawData)
    {
        delete [] m_RawData;
        m_RawData = nullptr;
    }
}
template <typename Pixel>
unsigned int GenericImage<Pixel>::GetWidth() const
{
    return m_Width;
}
template <typename Pixel>
unsigned int GenericImage<Pixel>::GetHeight() const
{
    return m_Height;
}
template <typename Pixel>
unsigned int GenericImage<Pixel>::GetPlants() const
{
    return m_Plants;
}
template <typename Pixel>
const typename Pixel::Type GenericImage<Pixel>::GetPixel(uint32_t x, uint32_t y, uint8_t Plant) const
{
    assert(Plant < m_Plants);
    return *(reinterpret_cast<typename Pixel::Type *>(m_RawData + y * m_Offset + x * m_SizeOfPixel) + Plant);
}
template <typename Pixel>
const Pixel GenericImage<Pixel>::GetPixel(uint32_t x, uint32_t y) const
{
    Pixel pix;
    typename GenericImage<Pixel>::iterator it = GetColRow(x, y);
    for(uint8_t p = 0; p < m_Plants; ++p)
    {
        pix[p] = it[p];
    }
    return pix;
}
template <typename Pixel>
void GenericImage<Pixel>::SetPixel(uint32_t x, uint32_t y, uint8_t Plant, const typename Pixel::Type &Value)
{
    assert(Plant < m_Plants);
    *(reinterpret_cast<typename Pixel::Type *>(m_RawData + y * m_Offset + x * m_SizeOfPixel) + Plant) = Value;
}
template <typename Pixel>
void GenericImage<Pixel>::SetPixel(uint32_t x, uint32_t y, const Pixel &Value)
{
    typename GenericImage<Pixel>::iterator it = GetColRow(x, y);
    for(uint8_t p = 0; p < m_Plants; ++p)
    {
        it[p] = Value[p];
    }
}
template <typename Pixel>
const typename GenericImage<Pixel>::iterator GenericImage<Pixel>::begin() const
{
    GenericImage<Pixel>::iterator it(m_RawData, m_SizeOfPixel, 0);
    return it;
}
template <typename Pixel>
const typename GenericImage<Pixel>::iterator GenericImage<Pixel>::end() const
{
    GenericImage<Pixel>::iterator it(m_RawData, m_SizeOfPixel, m_BufSize);
    return it;
}
template <typename Pixel>
const typename GenericImage<Pixel>::iterator GenericImage<Pixel>::GetRow(uint32_t Row) const
{
    GenericImage<Pixel>::iterator it(m_RawData, m_SizeOfPixel, m_Offset * Row);
    return it;
}
template <typename Pixel>
const typename GenericImage<Pixel>::iterator GenericImage<Pixel>::GetColRow(uint32_t Col, uint32_t Row) const
{
    GenericImage<Pixel>::iterator it(m_RawData, m_SizeOfPixel, m_Offset * Row + m_SizeOfPixel * Col);
    return it;
}
template <typename Pixel>
GenericImage<Pixel>::iterator::iterator()
: m_SizeOfPixel(0),
  m_RawData(nullptr)
{

}
template <typename Pixel>
GenericImage<Pixel>::iterator::iterator(uint8_t * pRawData, uint32_t SizeOfPixel, uint32_t idx)
: m_SizeOfPixel(SizeOfPixel),
  m_RawData(pRawData)
{
    assert(m_RawData != nullptr);
    m_RawData += idx;
}
template <typename Pixel>
typename GenericImage<Pixel>::iterator &GenericImage<Pixel>::iterator::operator++()
{
    assert(m_RawData != nullptr);
    m_RawData += m_SizeOfPixel;
    return (*this);
}
template <typename Pixel>
typename GenericImage<Pixel>::iterator &GenericImage<Pixel>::iterator::operator--()
{
    assert(m_RawData != nullptr);
    m_RawData -= m_SizeOfPixel;
    return (*this);
}
template <typename Pixel>
bool GenericImage<Pixel>::iterator::operator==(const typename GenericImage<Pixel>::iterator &it) const
{
    return (this->m_RawData == it.m_RawData);
}
template <typename Pixel>
bool GenericImage<Pixel>::iterator::operator!=(const typename GenericImage<Pixel>::iterator &it) const
{
    return (this->m_RawData != it.m_RawData);
}
template <typename Pixel>
typename Pixel::Type *GenericImage<Pixel>::iterator::operator*()
{
    return reinterpret_cast<typename Pixel::Type *>(m_RawData);
}
template <typename Pixel>
typename Pixel::Type &GenericImage<Pixel>::iterator::operator[](uint8_t Plant)
{
    assert(Plant < Pixel::Plants);
    assert(m_RawData != nullptr);
    return reinterpret_cast<typename Pixel::Type *>(m_RawData)[Plant];
}

#endif //GENERICIMAGE_H
