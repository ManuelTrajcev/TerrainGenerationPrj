#ifndef OGLDEV_2D_ARRAY_H
#define OGLDEV_2D_ARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include <cstring>  // For memcpy
#include <new>      // For std::bad_alloc

template<typename Type>
class Array2D {
public:
    Array2D() : m_p(nullptr), m_cols(0), m_rows(0) {}

    Array2D(int Cols, int Rows)
    {
        InitArray2D(Cols, Rows);
    }

    Array2D(const Array2D<Type>& other)
    {
        // Copy constructor
        m_cols = other.m_cols;
        m_rows = other.m_rows;

        m_p = (Type*)malloc(m_cols * m_rows * sizeof(Type));

        if (m_p == nullptr) {
            throw std::bad_alloc();
        }

        memcpy(m_p, other.m_p, m_cols * m_rows * sizeof(Type));
    }

    Array2D<Type>& operator=(const Array2D<Type>& other)
    {
        if (this == &other) {
            return *this;
        }

        Destroy();

        m_cols = other.m_cols;
        m_rows = other.m_rows;

        m_p = (Type*)malloc(m_cols * m_rows * sizeof(Type));

        if (m_p == nullptr) {
            throw std::bad_alloc();
        }

        memcpy(m_p, other.m_p, m_cols * m_rows * sizeof(Type));

        return *this;
    }

    ~Array2D()
    {
        Destroy();
    }

    void InitArray2D(int Cols, int Rows)
    {
        m_cols = Cols;
        m_rows = Rows;

        Destroy(); // Ensure previous memory is freed

        m_p = (Type*)malloc(Cols * Rows * sizeof(Type));

        if (m_p == nullptr) {
            throw std::bad_alloc();
        }
    }

    void InitArray2D(int Cols, int Rows, Type InitVal)
    {
        InitArray2D(Cols, Rows);

        for (int i = 0; i < Cols * Rows; i++) {
            m_p[i] = InitVal;
        }
    }

    void InitArray2D(int Cols, int Rows, void* pData)
    {
        m_cols = Cols;
        m_rows = Rows;

        Destroy(); // Ensure previous memory is freed

        m_p = (Type*)pData;
    }

    void Destroy()
    {
        if (m_p) {
            free(m_p);
            m_p = nullptr;
        }
    }

    Type* GetAddr(int Col, int Row) const
    {
        size_t Index = CalcIndex(Col, Row);
        return &m_p[Index];
    }

    Type* GetBaseAddr() const
    {
        return m_p;
    }

    int GetSize() const
    {
        return m_rows * m_cols;
    }

    int GetSizeInBytes() const
    {
        return GetSize() * sizeof(Type);
    }

    const Type& Get(int Col, int Row) const
    {
        return *GetAddr(Col, Row);
    }

    const Type& Get(int Index) const
    {
#ifndef NDEBUG
        if (Index >= m_rows * m_cols) {
            printf("%s:%d - index %d is out of bounds (max size %d)\n", __FILE__, __LINE__, Index, m_rows * m_cols);
            exit(0);
        }
#endif

        return m_p[Index];
    }

    Type& At(int Col, int Row)
    {
        size_t Index = CalcIndex(Col, Row);
        return m_p[Index];
    }

    void Set(int Col, int Row, const Type& Val)
    {
        *GetAddr(Col, Row) = Val;
    }

    void Set(int Index, const Type& Val)
    {
#ifndef NDEBUG
        if (Index >= m_rows * m_cols) {
            printf("%s:%d - index %d is out of bounds (max size %d)\n", __FILE__, __LINE__, Index, m_rows * m_cols);
            exit(0);
        }
#endif

        m_p[Index] = Val;
    }

    void GetMinMax(Type& Min, Type& Max)
    {
        Max = Min = m_p[0];

        for (int i = 1; i < m_rows * m_cols; i++) {
            if (m_p[i] < Min) {
                Min = m_p[i];
            }

            if (m_p[i] > Max) {
                Max = m_p[i];
            }
        }
    }

    void Normalize(Type MinRange, Type MaxRange, Type falloffFactor)
    {
        Type Min, Max;
        GetMinMax(Min, Max);


        if (Max <= Min) {
            return;
        }

        Type terrainSize = m_rows * m_cols;

        Type MinMaxDelta = Max - Min;
        Type MinMaxRange = MaxRange - MinRange;
        Type centerX = m_rows / 2.0;
        Type centerY = m_cols / 2.0;
        float maxDistance = std::sqrt(centerX * centerX + centerY * centerY);

        for (int y = 0; y < m_rows; ++y) {
            for (int x = 0; x < m_cols; ++x) {
                int index = y * m_rows + x;
                       
                float distanceX = x - centerX;
                float distanceY = y - centerY;
                float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

                float falloff = 1.0f - std::pow(distance / maxDistance, falloffFactor);

                m_p[index] = ((m_p[index] - Min) / MinMaxDelta) * (MaxRange - MinRange) * falloff + MinRange;
            }
        }

    }

    void PrintFloat()
    {
        for (int y = 0; y < m_rows; y++) {
            printf("%d: ", y);
            for (int x = 0; x < m_cols; x++) {
                float f = (float)m_p[y * m_cols + x];
                printf("%.6f ", f);
            }
            printf("\n");
        }
    }

private:
    size_t CalcIndex(int Col, int Row) const
    {
#ifndef NDEBUG
        if (Col < 0 || Col >= m_cols || Row < 0 || Row >= m_rows) {
            printf("%s:%d - index out of bounds\n", __FILE__, __LINE__);
            exit(0);
        }
#endif

        return Row * m_cols + Col;
    }

    Type* m_p;
    int m_cols;
    int m_rows;
};

#endif
