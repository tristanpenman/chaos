#pragma once

#include <QColor>
#include <QGraphicsItem>
#include <QPainter>

class Rectangle : public QGraphicsItem
{
public:
    Rectangle(int width, int height, const QColor& colour);

    void setColor(const QColor& color);
    void setSize(int width, int height);

    QRectF boundingRect() const override;

private:
    QRectF m_boundingRect;
    QColor m_color;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
};

inline Rectangle::Rectangle(int width, int height, const QColor& color)
  : QGraphicsItem()
  , m_boundingRect(0, 0, width, height)
  , m_color(color)
{

}

inline void Rectangle::setColor(const QColor &color)
{
  m_color = color;
}

inline void Rectangle::setSize(int width, int height)
{
  m_boundingRect.setWidth(width);
  m_boundingRect.setHeight(height);
}

inline QRectF Rectangle::boundingRect() const
{
  return m_boundingRect;
}

inline void Rectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  painter->setPen(Qt::NoPen);
  painter->setBrush(m_color);
  painter->drawRect(m_boundingRect);
}
