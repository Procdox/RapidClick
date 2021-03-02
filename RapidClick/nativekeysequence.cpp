#include "nativekeysequence.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

NativeKeySequence::~NativeKeySequence() {
}
NativeKeySequence::NativeKeySequence(QWidget* parent)
: QLineEdit(parent) {
  setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
}

void NativeKeySequence::set(qint32 v, const QString& s) {
  if(v == last || v == 0x0)
    return;

  last = v;

  if(last == 0x01) {
    setText("Left Mouse");
  }
  else if(last == 0x02) {
    setText("Right Mouse");
  }
  else {
    setText(s);
  }
  emit nativeReady(last);
}

bool NativeKeySequence::event(QEvent *event) {
  
  if(event->type() == QEvent::KeyPress) {
    auto* key_event = static_cast<QKeyEvent*>(event);
    if( key_event->key() <= 0x01000017 && key_event->key() != last){
      set(key_event->nativeVirtualKey(), QKeySequence(key_event->key()).toString() );
    }
    return true;
  }
  else if(event->type() == QEvent::MouseButtonDblClick) {
    auto click_event = static_cast<QMouseEvent*>(event);
    if(click_event->button() == Qt::MouseButton::LeftButton) {
      set(0x01);
    }
    else if(click_event->button() == Qt::MouseButton::RightButton) {
      set(0x02);
    }
    return true;
  }
  else if(event->type() == QEvent::Leave) {
    clearFocus();
  }

  return QLineEdit::event(event);
}