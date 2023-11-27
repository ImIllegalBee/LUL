#pragma once

namespace LUL
{
    template<typename T>
    struct LUL_DLL Vec2
    {
        T x = 0;
        T y = 0;
    };

    template<typename T>
    struct LUL_DLL Vec3
    {
        T x = 0;
        T y = 0;
        T z = 0;
    };

    template<typename T>
    struct LUL_DLL Vec4
    {
        T x = 0;
        T y = 0;
        T z = 0;
        T w = 0;
    };

    // Just for fun section ---------------------------------------------------

    template<typename T>
    struct LUL_DLL Vector
    {
    public:
        Vector()
        {
            Resize();
        }

        Vector(int32_t uGrowth) :
            m_ReAllocPhysicalMemGrowth(uGrowth)
        {
            Resize();
        }

        ~Vector()
        {
            Reset();
        }

    public:
        void operator=(Vector& v)
        {
            this->Reset();

            this->m_PhysicalAllocMem = v.m_PhysicalAllocMem;
            this->m_AllocedMem = v.m_AllocedMem;

            this->m_Buff = (T*) malloc(sizeof(T) * m_ReAllocPhysicalMemGrowth);
            if (m_Buff)
            {
                for (size_t i = 0; i < m_PhysicalAllocMem; i++)
                {
                    this->m_Buff[ i ] = v.m_Buff[ i ];
                }
            }
        }

        bool operator==(Vector& v)
        {
            return ( this->m_Buff == v.m_Buff ) ? true : false;
        }

        T& operator[](size_t i) const
        {
            if (i >= m_PhysicalAllocMem)
                throw;

            return m_Buff[ i ];
        }

        T& Push(T item)
        {
            if (m_AllocedMem >= m_PhysicalAllocMem)
                Resize();

            if (m_AllocedMem < 0)
                m_AllocedMem = 0;

            return m_Buff[ m_AllocedMem++ ] = item;
        }

        void Pop()
        {
            m_Buff[ m_AllocedMem-- ].~T();
        }

        void Resize()
        {
            T* tmpBuff = (T*) malloc(sizeof(T) * ( m_PhysicalAllocMem + m_ReAllocPhysicalMemGrowth ));

            if (tmpBuff)
            {
                for (size_t i = 0; i < m_PhysicalAllocMem; i++)
                {
                    tmpBuff[ i ] = m_Buff[ i ];
                }
            }
            else
            {
                throw;
            }

            delete( m_Buff );
            m_Buff = tmpBuff;
            m_PhysicalAllocMem += m_ReAllocPhysicalMemGrowth;
        }

        /// <summary>
        /// Set allocated memory index to zero.
        /// Vector will overwrite data.
        /// </summary>
        void Clear()
        {
            m_AllocedMem = 0;
        }

        /// <summary>
        /// Remake the buffer.
        /// </summary>
        void Reset()
        {
            delete( m_Buff );
            m_PhysicalAllocMem = 0;
            m_AllocedMem = 0;
        }

        size_t Size() const { return m_AllocedMem; }

        size_t IndxSize() const { return m_AllocedMem ? m_AllocedMem - 1 : 0; }

    private:
        T* m_Buff = nullptr;

        size_t m_AllocedMem = 0;
        size_t m_PhysicalAllocMem = 0;
        const int32_t m_ReAllocPhysicalMemGrowth = 12;

    };

    template<typename T>
    struct LUL_DLL LVec
    {
    public:
        LVec()
        {
            Init();
        }

        ~LVec()
        {
            Clear();
        }

    public:
        T& operator[](size_t uIndex)
        {
            if (uIndex >= m_uItems)
                throw;

            int32_t blockIndx, inBufferIndx;
            CalcBothIndexes(uIndex, blockIndx, inBufferIndx);

            return m_pHandlesToBuffers[ blockIndx ][ inBufferIndx ]; // Return refrence to the item from buffer
        }

        T& Push(T item)
        {
            if (m_uItems >= m_uMemSize)
                Resize();

            int32_t blockIndx, inBufferIndx;
            CalcBothIndexes(m_uItems, blockIndx, inBufferIndx);

            m_uItems++;

            m_pHandlesToBuffers[ blockIndx ][ inBufferIndx ] = item;
            return ( m_pHandlesToBuffers[ blockIndx ][ inBufferIndx ] ); // Return it's reference
        }

