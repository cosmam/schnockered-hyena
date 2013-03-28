#ifndef BOARD_H
#define BOARD_H

#include "Common.h"

#include<QVector>

class Square;

class Board
{
public:
    Board( uint rows, uint cols );

    Square * GetSquare( uint row, uint col ) const;

    uint GetSize() const;

private:

    uint m_rows, m_cols;
    QVector<Square *> m_squares;
};

#endif // BOARD_H
