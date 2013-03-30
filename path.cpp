#include "path.h"
#include <QSharedData>

#include "square.h"
#include "board.h"

Board * Path::m_board = NULL;
Square * Path::m_stop = NULL;
uint Path::m_targetSize = 0;
uint Path::m_maxRow = 0;
uint Path::m_maxCol = 0;
bool Path::m_endConditions = true;

class Path::PathData : public QSharedData {
public:
    QList<Square *> m_squares;
};

Path::Path( Square * start  ) :
    data(new PathData)
{
    if( start )
    {
        data->m_squares.append( start );
    }
}

Path::Path(const Path &rhs) : data(rhs.data)
{
}

Path &Path::operator=(const Path &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

Path::~Path()
{
}

const QList<Square *> Path::GetSquares() const
{
    return data->m_squares;
}

bool Path::IsValid() const
{
    bool valid(true);

    if( data->m_squares.contains( m_stop ) &&
        ( static_cast<uint>(data->m_squares.size()) != m_targetSize ) )
    {
        valid = false;
    }

    return valid;
}

QList<Path> Path::Step() const
{
    QList<Path> paths;
    Square * square = data->m_squares.last();
    Square * other( NULL );
    eDirection dir;
    bool bValid(true);

    for( uint i=0 ; i < 4 ; i++ )
    {
        dir = static_cast<eDirection>(i);
        other = square->GetSquare( dir );
        if( other && !data->m_squares.contains( other ) )
        {
            bValid =true;
\
            if( m_endConditions )
            {
                if( ( dir == DIRECTION_RIGHT ) && ( other != m_stop ) &&
                    ( other->GetColumn() == m_maxCol ) &&
                    ( other->GetRow() > 0 ) )
                {
                    // if path doesn't contain square above it, not valid
                    bValid = data->m_squares.contains(
                                      m_board->GetSquare( other->GetRow() - 1,
                                                          other->GetColumn() ) );
                }
                else if( ( dir == DIRECTION_DOWN ) && ( other != m_stop ) &&
                         ( other->GetRow() == m_maxRow ) &&
                         ( other->GetColumn() != m_maxCol ) )
                {
                    // if path doesn't contain square above it, not valid
                    bValid = data->m_squares.contains(
                                      m_board->GetSquare( other->GetRow(),
                                                          other->GetColumn() + 1 ) );
                }
            }

            if( bValid )
            {
                Path path( *this );
                path.data->m_squares.append( other );
                paths.append( path );
            }
        }
    }

    return paths;
}

uint Path::Size() const
{
    return static_cast<uint>(data->m_squares.size());
}

QDebug operator<<(QDebug out, const Path & path )
{
    foreach( Square * square, path.GetSquares() )
    {
        out << square->GetID();
        out << " ";
    }

    return out;
}

void Path::SetBoard( Board * board )
{
    m_board = board;
}

void Path::SetStop( Square * square )
{
    m_stop = square;
}

void Path::SetTargetSize( uint size )
{
    m_targetSize = size;
}

void Path::SetMaxRow( uint row )
{
    m_maxRow = row;
}

void Path::SetMaxCol( uint col )
{
    m_maxCol = col;
}

void Path::SetEndContitions( bool end )
{
    m_endConditions = end;
}
