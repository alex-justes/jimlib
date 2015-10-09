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

#ifndef JIMLIB_GENERICIMAGE_H
#define JIMLIB_GENERICIMAGE_H

#include <cstring>
#include "Utils/CheckTypes.hpp"
#include "Image/GenericPixel.hpp"

using namespace std;

/*!
 * \brief Class GenericImage<PixelType> provides general-purpose generic interface for storing
 * and processing different image types.
 *
 * The main goal of the GenericImage<PixelType> is to provide generic, fast and as compile-time type safe
 * as it could be interface for storing and processing different data which can be represented as
 * matrix of N-dimensions vectors.
 *
 * GenericImage<PixelType> provides dynamicaly allocated storage with static compile-time checks to
 * ensure that all operations are type-safe (as much as possible).
 *
 * It uses no overhead in storing complicated data-types and provides simple iterator-like interface
 * for accessing underlying data.
 *
 * Before speaking about in-memory representation let's define some things:
 * 1. Image is a set of Pixels.
 * 2. Pixel is a set of the fixed amount of the fixed-sized Plants.
 * 3. Plant is a fixed-sized storage for any data-type.
 * 4. Size of the Plant in bits should be divisible by 8 (integer amount of bytes).
 *
 * For example RGB24 pixel has 3 plants: R, G, B, each plant occupies 8 bits.
 *
 * Those definitions are not limiting you to use it only for image types:
 * Consider you need to hold some complicated data in the matrix form, i.e. vectors with N dimensions.
 * You can define Plant as a dimension of the vector(dimension could be any type, i.e. double or
 * any abstract type), so Pixel would hold the entire vector(N dimensions).
 *
 * The only restriction is the Size of the Plant. For example if you need less than a byte to hold
 * value(i.e. binary image) you need to use the smallest sized plant - 1 byte plant.
 *
 * All needed memory allocated dynamicaly and in one piece. Images' in-memory representation is linear
 * row-based set of pixels. So, provided X and Y as coordinates of some pixel in the Image the desired
 * location would be Y * Width * sizeof(Pixel) + X.
 *
 * The internal representaion of the underlying iterator is simple pointer to some location in the Image memory,
 * so using iterators for a linear access shows no overhead.
 *
 * Random access is a little slower but still pretty fast (just because we need to compute Y * Width * sizeof(Pixel) + X
 * every time).
 */
 
template<typename Pixel>
class GenericImage
{
public:
    /*!
     * Empty default constructor. Checks that Pixel is derived from GenericPixel (static_assert).
     */
    GenericImage();

    /*!
     * Destructor. Frees allocated storage.
     */
    virtual ~GenericImage();

    /*!
     * \return Width of the image.
     */
    uint32_t GetWidth() const;

    /*!
     * \return Height of the image.
     */
    uint32_t GetHeight() const;

    /*!
     * \return Plant of the image.
     */
    uint32_t GetPlants() const;

    /*!
     * Get Pixel[Plant] value located at the (x, y)
     * \param[in] x X Coordinate
     * \param[in] y Y Coordinate
     * \param[in] Plant Image plant
     * \return Value of the Pixel[Plant]
     */
    const typename Pixel::Type GetPixel(uint32_t x, uint32_t y, uint8_t Plant) const;

    /*!
     * Set Pixel[Plant] value located at the (x, y)
     * \param[in] x X Coordinate
     * \param[in] y Y Coordinate
     * \param[in] Plant Image plant
     * \param[in] Value Value to be set
     */
    void SetPixel(uint32_t x, uint32_t y, uint8_t Plant, const typename Pixel::Type &Value);

    /*!
     * Get Pixel value located at the (x, y)
     * \param[in] x X Coordinate
     * \param[in] y Y Coordinate
     * \return Pixel value
     */
    const Pixel GetPixel(uint32_t x, uint32_t y) const;

    /*!
     * Set Pixel value located at the (x, y)
     * \param[in] x X Coordinate
     * \param[in] y Y Coordinate
     * \param[in] Value Value to be set
     */
    void SetPixel(uint32_t x, uint32_t y, const Pixel &Value);

    /*!
     * Allocate buffer for the WidthXHeight Image.
     * \param[in] Width Witdh of the Image
     * \param[in] Height Height of the Image
     */
    void Create(uint32_t Width, uint32_t Height);

    /*!
     * Allocate buffer for the WidthXHeight Image and fill it with Value.
     * \param[in] Width Witdh of the Image
     * \param[in] Height Height of the Image
     * \param[in] Value Value to be set
     */
    void Create(uint32_t Width, uint32_t Height, const Pixel &Value);

