#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Class/clipboardhelper.h"
#include "Class/gifeditor.h"
#include "Class/playgifmanager.h"
#include "Class/settings.h"
#include "Dialog/waitingdialog.h"
#include "GifImage/decoder/gifdecoder.h"
#include "GifImage/encoder/GifEncoder.h"
#include "cropgifdialog.h"
#include <DGraphicsView>
#include <DLabel>
#include <DMainWindow>
#include <DStatusBar>
#include <QGraphicsPixmapItem>
#include <QListWidget>
#include <QUndoStack>

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow {
  Q_OBJECT
public:
  MainWindow(DMainWindow *parent = nullptr);

  void openGif(QString filename);
  bool checkIsGif(QString filename);

private:
  void refreshImglist();
  void refreshListLabel(int start = 0);
  void setEditMode(bool b);
  bool ensureSafeClose();
  void setSaved(bool b);
  void setWritable(bool b);
  void showGifMessage(QString message = "");
  bool saveGif(QString filename);

  QRect adjustImageSize(int x, int y, int w, int h);

private:
  void on_new_frompics();
  void on_new_fromgifs();
  void on_open();
  void on_save();
  void on_saveas();
  void on_export();
  void on_close();
  void on_setting();

  void on_undo();
  void on_redo();
  void on_copy();
  void on_cut();
  void on_paste();
  void on_del();
  void on_selall();
  void on_selreverse();
  void on_desel();
  void on_goto();

  void on_beginframe();
  void on_last();
  void on_play();
  void on_stop();
  void on_next();
  void on_endframe();

  void on_decreaseframe();
  void on_delbefore();
  void on_delafter();
  void on_reverse();
  void on_moveleft();
  void on_moveright();
  void on_createreverse();
  void on_setdelay();
  void on_scaledelay();
  void on_insertpic();
  void on_merge();

  void on_scalepic();
  void on_cutpic();
  void on_crop();
  void on_fliph();
  void on_flipv();
  void on_clockwise();
  void on_anticlockwise();
  void on_exportapply();
  void on_applypic();

  void on_onion();
  void on_fullscreen();
  void on_about();
  void on_sponsor();
  void on_wiki();

private:
  QUndoStack undo;
  GifEditor *editor;
  QListWidget *imglist;
  DStatusBar *status;

  QString lastusedpath;
  QString curfilename;

  GifDecoder gif;

  PlayGifManager *player;

  CropGifDialog *cuttingdlg;

  // DToolBar *toolmain, *toolplay, *tooledit, *tooleffect;
  QList<QAction *> dismenu, distool;
  QAction *undotool, *undomenu, *redotool, *redomenu;

  Settings *m_settings;
  QString _windowmode;
  int _quality = 10;

protected:
  void closeEvent(QCloseEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;

private:
  DIconButton *iSetfitInView;

  DLabel *iReadWrite;
  DLabel *iSaved;
  DLabel *iw;

  QPixmap infoSaved;
  QPixmap infoUnsaved;
  QPixmap infoSaveg;
  QPixmap infoReadonly;
  QPixmap infoWriteable;
  QPixmap inforwg;
};

#endif // MAINWINDOW_H
