#ifndef SQUARE_H
#define SQUARE_H

#include "Common.h"

#include<QDebug>
#include<QVector>

class Square
{
public:
    Square(uint id, uint row, uint col);

    uint GetID() const;
    uint GetRow() const;
    uint GetColumn() const;

    void SetSquare( eDirection dir, Square * square );

    Square * GetSquare( eDirection dir ) const;

private:

    uint m_id, m_row, m_col;
    QVector<Square *> m_squares;
};

QDebug operator<<(QDebug out, const Square & square );

#endif // SQUARE_H
