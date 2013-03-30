#include "board.h"

#include "path.h"
#include "square.h"

Board::Board( uint rows, uint cols) :
    m_rows( rows ),
    m_cols( cols )
{
    Square * square( NULL );
    m_squares.fill( NULL, m_rows * m_cols );

    for( uint i=0 ; i < m_rows ; i++ )
    {
        for( uint j=0 ; j < m_cols ; j++ )
        {
            square = new Square( m_cols * i + j, i, j );
            m_squares.replace( m_cols * i + j, square );
        }
    }

    for( uint i=0 ; i < m_rows ; i++ )
    {
        for( uint j=0 ; j < m_cols ; j++ )
        {
            square = GetSquare( i, j );

            if( i > 0 )
            {
                square->SetSquare( DIRECTION_UP, GetSquare( i-1, j ) );
            }

            if( i < ( m_rows - 1 ) )
            {
                square->SetSquare( DIRECTION_DOWN, GetSquare( i+1, j ) );
            }

            if( j > 0 )
            {
                square->SetSquare( DIRECTION_LEFT, GetSquare( i, j-1 ) );
            }

            if( j < ( m_cols - 1 ) )
            {
                square->SetSquare( DIRECTION_RIGHT, GetSquare( i, j+1 ) );
            }
        }
    }

}

Board::~Board()
{
    foreach( Square * square, m_squares )
    {
        delete square;
    }
}

Square * Board::GetSquare( uint row, uint col ) const
{
    Square * square( NULL );
    if( ( row < m_rows ) && ( col < m_cols ) )
    {
        square = m_squares.at( m_cols * row + col );
    }
    return square;
}

uint Board::GetSize() const
{
    return ( m_rows * m_cols );
}

uint Board::GetMaxRow() const
{
    return (m_rows - 1);
}

uint Board::GetMaxCol() const
{
    return (m_cols - 1);
}
