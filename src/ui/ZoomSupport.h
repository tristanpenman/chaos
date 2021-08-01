#pragma once

#include <QObject>
#include <QGraphicsView>

/**
 * Zoom support
 *
 * Based on this Stack Overflow post: https://stackoverflow.com/a/19114517
 *
 * This class adds ability to zoom a QGraphicsView using the mouse scroll wheel. The point under cursor
 * remains in the same position while scrolling.
 *
 * When the user starts scrolling, this class remembers original scene position and keeps it until scrolling is
 * completed. It's better than getting original scene position at each scrolling step because that approach leads to
 * position errors due positioning restrictions.
 *
 * When zoomed using the scroll wheel, this class emits zoomed() signal.
 *
 * Usage:
 *
 *   new ZoomSupport(view);
 *
 * The object will be deleted automatically when the view is deleted.
 *
 * You can set keyboard modifiers used for zooming using setModified(). Zooming will be performed only on exact match
 * of modifiers combination. The default modifier is the meta key.
 *
 * You can change zoom velocity by calling setZoomFactorBase(). The default zoom factor base is 1.0015.
 *
 * The zoom coefficient is calculated as zoomFactorBase ^ angleDelta (see QWheelEvent::angleDelta).
 */
class ZoomSupport : public QObject
{
  Q_OBJECT

public:
  ZoomSupport(QGraphicsView* view);

  void gentleZoom(double factor);
  void setModifiers(Qt::KeyboardModifiers modifiers);
  void setZoomFactorBase(double value);

private:
  bool eventFilter(QObject* object, QEvent* event);

  QGraphicsView* m_view;
  Qt::KeyboardModifiers m_modifiers;
  double m_zoomFactorBase;
  QPointF m_targetScenePos;
  QPointF m_targetViewportPos;

signals:
  void zoomed();
};
