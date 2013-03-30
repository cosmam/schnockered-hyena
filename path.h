#ifndef PATH_H
#define PATH_H

#include <QSharedDataPointer>
#include <QVector>
#include <QDebug>

class Board;
class Square;

class Path
{
public:
    Path( Square * start );
    Path(const Path &);
    Path &operator=(const Path &);
    ~Path();

    static void SetBoard( Board * board );

    static void SetStop( Square * square );

    static void SetTargetSize( uint size );

    static void SetMaxRow( uint row );

    static void SetMaxCol( uint col );

    static void SetEndContitions( bool end );

    const QList<Square *> GetSquares() const;

    bool IsValid() const;

    QList<Path> Step() const;

    uint Size() const;

private:
    class PathData;
    QSharedDataPointer<PathData> data;

    static Board * m_board;
    static Square * m_stop;
    static uint m_targetSize;
    static uint m_maxRow;
    static uint m_maxCol;
    static bool m_endConditions;
};

QDebug operator<<(QDebug out, const Path & path );

#endif // PATH_H
