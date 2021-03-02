#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RapidClick.h"

#include "click.h"

class RapidClick final: public QMainWindow
{
  Q_OBJECT

  Clicker cl;

  qint32 last_activate = 0x00;
  qint32 last_output = 0x00;
public:
  RapidClick(QWidget *parent = Q_NULLPTR);

private:
  Ui::RapidClickClass ui;

  bool event(QEvent*);

private slots:
  void handleActivateChanged(qint32);
  void handleOutputChanged(qint32);
  void handleModeChanged(int);
};
