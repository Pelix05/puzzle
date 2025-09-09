#ifndef PUZZLEWIDGET_H
#define PUZZLEWIDGET_H

#include <QPoint>
#include <QPixmap>
#include <QVector>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
QT_END_NAMESPACE

class PuzzleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PuzzleWidget(int imageSize, int gridSize, QWidget *parent = nullptr);
    void clear();

    int pieceSize() const;
    int imageSize() const;
    void setGridSize(int grid);
    int gridSize() const { return m_GridSize; }
    void removePiece(const QRect &rect);
    void addPieceWithRotation(const QPixmap &pixmap, const QPoint &location, int rotation);
    void rotatePieceAt(const QRect &square);


signals:
    void puzzleCompleted();
    void piecePlaced(QPixmap pixmap, QPoint location, QRect rect);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    struct Piece {
        QPixmap pixmap;
        QPixmap original; // real photo (0°)
        QRect rect;
        QPoint location;
        int rotation;     // 0, 90, 180, 270
    };

    int findPiece(const QRect &pieceRect) const;
    const QRect targetSquare(const QPoint &position) const;
    void checkCompletion();


    QVector<Piece> pieces;
    QRect highlightedRect;
    QPoint dragStartPos;
    int m_ImageSize;
    int m_GridSize;

    void rotatePieceAt(int index);

};

#endif // PUZZLEWIDGET_H
