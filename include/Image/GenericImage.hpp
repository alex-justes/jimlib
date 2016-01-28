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

/*!
 *  \file
 *  \brief GenericImage<PixelType> class specification.
 *  \author Alexey Titov
 *  \date September 2015
 *  \copyright zlib
 *
 *  Full specification of the GenericImage<PixelType> class and underlying iterator.
 */

#ifndef JIMLIB_GENERICIMAGE_HPP
#define JIMLIB_GENERICIMAGE_HPP

#include <cstring>
#include "Utils/CheckTypes.hpp"
#include "Image/GenericPixel.hpp"


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

namespace jimlib
{
    template<typename Pixel>
    class GenericImage
    {
    public:
        static const uint8_t Plants = Pixel::Plants; //< Amount of plants
        static const uint32_t SizeOfPlant = sizeof(typename Pixel::Type); //< Size of Plant in bytes
        static const uint32_t SizeOfPixel = Pixel::Plants * sizeof(typename Pixel::Type); //< Size of Pixel in bytes

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
         * Get Pixel[Plant] value located at the (x, y)
         * \param[in] x X Coordinate
         * \param[in] y Y Coordinate
         * \param[in] Plant Image plant
         * \return Value of the Pixel[Plant]
         */
        const typename Pixel::Type &GetPixel(uint32_t x, uint32_t y, uint8_t Plant) const;

        typename Pixel::Type &GetPixel(uint32_t x, uint32_t y, uint8_t Plant);

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
         * Get pointer to Pixel located at the (x, y)
         * \param[in] x X Coordinate
         * \param[in] y Y Coordinate
         * \return pointer to Pixel
         */
        typename Pixel::Type * GetPixel(uint32_t x, uint32_t y);

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
         * Resize Dst Image to the current Width and Height, then copy internal buffer to it.
         * \param[in] Dst Destination Image.
         * \warning It is unsafe! Use it only it specific bottlenecks, wheng you are sure,
         * that types are same! (i.e. BinaryImage should contain only 1 or 0 pixels, and GrayImage - 0..255,
         * but underlying type is Mono8, so it possible to copy one to another with this function!).
         */
        void CopyTo_Unsafe(GenericImage<Pixel> &Dst) const;

        /*!
         * Resize Current Image to the Src Width and Height, then copy it to own internal buffer.
         * \param[in] Src Source Image.
         * \warning It is unsafe! Use it only it specific bottlenecks, wheng you are sure,
         * that types are same! (i.e. BinaryImage should contain only 1 or 0 pixels, and GrayImage - 0..255,
         * but underlying type is Mono8, so it possible to copy one to another with this function!).
         */
        void CopyFrom_Unsafe(const GenericImage<Pixel> &Src);

        class const_oterator;

        /*!
         * Provides iterator-like interface for Image data.
         */
        class iterator
        {
            friend class const_iterator;
        public:
            /*!
             * Empty default constructor.
             */
            iterator();

            /*!
             * Create iterator and set its' position.
             * \param[in] pRawData Pointer to internal buffer.
             * \param[in] idx Offset from the start of the buffer.
             */
            iterator(uint8_t *pRawData, uint32_t idx);

            /*!
             * Create iterator from another iterator.
             * \paran[in] it another iterator
             */
            iterator(const iterator &it);

            /*!
             * Move to next pixel.
             */
            iterator &operator++();

            /*!
             * Move to +offset pixels from current position.
             * \param[in] offset offset from from current position.
             */
            void operator+=(uint32_t offset);

            /*!
             * Move to -offset pixels from current position.
             * \param[in] offset offset from from current position.
             */
            void operator-=(uint32_t offset);

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
            typename Pixel::Type &operator[](uint8_t Plant);

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
            /*!
             * Check if iterators are equal.
             * \return true if equal, false otherwise.
             */
            bool operator==(const typename GenericImage<Pixel>::const_iterator &it) const;

            /*!
             * Check if iterators are not equal.
             * \return true if not equal, false otherwise.
             */
            bool operator!=(const typename GenericImage<Pixel>::const_iterator &it) const;

