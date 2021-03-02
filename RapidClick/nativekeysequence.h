#pragma once

#include <QLineEdit>

class NativeKeySequence : public QLineEdit {
  Q_OBJECT

  int last = 0x00;

public:
  ~NativeKeySequence();
  NativeKeySequence(QWidget*);

  void set(qint32, const QString& s = {});

private:
  bool event(QEvent*);

signals:
  void nativeReady(qint32);
};