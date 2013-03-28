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
    Path( Square * start, Square * stop, uint targetSize );
    Path(const Path &);
    Path &operator=(const Path &);
    ~Path();
    
    void SetBoard( Board * board );

    const QVector<Square *> GetSquares() const;

    bool IsValid() const;

    QList<Path> Step() const;

    uint Size() const;

private:
    class PathData;
    QSharedDataPointer<PathData> data;
};

QDebug operator<<(QDebug out, const Path & path );

#endif // PATH_H
