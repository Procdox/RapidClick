#include "RapidClick.h"

#include <QDebug>
#include <QMouseEvent>

RapidClick::RapidClick(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);

  bool res = true;
  res &= (bool)connect(ui.keys_activate, &NativeKeySequence::nativeReady, this, &RapidClick::handleActivateChanged);
  res &= (bool)connect(ui.keys_output, &NativeKeySequence::nativeReady, this, &RapidClick::handleOutputChanged);
  res &= (bool)connect(ui.mode, SIGNAL(activated(int)), this, SLOT(handleModeChanged(int)));

  Q_ASSERT(res);

  ui.keys_activate->set(0x25,"Left");
  ui.keys_output->set(0x01);
}

bool RapidClick::event(QEvent *event){
  if( event->type()==QEvent::Enter ) {
    qDebug() << "entering";
    cl.pause();
  }
  else if( event->type()==QEvent::Leave ) {
    qDebug() << "leaving";
    if( ui.enabled->isChecked() ) {
      if( ui.mode->currentIndex() == 0)
        cl.enableHold();
      else
        cl.enableToggle();
    }
  }

  return QMainWindow::event(event);
}


void RapidClick::handleActivateChanged(qint32 key) {
  if(key == last_output) {
    ui.enabled->setChecked(false);
    cl.pause(); 
  }
  cl.setActivate(key);
  last_activate = key;
}
void RapidClick::handleOutputChanged(qint32 key) {
  if(key == last_activate) {
    ui.enabled->setChecked(false);
    cl.pause(); 
  }
  cl.setOutput(key);
  last_output = key;
}
void RapidClick::handleModeChanged(int index) {
  if( cl.isEnabled() ) {
    if( index == 0)
      cl.enableHold();
    else
      cl.enableToggle();
  }
}




