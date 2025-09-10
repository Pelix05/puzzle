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
    QRect updateRect = highlightedRect.united(targetSquare(event->position().toPoint()));
    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType())
        && findPiece(targetSquare(event->position().toPoint())) == -1)
    {
        highlightedRect = targetSquare(event->position().toPoint());
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
    if (!event->mimeData()->hasFormat(PiecesList::puzzleMimeType())) {
        event->ignore();
        return;
    }

    QByteArray pieceData = event->mimeData()->data(PiecesList::puzzleMimeType());
    QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
    QPixmap pixmap;
    QPoint location;
    int rotation;
    dataStream >> pixmap >> location >> rotation;

    QRect targetRect = targetSquare(event->position().toPoint());
    if (findPiece(targetRect) != -1) {
        highlightedRect = QRect();
        event->ignore();
        return;
    }

    // Resize the piece to exactly match the target square
    QPixmap resizedPiece = pixmap.scaled(targetRect.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    Piece piece;
    piece.original = pixmap;
    piece.pixmap = resizedPiece.transformed(QTransform().rotate(rotation), Qt::SmoothTransformation);
    piece.rotation = rotation;
    piece.rect = targetRect;
    piece.location = location;

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

    bool allCorrect = true;

    for (const Piece &p : pieces) {
        QRect correctRect(p.location * pieceSize(), QSize(pieceSize(), pieceSize()));
        bool posOk = (p.rect == correctRect);
        bool rotOk = (p.rotation == 0);

        qDebug() << "[CheckCompletion] Piece at" << p.location
                 << "rect =" << p.rect
                 << "correctRect =" << correctRect
                 << "rotation =" << p.rotation
                 << "posOk =" << posOk
                 << "rotOk =" << rotOk;

        if (!posOk || !rotOk)
            allCorrect = false;
    }

    if (allCorrect) {
        qDebug() << "[PuzzleWidget] Puzzle completed!";
        emit puzzleCompleted();
    }
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
void PuzzleWidget::clear()
{
    pieces.clear();
    highlightedRect = QRect();
    update();
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
    // simpan juga rotation supaya tidak hilang
    dataStream << piece.original << piece.location << piece.rotation;

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
    p.rotation = (p.rotation + 90) % 360;  // update rotasi di data
    p.pixmap = p.original.transformed(QTransform().rotate(p.rotation), Qt::SmoothTransformation);

    qDebug() << "[Rotate] Piece at" << p.location << "rotation now =" << p.rotation;

    update(p.rect);
    checkCompletion();  // cek setiap kali rotate
}

void PuzzleWidget::addPieceWithRotation(const QPixmap &pixmap, const QPoint &location,
                                        int rotation, const QRect &rect)
{
    Piece piece;
    piece.pixmap = pixmap.transformed(QTransform().rotate(rotation), Qt::SmoothTransformation);
    piece.original = pixmap;
    piece.rotation = rotation;
    piece.location = location;
    piece.rect = rect; // ← Gunakan rect yang diberikan, bukan yang dihitung ulang

    pieces.append(piece);
}

void PuzzleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    int ps = pieceSize(); // current size of each grid cell

    for (const Piece &p : pieces) {
        // Scale each piece to fill the grid cell
        painter.drawPixmap(p.rect.topLeft(), p.pixmap.scaled(ps, ps, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}


void PuzzleWidget::setPuzzleImage(const QPixmap &pix)
{
    m_fullPuzzleImage = pix;
    pieces.clear();

    int fullWidth = m_fullPuzzleImage.width();
    int fullHeight = m_fullPuzzleImage.height();
    int pieceWidth = fullWidth / m_GridSize;
    int pieceHeight = fullHeight / m_GridSize;

    for (int y = 0; y < m_GridSize; ++y)
    {
        for (int x = 0; x < m_GridSize; ++x)
        {
            QPixmap piece = m_fullPuzzleImage.copy(
                x * pieceWidth, y * pieceHeight, pieceWidth, pieceHeight
                );

            Piece p;
            p.original = piece;
            p.pixmap = piece;
            p.rect = QRect(x * pieceWidth, y * pieceHeight, pieceWidth, pieceHeight);
            p.location = QPoint(x, y);
            p.rotation = 0;
            pieces.append(p);
        }
    }

    update();
}

int PuzzleWidget::pieceSize() const
{
    int size = qMin(width(), height()); // use current widget size
    return size / m_GridSize;
}


const QRect PuzzleWidget::targetSquare(const QPoint &position) const
{
    return QRect(position / pieceSize() * pieceSize(),
                 QSize(pieceSize(), pieceSize()));
}



