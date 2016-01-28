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

#ifndef JIMLIB_GRAPH_HPP
#define JIMLIB_GRAPH_HPP
#include <cstdint>
#include <vector>
namespace jimlib
{
//    namespace GraphType
//    {
//        const uint8_t LinkedList = 0;
//    }
//    template <uint8_t Type>
    class Graph
    {
        // linked-list (vector) (vector of vectors)
    public:

        void AddChild(uint16_t Parent, uint16_t Child);
        void Allocate(uint16_t Amount);
        void BFS(uint16_t Parent, void * pData, void (*func)(uint16_t, void * pData)) const;
        void DFS(uint16_t Parent, void * pData, void (*func)(uint16_t, void * pData)) const;
    private:
        typedef uint16_t Node;
        typedef std::vector<std::vector<Node>> LinkedList;
        LinkedList m_LinkedList;
    };
    inline void Graph::AddChild(uint16_t Parent, uint16_t Child)
    {
        m_LinkedList[Parent].push_back(Child);
    }
    // TODO: replace recursion with iteration
    inline void Graph::BFS(uint16_t Parent, void *pData, void (*func)(uint16_t, void *)) const
    {
        if (func == nullptr)
        {
            return;
        }
        std::vector<Node>::const_iterator it = m_LinkedList[Parent].begin();
        for (; it != m_LinkedList[Parent].end(); ++it)
        {
            func(*it, pData);
        }
        it = m_LinkedList[Parent].begin();
        for (; it != m_LinkedList[Parent].end(); ++it)
        {
            BFS(*it, pData, func);
        }
    }
    inline void Graph::DFS(uint16_t Parent, void *pData, void (*func)(uint16_t, void *)) const
    {
        if (func == nullptr)
        {
            return;
        }
        std::vector<Node>::const_iterator it = m_LinkedList[Parent].begin();
        for (; it != m_LinkedList[Parent].end(); ++it)
        {
            func(*it, pData);
            DFS(*it, pData, func);
        }
    }
    inline void Graph::Allocate(uint16_t Amount)
    {
        m_LinkedList.reserve(Amount);
        for (uint32_t i = 0; i < Amount; ++i)
        {
            std::vector<Node> n;
            m_LinkedList.push_back(n);
        }
    }
};


#endif //JIMLIB_GRAPH_HPP