            static const uint32_t SizeOfPixel = Pixel::SizeOfPixel; //< Size of the underlying Pixel
        private:
            uint8_t *m_RawData; //< Pointer to the current position in the internal buffer.
        };

        /*!
         * Provides iterator-like interface for Image data.
         */
        class const_iterator
        {
            friend class iterator;
        public:
            /*!
             * Empty default constructor.
             */
            const_iterator();

            /*!
             * Create iterator and set its' position.
             * \param[in] pRawData Pointer to internal buffer.
             * \param[in] idx Offset from the start of the buffer.
             */
            const_iterator(uint8_t *pRawData, uint32_t idx);

            /*!
             * Create iterator from another iterator.
             * \paran[in] it another iterator
             */
            const_iterator(const iterator &it);

            /*!
             * Create iterator from another iterator.
             * \paran[in] it another iterator
             */
            const_iterator(const const_iterator &it);

            /*!
             * Move to next pixel.
             */
            const_iterator &operator++();

            /*!
             * Move to +offset pixels from current position.
             * \param[in] offset offset from from current position.
             */
            void operator+=(uint32_t offset);

            /*!
             * Move to -offset pixels from current position.
             * \param[in] offset offset from from current position.
             */
            void operator-=(uint32_t offset);

            /*!
             * Move to previous pixel.
             */
            const_iterator &operator--();

            /*!
             * Get pointer to the current position in the internal buffer.
             * \return Pointer to the current position in the internal buffer.
             */
            const typename Pixel::Type *operator*() const;

            /*!
             * Get value of the current Pixel[Plant].
             * \return value of the current Pixel[Plant].
             */
            const typename Pixel::Type &operator[](uint8_t Plant) const;

            /*!
             * Check if iterators are equal.
             * \return true if equal, false otherwise.
             */
            bool operator==(const const_iterator &it) const;

            /*!
             * Check if iterators are not equal.
             * \return true if not equal, false otherwise.
             */
            bool operator!=(const const_iterator &it) const;

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

            static const uint32_t SizeOfPixel = Pixel::SizeOfPixel; //< Size of the underlying Pixel
        private:
            const uint8_t *m_RawData; //< Pointer to the current position in the internal buffer.
        };

        /*!
         * Get iterator pointed to the begining of the Image.
         * \return iterator pointed to the begining of the Image.
         */
        iterator begin();

        /*!
         * Get iterator pointed to the end of the Image.
         * \return iterator pointed to the end of the Image.
         */
        iterator end();

        /*!
         * Get iterator pointed to the begining of the specified row.
         * \param[in] Row Row of the Image.
         * \return iterator pointed to the begining of the specified row.
         */
        iterator GetRow(uint32_t Row);

        /*!
         * Get iterator pointed to the (column, row).
         * \param[in] Col Column of the Image.
         * \param[in] Row Row of the Image.
         * \return iterator pointed to the (column, row).
         */
        iterator GetColRow(uint32_t Col, uint32_t Row);

        /*!
         * Get const_iterator pointed to the begining of the Image.
         * \return iterator pointed to the begining of the Image.
         */
        const_iterator begin() const;

        /*!
         * Get const_iterator pointed to the end of the Image.
         * \return iterator pointed to the end of the Image.
         */
        const_iterator end() const;

        /*!
         * Get const_iterator pointed to the begining of the specified row.
         * \param[in] Row Row of the Image.
         * \return iterator pointed to the begining of the specified row.
         */
        const_iterator GetRow(uint32_t Row) const;

        /*!
         * Get const_iterator pointed to the (column, row).
         * \param[in] Col Column of the Image.
         * \param[in] Row Row of the Image.
         * \return iterator pointed to the (column, row).
         */
        const_iterator GetColRow(uint32_t Col, uint32_t Row) const;


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

        uint32_t m_Width; //< Current Width
        uint32_t m_Height; //< Current Height
        uint32_t m_BufSize; //< Current Buffer size
        uint32_t m_Offset; //< Offset to the next row
        uint8_t *m_RawData; //< Image buffer
    };

// =======================================================


