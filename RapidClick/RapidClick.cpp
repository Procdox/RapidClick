#include "RapidClick.h"

#include <QSettings>
#include <QDebug>

RapidClick::~RapidClick() {
  qDebug() << "Saving...";
  qDebug() << ui.keys_activate->text() << ui.keys_output->text() << last_activate << last_output << ui.mode->currentIndex();
  QSettings settings;
  settings.beginGroup("rapid");
  settings.setValue("last_activate", (int)last_activate);
  settings.setValue("last_output", (int)last_output);
  settings.setValue("title_activate", ui.keys_activate->text());
  settings.setValue("title_output", ui.keys_output->text());
  settings.setValue("mode", ui.mode->currentIndex());
  settings.endGroup();
}

RapidClick::RapidClick(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);

  bool res = true;
  res &= (bool)connect(ui.keys_activate, &NativeKeySequence::nativeReady, this, &RapidClick::handleActivateChanged);
  res &= (bool)connect(ui.keys_output, &NativeKeySequence::nativeReady, this, &RapidClick::handleOutputChanged);

  Q_ASSERT(res);

  QSettings settings;
  settings.beginGroup("rapid");
  const auto last_activate = settings.value("last_activate",0x25).toInt();
  const auto last_output = settings.value("last_output",0x01).toInt();
  const auto title_activate = settings.value("title_activate","Left").toString();
  const auto title_output = settings.value("title_output","").toString();
  const auto mode = settings.value("mode",0).toInt();
  settings.endGroup();

  qDebug() << "Loading...";
  qDebug() << title_activate << title_output << last_activate << last_output << mode;

  ui.keys_activate->set(last_activate, title_activate);
  ui.keys_output->set(last_output, title_output);
  ui.mode->setCurrentIndex(mode);
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




