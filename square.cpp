#include "square.h"

Square::Square(uint id, uint row, uint col) :
    m_id( id ),
    m_row( row ),
    m_col( col )
{
    m_squares.fill( NULL, 4 );
}

uint Square::GetID() const
{
    return m_id;
}

uint Square::GetRow() const
{
    return m_row;
}

uint Square::GetColumn() const
{
    return m_col;
}

void Square::SetSquare( eDirection dir, Square * square )
{
    m_squares.replace( dir, square );
}

Square * Square::GetSquare( eDirection dir ) const
{
    return m_squares.at( dir );
}

QDebug operator<<(QDebug out, const Square & square )
{
    QString dirs;
    Square * other( NULL );

    other = square.GetSquare( DIRECTION_UP );
    dirs += ( other == NULL ? "NA" : QString::number(other->GetID()) );
    dirs += " ";
    other = square.GetSquare( DIRECTION_RIGHT );
    dirs += ( other == NULL ? "NA" : QString::number(other->GetID()) );
    dirs += " ";
    other = square.GetSquare( DIRECTION_DOWN );
    dirs += ( other == NULL ? "NA" : QString::number(other->GetID()) );
    dirs += " ";
    other = square.GetSquare( DIRECTION_LEFT );
    dirs += ( other == NULL ? "NA" : QString::number(other->GetID()) );

    out << quint32( square.GetID() )
        << quint32( square.GetRow() )
        << quint32( square.GetColumn() )
        << dirs;

    return out;
}