    template<typename Pixel>
    GenericImage<Pixel>::GenericImage()
            : m_Width(0),
              m_Height(0),
              m_BufSize(0),
              m_Offset(0),
              m_RawData(nullptr)
    {
        static_assert(CheckTypes<GenericPixel<typename Pixel::Type, Pixel::Plants>,
                              typename Pixel::ParentType>::areSame,
                      "Type is not GenericPixel<T, uint8_t Plants>!");
    }

    template<typename Pixel>
    GenericImage<Pixel>::~GenericImage()
    {
        if (m_RawData)
        {
            delete[] m_RawData;
        }
    }

    template<typename Pixel>
    void GenericImage<Pixel>::Create(uint32_t Width, uint32_t Height)
    {
        AllocateRawData(Width, Height);
    }

    template<typename Pixel>
    void GenericImage<Pixel>::Create(uint32_t Width, uint32_t Height, const Pixel &Value)
    {
        Create(Width, Height);
        typename GenericImage<Pixel>::iterator it = begin();
        for (; it != end(); ++it)
        {
            memcpy(*it, Value.m_Buffer, Value.SizeOfPixel);
        }
    }

    template<typename Pixel>
    void GenericImage<Pixel>::CopyToInternal(GenericImage<Pixel> &Dst) const
    {
        Dst.Create(GetWidth(), GetHeight());
        memcpy(Dst.m_RawData, m_RawData, m_BufSize);
    }

    template<typename Pixel>
    void GenericImage<Pixel>::CopyFromInternal(const GenericImage<Pixel> &Src)
    {
        Create(Src.GetWidth(), Src.GetHeight());
        memcpy(m_RawData, Src.m_RawData, m_BufSize);
    }

    template<typename Pixel>
    void GenericImage<Pixel>::CopyTo_Unsafe(GenericImage<Pixel> &Dst) const
    {
        Dst.Create(GetWidth(), GetHeight());
        memcpy(Dst.m_RawData, m_RawData, m_BufSize);
    }

    template<typename Pixel>
    void GenericImage<Pixel>::CopyFrom_Unsafe(const GenericImage<Pixel> &Src)
    {
        Create(Src.GetWidth(), Src.GetHeight());
        memcpy(m_RawData, Src.m_RawData, m_BufSize);
    }

    template<typename Pixel>
    void GenericImage<Pixel>::AllocateRawData(uint32_t Width, uint32_t Height)
    {
        uint32_t DataSize = Width * Height * Plants * SizeOfPlant;
        m_Width = Width;
        m_Height = Height;
        m_Offset = m_Width * SizeOfPixel;
        if (DataSize != m_BufSize)
        {
            DeleteRawData();
            m_BufSize = DataSize;
            m_RawData = new uint8_t[m_BufSize];
        }
    }

    template<typename Pixel>
    void GenericImage<Pixel>::DeleteRawData()
    {
        if (m_RawData)
        {
            delete[] m_RawData;
            m_RawData = nullptr;
        }
    }

    template<typename Pixel>
    unsigned int GenericImage<Pixel>::GetWidth() const
    {
        return m_Width;
    }

    template<typename Pixel>
    unsigned int GenericImage<Pixel>::GetHeight() const
    {
        return m_Height;
    }

    template<typename Pixel>
    const typename Pixel::Type &GenericImage<Pixel>::GetPixel(uint32_t x, uint32_t y, uint8_t Plant) const
    {
        assert(Plant < Plants);
        return *(reinterpret_cast<typename Pixel::Type *>(m_RawData + y * m_Offset + x * SizeOfPixel) + Plant);
    }

    template<typename Pixel>
    typename Pixel::Type &GenericImage<Pixel>::GetPixel(uint32_t x, uint32_t y, uint8_t Plant)
    {
        assert(Plant < Plants);
        return *(reinterpret_cast<typename Pixel::Type *>(m_RawData + y * m_Offset + x * SizeOfPixel) + Plant);
    }

    template<typename Pixel>
    const Pixel GenericImage<Pixel>::GetPixel(uint32_t x, uint32_t y) const
    {
        Pixel pix;
        typename GenericImage<Pixel>::const_iterator it = GetColRow(x, y);
        memcpy(pix.m_Buffer, *it, pix.SizeOfPixel);
        return pix;
    }
    template<typename Pixel>
    typename Pixel::Type * GenericImage<Pixel>::GetPixel(uint32_t x, uint32_t y)
    {
        return (*GetColRow(x,y));
    }