        void Resize()
        {
            if (m_GrowthsSize >= m_GrowthsMemAlloced)
            {
                size_t newSize = ( m_GrowthsMemAlloced + 4 ) * sizeof(size_t);
                void* newBlock = malloc(newSize);
                memcpy_s(newBlock,
                         newSize,
                         m_pGrowths,
                         m_GrowthsMemAlloced * sizeof(size_t));

                free(m_pGrowths);
                m_pGrowths = (T*) newBlock;
                m_GrowthsMemAlloced += 4;
            }

            // Create next growth
            size_t nextGrowth;
            if (m_pGrowths[ m_GrowthsSize - 1 ] < m_MaxGrowth)
            {
                m_pGrowths[ m_GrowthsSize ] = nextGrowth = ( m_pGrowths[ m_GrowthsSize - 1 ] * m_GrowthFactor );
            }
            else
            {
                // Clamp it
                m_pGrowths[ m_GrowthsSize ] = nextGrowth = ( m_pGrowths[ m_GrowthsSize - 1 ] );
            }
            m_GrowthsSize++;

            if (m_HandlesSize >= m_HandlesMemAlloced)
            {
                size_t newSize = ( m_HandlesMemAlloced + 4 ) * sizeof(T);
                void* newBlock = malloc(newSize);
                memcpy_s(newBlock,
                         newSize,
                         m_pHandlesToBuffers,
                         m_HandlesSize * sizeof(T));

                free(m_pHandlesToBuffers);
                m_pHandlesToBuffers = (T**) newBlock;
                m_HandlesMemAlloced += 4;
            }

            // Create new buffer with next growth
            m_pHandlesToBuffers[ m_HandlesSize++ ] = (T*) malloc(sizeof(T) * nextGrowth);

            m_uMemSize += nextGrowth;
        }

        /// <summary>
        /// Set size to 0, so it will overwrite data inside
        /// </summary>
        void Reset()
        {
            m_uItems = 0;
        }

        /// <summary>
        /// Destroy and reset the buffer
        /// </summary>
        void Clear()
        {
            // Destroy all blocks
            // Starting from items
            // Going to blocks for items
            // Ending at blocks with ptrs

            // Reinit
            //Init();
        }

    public:
        // Getters

        size_t GetSize() { return m_uItems; }

        size_t GetRealSize() { return m_uMemSize; }

        size_t GetBlocksAmount() { return m_HandlesSize; }

        size_t GetGrowthsAmount() { return m_GrowthsSize; }

    private:
        void Init()
        {
            // We need some base line for growths
            auto nextGrowth = m_pGrowths[ 0 ] = 32;
            m_GrowthsSize++;

            // Create new buffer with next growth
            m_pHandlesToBuffers[ m_HandlesSize++ ] = (T*) malloc(sizeof(T) * nextGrowth);

            m_uMemSize += nextGrowth;
        }

        int32_t CalcBlockIndx(const size_t& uGlobalIndx)
        {
            size_t uI = 0;
            for (int32_t k = 0; k < m_GrowthsSize; k++)
            {
                uI += m_pGrowths[ k ];

                if (uI > uGlobalIndx)
                    return k;
            }

            throw;
        }

        int32_t CalcInBufferIndx(const size_t& uGlobalIndx, const int32_t& uBlockIndx)
        {
            size_t result = uGlobalIndx;
            for (size_t k = 0; k < uBlockIndx; k++)
            {
                result -= m_pGrowths[ k ];
            }
            return static_cast<int32_t>( result );
        }

        void CalcBothIndexes(const size_t& uGlobalIndx, int32_t& uBlockIndex, int32_t& uInBufferIndex)
        {
            size_t uI = 0;
            for (int32_t k = 0; k < m_GrowthsSize; k++)
            {
                uI += m_pGrowths[ k ];

                if (uI > uGlobalIndx)
                {
                    uBlockIndex = k;
                    uInBufferIndex = static_cast<int32_t>( uGlobalIndx - ( uI - m_pGrowths[ k ] ) );
                    return;
                }
            }

            throw;
        }

    private:
        // Current sum of items allocated in blocks
        size_t m_uItems = 0;
        // Current sum of available memory allocated in blocks
        size_t m_uMemSize = 0;

        T** m_pHandlesToBuffers = (T**) malloc(( 4 ) * sizeof(T));
        size_t m_HandlesSize = 0;
        size_t m_HandlesMemAlloced = 4;

        size_t* m_pGrowths = (size_t*) malloc(4 * sizeof(size_t));
        size_t m_GrowthsSize = 0;
        size_t m_GrowthsMemAlloced = 4;
        const int8_t m_GrowthFactor = 2;
        const size_t m_MaxGrowth = MEM_KB(64);
    };
}