    /*!
     * Provides iterator-like interface for Image data.
     */
    class iterator
    {
    public:
        /*!
         * Empty default constructor.
         */
        iterator();

        /*!
         * Create iterator and set its' position.
         * \param[in] pRawData Pointer to internal buffer.
         * \param[in] SizeOfPixel Size of the underlying pixel.
         * \param[in] idx Offset from the start of the buffer.
         */
        iterator(uint8_t *pRawData, uint32_t SizeOfPixel, uint32_t idx);

        /*!
         * Move to next pixel.
         */
        iterator &operator++();

        /*!
         * Move to previous pixel.
         */
        iterator &operator--();

        /*!
         * Get pointer to the current position in the internal buffer.
         * \return Pointer to the current position in the internal buffer.
         */
        typename Pixel::Type *operator*();

        /*!
         * Get value of the current Pixel[Plant].
         * \return value of the current Pixel[Plant].
         */
        typename Pixel::Type &operator [] (uint8_t Plant);

        /*!
         * Check if iterators are equal.
         * \return true if equal, false otherwise.
         */
        bool operator==(const iterator &it) const;

        /*!
         * Check if iterators are not equal.
         * \return true if not equal, false otherwise.
         */
        bool operator!=(const iterator &it) const;
    private:
        uint32_t m_SizeOfPixel; //< Size of the underlying Pixel
        uint8_t * m_RawData; //< Pointer to the current position in the internal buffer.
    };

    /*!
     * Get iterator pointed to the begining of the Image.
     * \return iterator pointed to the begining of the Image.
     */
    const iterator begin() const;

    /*!
     * Get iterator pointed to the end of the Image.
     * \return iterator pointed to the end of the Image.
     */
    const iterator end() const;

    /*!
     * Get iterator pointed to the begining of the specified row.
     * \param[in] Row Row of the Image.
     * \return iterator pointed to the begining of the specified row.
     */
    const iterator GetRow(uint32_t Row) const;

    /*!
     * Get iterator pointed to the (column, row).
     * \param[in] Col Column of the Image.
     * \param[in] Row Row of the Image.
     * \return iterator pointed to the (column, row).
     */
    const iterator GetColRow(uint32_t Col, uint32_t Row) const;
protected:
    /*!
     * Resize Dst Image to the current Width and Height, then copy internal buffer to it.
     * \param[in] Dst Destination Image.
     * \attention It is protected so that derived classes can check types.
     */
    void CopyToInternal(GenericImage<Pixel> &Dst) const;

    /*!
    * Resize Current Image to the Src Width and Height, then copy it to own internal buffer.
    * \param[in] Src Source Image.
    * \attention It is protected so that derived classes can check types.
    */
    void CopyFromInternal(const GenericImage<Pixel> &Src);

    /*!
     * Allocate buffer for the WidthXHeight Image. Internal function.
     * \todo If new buffer size is less-or-equal to current buffer size, don't call DeleteRawData()
     * \param[in] Width Witdh of the Image
     * \param[in] Height Height of the Image
     */
    void AllocateRawData(uint32_t Width, uint32_t Height);

    /*!
     * Frees allocated buffer.
     */
    void DeleteRawData();
    const uint32_t m_SizeOfPixel; //< Size of Pixel in bytes
    const uint32_t m_SizeOfPlant; //< Size of Plant in bytes
    uint32_t m_Width; //< Current Width
    uint32_t m_Height; //< Current Height
    uint32_t m_BufSize; //< Current Buffer size
    uint32_t m_Offset; //< Offset to the next row
    uint8_t * m_RawData; //< Image buffer
    const uint8_t m_Plants; //< Amount of plants
};

// =======================================================

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
        for(uint16_t p = 0; p < m_Plants; ++p)
        {
            it[p] = Value[p];
        }
    }
}
template <typename Pixel>
void GenericImage<Pixel>::CopyToInternal(GenericImage<Pixel> &Dst) const
{
    Dst.Create(GetWidth(), GetHeight());
    memcpy(Dst.m_RawData, m_RawData, m_BufSize);
}
template <typename Pixel>
void GenericImage<Pixel>::CopyFromInternal(const GenericImage<Pixel> &Src)
{
    Create(Src.GetWidth(), Src.GetHeight());
    memcpy(m_RawData, Src.m_RawData, m_BufSize);
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
    for(uint16_t p = 0; p < m_Plants; ++p)
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
    for(uint16_t p = 0; p < m_Plants; ++p)
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

#endif //JIMLIB_GENERICIMAGE_H
