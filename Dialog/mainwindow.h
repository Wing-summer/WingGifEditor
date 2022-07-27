#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Class/clipboardhelper.h"
#include "Class/playgifmanager.h"
#include "GifImage/gifhelper.h"
#include "GifImage/gifimage.h"
#include <DGraphicsView>
#include <DLabel>
#include <DMainWindow>
#include <DStatusBar>
#include <QGraphicsPixmapItem>
#include <QListWidget>

DWIDGET_USE_NAMESPACE

enum GifError {
  E_GIF_ERR_OPEN_FAILED = 1,
  E_GIF_ERR_WRITE_FAILED,
  E_GIF_ERR_HAS_SCRN_DSCR,
  E_GIF_ERR_HAS_IMAG_DSCR,
  E_GIF_ERR_NO_COLOR_MAP,
  E_GIF_ERR_DATA_TOO_BIG,
  E_GIF_ERR_NOT_ENOUGH_MEM,
  E_GIF_ERR_DISK_IS_FULL,
  E_GIF_ERR_CLOSE_FAILED,
  E_GIF_ERR_NOT_WRITEABLE,
  D_GIF_ERR_OPEN_FAILED = 101,
  D_GIF_ERR_READ_FAILED,
  D_GIF_ERR_NOT_GIF_FILE,
  D_GIF_ERR_NO_SCRN_DSCR,
  D_GIF_ERR_NO_IMAG_DSCR,
  D_GIF_ERR_NO_COLOR_MAP,
  D_GIF_ERR_WRONG_RECORD,
  D_GIF_ERR_DATA_TOO_BIG,
  D_GIF_ERR_NOT_ENOUGH_MEM,
  D_GIF_ERR_CLOSE_FAILED,
  D_GIF_ERR_NOT_READABLE,
  D_GIF_ERR_IMAGE_DEFECT,
  D_GIF_ERR_EOF_TOO_SOON
};

class MainWindow : public DMainWindow {
  Q_OBJECT
public:
  MainWindow(DMainWindow *parent = nullptr);

private:
  void refreshImglist();
  void refreshListLabel(int start = 0);

private:
  void on_new_frompics();
  void on_new_fromgifs();
  void on_open();
  void on_save();
  void on_saveas();
  void on_export();
  void on_exit();

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
  void on_fliph();
  void on_flipv();
  void on_clockwise();
  void on_anticlockwise();
  void on_exportapply();
  void on_applypic();

  void on_about();
  void on_sponsor();
  void on_wiki();

private:
  DGraphicsView *editor;
  QListWidget *imglist;
  DStatusBar *status;
  GifHelper gif;

  QString lastusedpath;
  QString curfilename;
  QGraphicsScene scene;

  QGraphicsPixmapItem *picview;

  PlayGifManager *player;
  ClipBoardHelper *clip;

protected:
  void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
