#include "puzzlewidget.h"
#include "pieceslist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>
#include <QMouseEvent>

PuzzleWidget::PuzzleWidget(int imageSize, int gridSize, QWidget *parent)
    : QWidget(parent), m_ImageSize(imageSize), m_GridSize(gridSize)
{
    setAcceptDrops(true);
    setMinimumSize(m_ImageSize, m_ImageSize);
    setMaximumSize(m_ImageSize, m_ImageSize);
}



void PuzzleWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType()))
        event->accept();
    else
        event->ignore();
}

void PuzzleWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QRect updateRect = highlightedRect.united(targetSquare(event->pos()));
    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType())
        && findPiece(targetSquare(event->pos())) == -1)
    {
        highlightedRect = targetSquare(event->pos());
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        highlightedRect = QRect();
        event->ignore();
    }
    update(updateRect);
}

void PuzzleWidget::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->hasFormat(PiecesList::puzzleMimeType()))
    {
        event->ignore();
        return;
    }

    QByteArray pieceData = event->mimeData()->data(PiecesList::puzzleMimeType());
    QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
    QPixmap pixmap;
    QPoint location; // ini adalah lokasi grid yang benar dari piece
    dataStream >> pixmap >> location;

    QRect correctRect(location * pieceSize(), QSize(pieceSize(), pieceSize()));

    // kalau di posisi correctRect sudah ada piece → tolak
    if (findPiece(correctRect) != -1)
    {
        highlightedRect = QRect();
        event->ignore();
        return;
    }

    Piece piece;
    piece.pixmap = pixmap;
    piece.original = pixmap;
    piece.rotation = 0;
    piece.location = location;
    piece.rect = correctRect;

    pieces.append(piece);
    emit piecePlaced(piece.pixmap, piece.location, piece.rect);

    highlightedRect = QRect();
    update(piece.rect);

    event->setDropAction(Qt::MoveAction);
    event->accept();

    checkCompletion();
}


int PuzzleWidget::findPiece(const QRect &pieceRect) const
{
    for (int i = 0; i < pieces.size(); ++i)
        if (pieces[i].rect == pieceRect)
            return i;
    return -1;
}

void PuzzleWidget::checkCompletion()
{
    if (pieces.size() != m_GridSize * m_GridSize)
        return; // belum semua kepasang

    for (const Piece &p : pieces) {
        QRect correctRect(p.location * pieceSize(),
                          QSize(pieceSize(), pieceSize()));

        // Posisi harus sesuai
        if (p.rect != correctRect)
            return;

        // Rotasi harus kembali 0°
        if (p.rotation != 0)
            return;
    }

    // ✅ Semua sudah benar
    emit puzzleCompleted();
}



void PuzzleWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    QRect square = targetSquare(event->pos());
    int idx = findPiece(square);
    if (idx == -1)
        return;

    // Klik piece untuk rotate
    rotatePieceAt(square);
}
void PuzzleWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    QRect updateRect = highlightedRect;
    highlightedRect = QRect();
    update(updateRect);
    event->accept();
}

void PuzzleWidget::removePiece(const QRect &rect)
{
    int idx = findPiece(rect);
    if (idx != -1) {
        pieces.removeAt(idx);
        update(rect);
    }
}


void PuzzleWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    QRect square = targetSquare(event->pos());
    int idx = findPiece(square);
    if (idx == -1)
        return;

    Piece piece = pieces.takeAt(idx);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << piece.pixmap << piece.location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(PiecesList::puzzleMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - square.topLeft());
    drag->setPixmap(piece.pixmap);

    if (drag->exec(Qt::MoveAction) != Qt::MoveAction)
    {
        // Drag gagal → kembalikan piece
        pieces.insert(idx, piece);
    }
    update(square);
}

void PuzzleWidget::rotatePieceAt(const QRect &square)
{
    int idx = findPiece(square);
    if (idx == -1) return;

    Piece &p = pieces[idx];
    p.rotation = (p.rotation + 90) % 360;
    p.pixmap = p.original.transformed(QTransform().rotate(p.rotation), Qt::SmoothTransformation);

    update(p.rect);
}

void PuzzleWidget::addPieceWithRotation(const QPixmap &pixmap, const QPoint &location, int rotation)
{
    Piece piece;
    piece.pixmap = pixmap.transformed(QTransform().rotate(rotation), Qt::SmoothTransformation);
    piece.original = pixmap;
    piece.rotation = rotation;
    piece.location = location;
    piece.rect = QRect(location * pieceSize(), QSize(pieceSize(), pieceSize()));

    pieces.append(piece);
}

void PuzzleWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::white);

    if (highlightedRect.isValid())
    {
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