    template<typename Pixel>
    void GenericImage<Pixel>::SetPixel(uint32_t x, uint32_t y, uint8_t Plant, const typename Pixel::Type &Value)
    {
        assert(Plant < Plants);
        *(reinterpret_cast<typename Pixel::Type *>(m_RawData + y * m_Offset + x * SizeOfPixel) + Plant) = Value;
    }

    template<typename Pixel>
    void GenericImage<Pixel>::SetPixel(uint32_t x, uint32_t y, const Pixel &Value)
    {
        typename GenericImage<Pixel>::iterator it = GetColRow(x, y);
        memcpy(*it, Value.m_Buffer, Value.SizeOfPixel);
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::iterator GenericImage<Pixel>::begin()
    {
        GenericImage<Pixel>::iterator it(m_RawData, 0);
        return it;
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::iterator GenericImage<Pixel>::end()
    {
        GenericImage<Pixel>::iterator it(m_RawData, m_BufSize);
        return it;
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::iterator GenericImage<Pixel>::GetRow(uint32_t Row)
    {
        GenericImage<Pixel>::iterator it(m_RawData, m_Offset * Row);
        return it;
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::iterator GenericImage<Pixel>::GetColRow(uint32_t Col, uint32_t Row)
    {
        GenericImage<Pixel>::iterator it(m_RawData, m_Offset * Row + SizeOfPixel * Col);
        return it;
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::const_iterator GenericImage<Pixel>::begin() const
    {
        GenericImage<Pixel>::const_iterator it(m_RawData, 0);
        return it;
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::const_iterator GenericImage<Pixel>::end() const
    {
        GenericImage<Pixel>::const_iterator it(m_RawData, m_BufSize);
        return it;
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::const_iterator GenericImage<Pixel>::GetRow(uint32_t Row) const
    {
        GenericImage<Pixel>::const_iterator it(m_RawData, m_Offset * Row);
        return it;
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::const_iterator GenericImage<Pixel>::GetColRow(uint32_t Col, uint32_t Row) const
    {
        GenericImage<Pixel>::const_iterator it(m_RawData, m_Offset * Row + SizeOfPixel * Col);
        return it;
    }

    template<typename Pixel>
    GenericImage<Pixel>::iterator::iterator()
            : m_RawData(nullptr)
    {

    }

    template<typename Pixel>
    GenericImage<Pixel>::iterator::iterator(uint8_t *pRawData, uint32_t idx)
            : m_RawData(pRawData)
    {
        assert(m_RawData != nullptr);
        m_RawData += idx;
    }

    template<typename Pixel>
    GenericImage<Pixel>::iterator::iterator(const iterator &it)
            : m_RawData(it.m_RawData)
    {
        assert(m_RawData != nullptr);
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::iterator &GenericImage<Pixel>::iterator::operator++()
    {
        assert(m_RawData != nullptr);
        m_RawData += SizeOfPixel;
        return (*this);
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::iterator &GenericImage<Pixel>::iterator::operator--()
    {
        assert(m_RawData != nullptr);
        m_RawData -= SizeOfPixel;
        return (*this);
    }

    template<typename Pixel>
    void GenericImage<Pixel>::iterator::operator+=(uint32_t offset)
    {
        assert(m_RawData != nullptr);
        m_RawData += offset * SizeOfPixel;
    }

    template<typename Pixel>
    void GenericImage<Pixel>::iterator::operator-=(uint32_t offset)
    {
        assert(m_RawData != nullptr);
        m_RawData -= offset * SizeOfPixel;
    }

    template<typename Pixel>
    bool GenericImage<Pixel>::iterator::operator==(const typename GenericImage<Pixel>::iterator &it) const
    {
        return (this->m_RawData == it.m_RawData);
    }

    template<typename Pixel>
    bool GenericImage<Pixel>::iterator::operator!=(const typename GenericImage<Pixel>::iterator &it) const
    {
        return (this->m_RawData != it.m_RawData);
    }
    template<typename Pixel>
    bool GenericImage<Pixel>::iterator::operator==(const typename GenericImage<Pixel>::const_iterator &it) const
    {
        return (this->m_RawData == it.m_RawData);
    }

    template<typename Pixel>
    bool GenericImage<Pixel>::iterator::operator!=(const typename GenericImage<Pixel>::const_iterator &it) const
    {
        return (this->m_RawData != it.m_RawData);
    }

    template<typename Pixel>
    typename Pixel::Type *GenericImage<Pixel>::iterator::operator*()
    {
        return reinterpret_cast<typename Pixel::Type *>(m_RawData);
    }

    template<typename Pixel>
    typename Pixel::Type &GenericImage<Pixel>::iterator::operator[](uint8_t Plant)
    {
        assert(Plant < Pixel::Plants);
        assert(m_RawData != nullptr);
        return reinterpret_cast<typename Pixel::Type *>(m_RawData)[Plant];
    }


    template<typename Pixel>
    GenericImage<Pixel>::const_iterator::const_iterator()
            : m_RawData(nullptr)
    {

    }

    template<typename Pixel>
    GenericImage<Pixel>::const_iterator::const_iterator(uint8_t *pRawData, uint32_t idx)
            : m_RawData(pRawData)
    {
        assert(m_RawData != nullptr);
        m_RawData += idx;
    }

    template<typename Pixel>
    GenericImage<Pixel>::const_iterator::const_iterator(const const_iterator &it)
            : m_RawData(it.m_RawData)
    {
        assert(m_RawData != nullptr);
    }

    template<typename Pixel>
    GenericImage<Pixel>::const_iterator::const_iterator(const iterator &it)
            : m_RawData(it.m_RawData)
    {
        assert(m_RawData != nullptr);
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::const_iterator &GenericImage<Pixel>::const_iterator::operator++()
    {
        assert(m_RawData != nullptr);
        m_RawData += SizeOfPixel;
        return (*this);
    }

    template<typename Pixel>
    typename GenericImage<Pixel>::const_iterator &GenericImage<Pixel>::const_iterator::operator--()
    {
        assert(m_RawData != nullptr);
        m_RawData -= SizeOfPixel;
        return (*this);
    }

    template<typename Pixel>
    void GenericImage<Pixel>::const_iterator::operator+=(uint32_t offset)
    {
        assert(m_RawData != nullptr);
        m_RawData += offset * SizeOfPixel;
    }

    template<typename Pixel>
    void GenericImage<Pixel>::const_iterator::operator-=(uint32_t offset)
    {
        assert(m_RawData != nullptr);
        m_RawData -= offset * SizeOfPixel;
    }

    template<typename Pixel>
    bool GenericImage<Pixel>::const_iterator::operator==(const typename GenericImage<Pixel>::const_iterator &it) const
    {
        return (this->m_RawData == it.m_RawData);
    }

    template<typename Pixel>
    bool GenericImage<Pixel>::const_iterator::operator!=(const typename GenericImage<Pixel>::const_iterator &it) const
    {
        return (this->m_RawData != it.m_RawData);
    }

    template<typename Pixel>
    bool GenericImage<Pixel>::const_iterator::operator==(const typename GenericImage<Pixel>::iterator &it) const
    {
        return (this->m_RawData == it.m_RawData);
    }

    template<typename Pixel>
    bool GenericImage<Pixel>::const_iterator::operator!=(const typename GenericImage<Pixel>::iterator &it) const
    {
        return (this->m_RawData != it.m_RawData);
    }

    template<typename Pixel>
    const typename Pixel::Type *GenericImage<Pixel>::const_iterator::operator*() const
    {
        return reinterpret_cast<const typename Pixel::Type *>(m_RawData);
    }

    template<typename Pixel>
    const typename Pixel::Type &GenericImage<Pixel>::const_iterator::operator[](uint8_t Plant) const
    {
        assert(Plant < Pixel::Plants);
        assert(m_RawData != nullptr);
        return (reinterpret_cast<const typename Pixel::Type *>(m_RawData)[Plant]);
    }
};
#endif //JIMLIB_GENERICIMAGE_HPP
