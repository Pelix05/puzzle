#include "puzzlewidget.h"
#include "pieceslist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>

PuzzleWidget::PuzzleWidget(int imageSize, int gridSize, QWidget *parent)
    : QWidget(parent),m_ImageSize(imageSize), m_GridSize(gridSize)
{
    setAcceptDrops(true);
    setMinimumSize(m_ImageSize, m_ImageSize);
    setMaximumSize(m_ImageSize, m_ImageSize);
}

void PuzzleWidget::clear()
{
    pieces.clear();
    highlightedRect = QRect();
    inPlace = 0;
    update();
}

void PuzzleWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType()))
        event->accept();
    else
        event->ignore();
}

void PuzzleWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    QRect updateRect = highlightedRect;
    highlightedRect = QRect();
    update(updateRect);
    event->accept();
}

void PuzzleWidget::setGridSize(int grid)
{
    m_GridSize = grid;
    clear(); // reset papan puzzle
}


void PuzzleWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QRect updateRect = highlightedRect.united(targetSquare(event->pos()));

    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType())
        && findPiece(targetSquare(event->pos())) == -1) {

        highlightedRect = targetSquare(event->pos());
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        highlightedRect = QRect();
        event->ignore();
    }

    update(updateRect);
}

void PuzzleWidget::removePiece(const QRect &rect)
{
    int idx = findPiece(rect);
    if (idx != -1) {
        pieces.removeAt(idx);
        update(rect);
    }
}


void PuzzleWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType())
        && findPiece(targetSquare(event->pos())) == -1) {

        QByteArray pieceData = event->mimeData()->data(PiecesList::puzzleMimeType());
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        Piece piece;
        piece.rect = targetSquare(event->pos());
        dataStream >> piece.pixmap >> piece.location;

        pieces.append(piece);
        emit piecePlaced(piece.pixmap, piece.location, piece.rect);

        highlightedRect = QRect();
        update(piece.rect);

        event->setDropAction(Qt::MoveAction);
        event->accept();

        if (piece.location == piece.rect.topLeft() / pieceSize()) {
            inPlace++;
            if (inPlace == m_GridSize * m_GridSize)   // ✅ fleksibel
                emit puzzleCompleted();
        }
    } else {
        highlightedRect = QRect();
        event->ignore();
    }
}

int PuzzleWidget::findPiece(const QRect &pieceRect) const
{
    for (int i = 0, size = pieces.size(); i < size; ++i) {
        if (pieces.at(i).rect == pieceRect)
            return i;
    }
    return -1;
}

void PuzzleWidget::mousePressEvent(QMouseEvent *event)
{
    QRect square = targetSquare(event->pos());
    const int found = findPiece(square);

    if (found == -1)
        return;

    Piece piece = pieces.takeAt(found);

    if (piece.location == square.topLeft() / pieceSize())
        inPlace--;

    update(square);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << piece.pixmap << piece.location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(PiecesList::puzzleMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - square.topLeft());
    drag->setPixmap(piece.pixmap);

    if (drag->exec(Qt::MoveAction) != Qt::MoveAction) {
        pieces.insert(found, piece);
        update(targetSquare(event->pos()));

        if (piece.location == square.topLeft() / pieceSize())
            inPlace++;
    }
}

void PuzzleWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::white);

    if (highlightedRect.isValid()) {
        painter.setBrush(QColor("#ffcccc"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
    }

    for (const Piece &piece : pieces)
        painter.drawPixmap(piece.rect, piece.pixmap);
}

const QRect PuzzleWidget::targetSquare(const QPoint &position) const
{
    return QRect(position / pieceSize() * pieceSize(),
                 QSize(pieceSize(), pieceSize()));
}

int PuzzleWidget::pieceSize() const
{
    return m_ImageSize / m_GridSize;
}

int PuzzleWidget::imageSize() const
{
    return m_ImageSize;
}

