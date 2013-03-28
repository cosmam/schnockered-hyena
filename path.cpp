#include "path.h"
#include <QSharedData>

#include "square.h"
#include "board.h"

class Path::PathData : public QSharedData {
public:
    QVector<Square *> m_squares;
    Board * m_board;
    Square * m_stop;
    uint m_targetSize;
};

Path::Path( Square * start, Square * stop, uint targetSize ) :
    data(new PathData)
{
    if( start )
    {
        data->m_squares.append( start );
    }

    data->m_stop = stop;

    data->m_targetSize = targetSize;
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

void Path::SetBoard( Board * board )
{
    data->m_board = board;
}

const QVector<Square *> Path::GetSquares() const
{
    return data->m_squares;
}

bool Path::IsValid() const
{
    bool valid(true);

    if( data->m_squares.contains( data->m_stop ) &&
        ( static_cast<uint>(data->m_squares.size()) != data->m_targetSize ) )
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

    for( uint i=0 ; i < 4 ; i++ )
    {
        dir = static_cast<eDirection>(i);
        other = square->GetSquare( dir );
        if( other && !data->m_squares.contains( other ) )
        {
            Path path( *this );
            path.data->m_squares.append( other );
            paths.append( path );
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
    }

    return out;
}
