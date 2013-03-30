#ifndef BOARD_H
#define BOARD_H

#include "Common.h"

#include<QVector>

class Path;
class Square;

class Board
{
public:
    Board( uint rows, uint cols );
    ~Board();

    Square * GetSquare( uint row, uint col ) const;

    uint GetSize() const;

    uint GetMaxRow() const;

    uint GetMaxCol() const;

private:

    uint m_rows, m_cols;
    QVector<Square *> m_squares;
};

#endif // BOARD_H
