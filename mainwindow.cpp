#include "mainwindow.h"
#include "sponsordialog.h"
#include <DInputDialog>
#include <DMenu>
#include <DMessageManager>
#include <DTitlebar>
#include <DToolBar>
#include <DToolButton>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QShortcut>
#include <QVBoxLayout>
#include <QWidget>

#define ICONRES(name) QIcon(":/images/" name ".png")

MainWindow::MainWindow(DMainWindow *parent) : DMainWindow(parent) {
  setMinimumSize(800, 600);
  setWindowTitle(tr("WingGifEditor"));
  setWindowIcon(ICONRES("icon"));
  auto w = new QWidget(this);
  setCentralWidget(w);
  auto vlayout = new QVBoxLayout(w);
  editor = new DGraphicsView(w);
  editor->setInteractive(true);
  picview = scene.addPixmap(QPixmap(":/images/icon.png"));
  picview->setScale(1);
  editor->setScene(&scene);
  vlayout->addWidget(editor);
  imglist = new QListWidget(w);
  imglist->setFlow(QListWidget::TopToBottom);
  imglist->setViewMode(QListWidget::IconMode);
  imglist->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
  imglist->setSelectionMode(
      QAbstractItemView::SelectionMode::ExtendedSelection);
  imglist->setIconSize(QSize(100, 100));
  imglist->setFixedHeight(140);
  imglist->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
  imglist->setResizeMode(QListView::Adjust);
  vlayout->addWidget(imglist);

  connect(imglist, &QListWidget::itemSelectionChanged, this,
          [=] { picview->setPixmap(gif.frameimg(imglist->currentRow())); });

  auto title = titlebar();
  title->setIcon(QIcon()); // TODO
  title->setTitle(tr("WingGifEditor"));

  auto menu = new DMenu(this);
  QAction *a;

  auto keyselrev =
      QKeySequence(Qt::KeyboardModifier::ControlModifier | Qt::Key_I);
  auto keygoto =
      QKeySequence(Qt::KeyboardModifier::ControlModifier | Qt::Key_G);
  auto keybeginframe = QKeySequence(Qt::Key_Home);
  auto keylastframe = QKeySequence(Qt::Key_Left);
  auto keynextframe = QKeySequence(Qt::Key_Right);
  auto keyendframe = QKeySequence(Qt::Key_End);
  auto keyplay = QKeySequence(Qt::Key_F5);
  auto keypause = QKeySequence(Qt::Key_F10);
  auto keyreduceframe =
      QKeySequence(Qt::KeyboardModifier::ControlModifier | Qt::Key_D);
  auto keyrmleft =
      QKeySequence(Qt::KeyboardModifier::ControlModifier | Qt::Key_J);
  auto keyrmright =
      QKeySequence(Qt::KeyboardModifier::ControlModifier | Qt::Key_K);
  auto keyrev = QKeySequence(Qt::KeyboardModifier::ControlModifier |
                             Qt::KeyboardModifier::ShiftModifier | Qt::Key_I);
  auto keymvl =
      QKeySequence(Qt::KeyboardModifier::ControlModifier | Qt::Key_BracketLeft);
  auto keymvr = QKeySequence(Qt::KeyboardModifier::ControlModifier |
                             Qt::Key_BracketRight);
  auto keysetdelay =
      QKeySequence(Qt::KeyboardModifier::ControlModifier | Qt::Key_T);
  auto keyscaledelay =
      QKeySequence(Qt::KeyboardModifier::ControlModifier |
                   Qt::KeyboardModifier::ShiftModifier | Qt::Key_T);
  auto keyscalepic =
      QKeySequence(Qt::KeyboardModifier::ShiftModifier | Qt::Key_T);
  auto keycutpic =
      QKeySequence(Qt::KeyboardModifier::ControlModifier |
                   Qt::KeyboardModifier::ShiftModifier | Qt::Key_X);

#define AddMenuIconAction(Icon, Title, Slot, Owner)                            \
  a = new QAction(Owner);                                                      \
  a->setText(Title);                                                           \
  a->setIcon(ICONRES(Icon));                                                   \
  connect(a, &QAction::triggered, this, &Slot);                                \
  Owner->addAction(a);

#define AddMenuAction(Title, Slot, Owner)                                      \
  a = new QAction(Owner);                                                      \
  a->setText(Title);                                                           \
  connect(a, &QAction::triggered, this, &Slot);                                \
  Owner->addAction(a);

#define AddMenuShortcutAction(Icon, Title, Slot, Owner, ShortCut)              \
  AddMenuIconAction(Icon, Title, Slot, Owner);                                 \
  a->setShortcut(ShortCut);                                                    \
  a->setShortcutVisibleInContextMenu(true);

#define AddToolSubMenuShortcutAction(Icon, Title, Slot, ShortCut)              \
  AddMenuShortcutAction(Icon, Title, Slot, tm, ShortCut)

  auto tm = new QMenu(this);
  tm->setIcon(ICONRES("file"));
  tm->setTitle(tr("File"));

  auto m = new QMenu(this);
  m->setIcon(ICONRES("new"));
  m->setTitle(tr("New"));
  AddMenuIconAction("pics", tr("NewFromPics"), MainWindow::on_new_frompics, m);
  AddMenuIconAction("gifs", tr("NewFromGifs"), MainWindow::on_new_fromgifs, m);
  tm->addMenu(m);

  AddMenuShortcutAction("open", tr("Open"), MainWindow::on_open, tm,
                        QKeySequence::Open);
  AddMenuShortcutAction("save", tr("Save"), MainWindow::on_save, tm,
                        QKeySequence::Save);
  AddMenuShortcutAction("saveas", tr("SaveAs"), MainWindow::on_saveas, tm,
                        QKeySequence::SaveAs);
  AddMenuIconAction("export", tr("Export"), MainWindow::on_export, tm);
  tm->addSeparator();
  AddMenuShortcutAction("close", tr("Close"), MainWindow::on_exit, tm,
                        QKeySequence::Close);

  menu->addMenu(tm);

  tm = new QMenu(this);
  tm->setTitle(tr("Edit"));
  tm->setIcon(ICONRES("edit"));
  AddMenuShortcutAction("undo", tr("Undo"), MainWindow::on_undo, tm,
                        QKeySequence::Undo);
  AddMenuShortcutAction("redo", tr("Redo"), MainWindow::on_redo, tm,
                        QKeySequence::Redo);
  tm->addSeparator();
  AddMenuShortcutAction("cut", tr("Cut"), MainWindow::on_cut, tm,
                        QKeySequence::Cut);
  AddMenuShortcutAction("copy", tr("Copy"), MainWindow::on_copy, tm,
                        QKeySequence::Copy);
  AddMenuShortcutAction("paste", tr("Paste"), MainWindow::on_paste, tm,
                        QKeySequence::Paste);
  AddMenuShortcutAction("del", tr("Delete"), MainWindow::on_del, tm,
                        QKeySequence::Delete);
  tm->addSeparator();
  AddMenuShortcutAction("selall", tr("SelectAll"), MainWindow::on_selall, tm,
                        QKeySequence::SelectAll);
  AddMenuShortcutAction("desel", tr("Deselect"), MainWindow::on_desel, tm,
                        QKeySequence::Deselect);
  AddMenuShortcutAction("selrev", tr("ReverseSelection"),
                        MainWindow::on_selreverse, tm, keyselrev);
  tm->addSeparator();
  AddMenuShortcutAction("jmp", tr("Goto"), MainWindow::on_goto, tm, keygoto);
  menu->addMenu(tm);

  tm = new QMenu(this);
  tm->setIcon(ICONRES("play"));
  tm->setTitle(tr("Play"));
  AddMenuShortcutAction("first", tr("FirstFrame"), MainWindow::on_beginframe,
                        tm, keybeginframe);
  AddMenuShortcutAction("back", tr("LastFrame"), MainWindow::on_last, tm,
                        keylastframe);
  AddMenuShortcutAction("gifplay", tr("Play"), MainWindow::on_play, tm,
                        keyplay);
  AddMenuShortcutAction("pause", tr("Stop"), MainWindow::on_stop, tm, keypause);
  AddMenuShortcutAction("foreword", tr("NextFrame"), MainWindow::on_next, tm,
                        keynextframe);
  AddMenuShortcutAction("last", tr("EndFrame"), MainWindow::on_endframe, tm,
                        keylastframe);
  title->setMenu(menu);

  tm = new QMenu(this);
  tm->setIcon(ICONRES("pic"));
  tm->setTitle(tr("Picture"));
  AddMenuShortcutAction("rmframe", tr("ReduceFrame"),
                        MainWindow::on_decreaseframe, tm, keyreduceframe);
  AddMenuShortcutAction("rml", tr("DeleteBefore"), MainWindow::on_delbefore, tm,
                        keyrmleft);
  AddMenuShortcutAction("rmr", tr("DeleteAfter"), MainWindow::on_delafter, tm,
                        keyrmright);
  tm->addSeparator();
  AddMenuShortcutAction("mvf", tr("MoveLeft"), MainWindow::on_moveleft, tm,
                        keymvl);
  AddMenuShortcutAction("mvb", tr("MoveRight"), MainWindow::on_moveright, tm,
                        keymvr);
  AddMenuShortcutAction("reverse", tr("Reverse"), MainWindow::on_reverse, tm,
                        keyrev);
  AddMenuIconAction("reverseplus", tr("CreateReverse"),
                    MainWindow::on_createreverse, tm);
  tm->addSeparator();
  AddMenuShortcutAction("setdelay", tr("SetDelay"), MainWindow::on_setdelay, tm,
                        keysetdelay);
  AddMenuShortcutAction("scaledelay", tr("ScaleDelay"),
                        MainWindow::on_scaledelay, tm, keyscaledelay);
  AddMenuIconAction("pics", tr("InsertPics"), MainWindow::on_insertpic, tm);
  AddMenuIconAction("gifs", tr("MergeGIfs"), MainWindow::on_merge, tm);
  AddMenuShortcutAction("scale", tr("ScaleGif"), MainWindow::on_scalepic, tm,
                        keyscalepic);
  AddMenuShortcutAction("cutpic", tr("CutGif"), MainWindow::on_cutpic, tm,
                        keycutpic);
  tm->addSeparator();
  AddMenuIconAction("fliph", tr("FilpH"), MainWindow::on_fliph, tm);
  AddMenuIconAction("flipv", tr("FlipV"), MainWindow::on_flipv, tm);
  AddMenuIconAction("rotatel", tr("RotateLeft"), MainWindow::on_clockwise, tm);
  AddMenuIconAction("rotater", tr("RotateR"), MainWindow::on_anticlockwise, tm);
  tm->addSeparator();
  AddMenuIconAction("blank", tr("ExportBlank"), MainWindow::on_exportapply, tm);
  AddMenuIconAction("model", tr("ApplyModel"), MainWindow::on_applypic, tm);
  menu->addMenu(tm);

  tm = new DMenu(this);
  tm->setTitle(tr("Author"));
  tm->setIcon(ICONRES("author"));
  AddMenuIconAction("soft", tr("About"), MainWindow::on_about, tm);
  AddMenuIconAction("sponsor", tr("Sponsor"), MainWindow::on_sponsor, tm);
  AddMenuIconAction("wiki", tr("Wiki"), MainWindow::on_wiki, tm);
  a = new QAction(ICONRES("qt"), tr("AboutQT"), tm);
  connect(a, &QAction::triggered, this, [=] { QMessageBox::aboutQt(this); });
  tm->addAction(a);
  menu->addMenu(tm);

  auto toolbar = new DToolBar(this);

#define AddToolBarAction(Icon, Owner, Slot, ToolTip)                           \
  a = new QAction(Owner);                                                      \
  a->setIcon(ICONRES(Icon));                                                   \
  connect(a, &QAction::triggered, this, &Slot);                                \
  a->setToolTip(ToolTip);                                                      \
  Owner->addAction(a);

#define AddToolBarTool(Icon, Slot, ToolTip)                                    \
  AddToolBarAction(Icon, toolbar, Slot, ToolTip)

  DToolButton *tbtn;
  DMenu *tmenu;

#define AddToolBtnBegin(DIcon)                                                 \
  tbtn = new DToolButton(this);                                                \
  tbtn->setIcon(ICONRES(DIcon));                                               \
  tmenu = new DMenu(this);

#define AddToolBtnBtn(Icon, Title, Slot)                                       \
  a = new QAction(ICONRES(Icon), Title, this);                                 \
  connect(a, &QAction::triggered, this, &Slot);                                \
  tmenu->addAction(a);

#define AddToolBtnEnd()                                                        \
  tbtn->setMenu(tmenu);                                                        \
  tbtn->setPopupMode(DToolButton::ToolButtonPopupMode::InstantPopup);          \
  toolbar->addWidget(tbtn);

  AddToolBtnBegin("new") {
    AddToolBtnBtn("pics", tr("NewFromPics"), MainWindow::on_new_frompics);
    AddToolBtnBtn("gifs", tr("NewFromGifs"), MainWindow::on_new_fromgifs);
  }
  AddToolBtnEnd();

  AddToolBarTool("open", MainWindow::on_open, tr("Open"));
  AddToolBarTool("save", MainWindow::on_save, tr("Save"));
  AddToolBarTool("saveas", MainWindow::on_saveas, tr("SaveAs"));
  AddToolBarTool("export", MainWindow::on_export, tr("Export"));
  toolbar->addSeparator();
  AddToolBarTool("undo", MainWindow::on_undo, tr("Undo"));
  AddToolBarTool("redo", MainWindow::on_redo, tr("Redo"));
  AddToolBarTool("cut", MainWindow::on_cut, tr("Cut"));
  AddToolBarTool("copy", MainWindow::on_copy, tr("Copy"));
  AddToolBarTool("paste", MainWindow::on_paste, tr("Paste"));
  AddToolBarTool("del", MainWindow::on_del, tr("Delete"));
  AddToolBarTool("selall", MainWindow::on_selall, tr("SelectAll"));
  AddToolBarTool("desel", MainWindow::on_desel, tr("Deselect"));
  AddToolBarTool("selrev", MainWindow::on_selreverse, tr("ReverseSelection"));
  AddToolBarTool("jmp", MainWindow::on_goto, tr("Goto"));
  toolbar->addSeparator();
  AddToolBarTool("soft", MainWindow::on_about, tr("About"));
  AddToolBarTool("sponsor", MainWindow::on_sponsor, tr("Sponsor"));
  AddToolBarTool("wiki", MainWindow::on_wiki, tr("Wiki"));
  toolbar->setObjectName("main");
  addToolBar(toolbar);

  addToolBarBreak();

  toolbar = new DToolBar(this);
  AddToolBarTool("rmframe", MainWindow::on_decreaseframe, tr("ReduceFrame"));
  AddToolBarTool("rml", MainWindow::on_delbefore, tr("DeleteBefore"));
  AddToolBarTool("rmr", MainWindow::on_delafter, tr("DeleteAfter"));
  AddToolBarTool("mvf", MainWindow::on_moveleft, tr("MoveLeft"));
  AddToolBarTool("mvb", MainWindow::on_moveright, tr("MoveRight"));
  AddToolBarTool("reverse", MainWindow::on_reverse, tr("Reverse"));
  AddToolBarTool("reverseplus", MainWindow::on_createreverse,
                 tr("CreateReverse"));
  toolbar->addSeparator();
  AddToolBarTool("setdelay", MainWindow::on_setdelay, tr("SetDelay"));
  AddToolBarTool("scaledelay", MainWindow::on_scaledelay, tr("ScaleDelay"));
  AddToolBarTool("pics", MainWindow::on_insertpic, tr("InsertPics"));
  AddToolBarTool("gifs", MainWindow::on_merge, tr("MergeGIfs"));
  AddToolBarTool("scale", MainWindow::on_scalepic, tr("ScaleGif"));
  AddToolBarTool("cutpic", MainWindow::on_cutpic, tr("CutGif"));
  toolbar->addSeparator();
  AddToolBarTool("fliph", MainWindow::on_fliph, tr("FilpH"));
  AddToolBarTool("flipv", MainWindow::on_flipv, tr("FlipV"));
  AddToolBarTool("rotatel", MainWindow::on_clockwise, tr("RotateLeft"));
  AddToolBarTool("rotater", MainWindow::on_anticlockwise, tr("RotateR"));
  toolbar->addSeparator();
  AddToolBarTool("blank", MainWindow::on_exportapply, tr("ExportBlank"));
  AddToolBarTool("model", MainWindow::on_applypic, tr("ApplyModel"));
  toolbar->setObjectName("pic");
  addToolBar(toolbar);

  toolbar = new DToolBar(this);
  AddToolBarTool("first", MainWindow::on_beginframe, tr("FirstFrame"));
  AddToolBarTool("back", MainWindow::on_last, tr("LastFrame"));
  AddToolBarTool("gifplay", MainWindow::on_play, tr("Play"));
  AddToolBarTool("pause", MainWindow::on_stop, tr("Stop"));
  AddToolBarTool("foreword", MainWindow::on_next, tr("NextFrame"));
  AddToolBarTool("last", MainWindow::on_endframe, tr("EndFrame"));
  toolbar->setObjectName("play");
  addToolBar(toolbar);

  status = new DStatusBar(this);
  setStatusBar(status);

  player = new PlayGifManager(this);
  connect(player, &PlayGifManager::tick, this,
          [=](int index) { imglist->setCurrentRow(index); });
}

void MainWindow::refreshImglist() {
  imglist->clear();
  for (int i = 0; i < gif.frameCount(); i++) {
    new QListWidgetItem(QIcon(gif.frameimg(i)),
                        QString("%1   %2 ms").arg(i).arg(gif.frameDelay(i)),
                        imglist);
  }
}

QString MainWindow::formatErrCode(int err) {
  switch (err) {
  case E_GIF_ERR_OPEN_FAILED:
    return tr("Failed to open given file");
  case E_GIF_ERR_WRITE_FAILED:
    return tr("Failed to write to given file");
  case E_GIF_ERR_HAS_SCRN_DSCR:
    return tr("Screen descriptor has already been set");
  case E_GIF_ERR_HAS_IMAG_DSCR:
    return tr("Image descriptor is still active");
  case E_GIF_ERR_NO_COLOR_MAP:
    return tr("Neither global nor local color map");
  case E_GIF_ERR_DATA_TOO_BIG:
    return tr("Number of pixels bigger than width * height");
  case E_GIF_ERR_NOT_ENOUGH_MEM:
    return tr("Failed to allocate required memory");
  case E_GIF_ERR_DISK_IS_FULL:
    return tr("Write failed (disk full?)");
    break;
  case E_GIF_ERR_CLOSE_FAILED:
    return tr("Failed to close given file");
  case E_GIF_ERR_NOT_WRITEABLE:
    return tr("Given file was not opened for write");
  case D_GIF_ERR_OPEN_FAILED:
    return tr("Failed to open given file");
  case D_GIF_ERR_READ_FAILED:
    return tr("Failed to read from given file");
  case D_GIF_ERR_NOT_GIF_FILE:
    return tr("Data is not in GIF format");
  case D_GIF_ERR_NO_SCRN_DSCR:
    return tr("No screen descriptor detected");
  case D_GIF_ERR_NO_IMAG_DSCR:
    return tr("No Image Descriptor detected");
  case D_GIF_ERR_NO_COLOR_MAP:
    return tr("Neither global nor local color map");
  case D_GIF_ERR_WRONG_RECORD:
    return tr("Wrong record type detected");
  case D_GIF_ERR_DATA_TOO_BIG:
    return tr("Number of pixels bigger than width * height");
    break;
  case D_GIF_ERR_NOT_ENOUGH_MEM:
    return tr("Failed to allocate required memory");
    break;
  case D_GIF_ERR_CLOSE_FAILED:
    return tr("Failed to close given file");
    break;
  case D_GIF_ERR_NOT_READABLE:
    return tr("Given file was not opened for read");
    break;
  case D_GIF_ERR_IMAGE_DEFECT:
    return tr("Image is defective, decoding aborted");
    break;
  case D_GIF_ERR_EOF_TOO_SOON:
    return tr("Image EOF detected before image complete");
    break;
  default:
    return QString();
  }
}

void MainWindow::on_new_frompics() {}

void MainWindow::on_new_fromgifs() {}

void MainWindow::on_open() {
  auto filename = QFileDialog::getOpenFileName(this, tr("ChooseFile"),
                                               lastusedpath, "gif (*.gif)");
  if (filename.isEmpty())
    return;
  lastusedpath = QFileInfo(filename).absoluteDir().absolutePath();
  if (!gif.load(filename)) {
    DMessageManager::instance()->sendMessage(this, ICONRES("icon"),
                                             formatErrCode(gif.getLastError()));
    return;
  }
  curfilename = filename;
  refreshImglist();
  imglist->setCurrentRow(0);
}

void MainWindow::on_del() {
  QList<int> indices;
  for (auto item : imglist->selectionModel()->selectedIndexes()) {
    indices.append(item.row());
  }
  std::sort(indices.begin(), indices.end());
  int offset = 0;
  for (auto item : indices) {
    auto off = item - offset;
    gif.removeFrame(off);
    offset++;
  }
  for (auto item : imglist->selectedItems()) {
    imglist->removeItemWidget(item);
    delete item;
  }
  for (int i = indices.first(); i < imglist->count(); i++) {
    imglist->item(i)->setText(
        QString("%1   %2 ms").arg(i).arg(gif.frameDelay(i)));
  }
}

void MainWindow::on_selall() { imglist->selectAll(); }

void MainWindow::on_selreverse() {
  auto len = imglist->count();
  for (auto i = 0; i < len; i++) {
    auto item = imglist->item(i);
    item->setSelected(!item->isSelected());
  }
}

void MainWindow::on_desel() {
  auto cur = imglist->currentRow();
  imglist->setCurrentRow(cur);
}

void MainWindow::on_goto() {
  bool ok;
  auto index =
      DInputDialog::getInt(this, tr("Goto"), tr("PleaseInputIndex"),
                           imglist->currentRow(), 0, imglist->count(), 1, &ok);
  if (ok) {
    imglist->setCurrentRow(index);
  }
}

void MainWindow::on_beginframe() { imglist->setCurrentRow(0); }

void MainWindow::on_last() {
  auto pos = imglist->currentRow();
  if (pos > 0) {
    imglist->setCurrentRow(pos - 1);
  }
}

void MainWindow::on_play() {
  QList<int> ints;
  auto len = gif.frameCount();
  for (auto i = 0; i < len; i++) {
    ints.append(gif.frameDelay(i));
  }
  player->setTickIntervals(ints);
  player->play(imglist->currentRow());
}

void MainWindow::on_stop() { player->stop(); }

void MainWindow::on_next() {
  auto pos = imglist->currentRow();
  auto max = imglist->count();
  if (pos < max - 1) {
    imglist->setCurrentRow(pos + 1);
  }
}

void MainWindow::on_endframe() { imglist->setCurrentRow(imglist->count() - 1); }

void MainWindow::on_decreaseframe() {}

void MainWindow::on_delbefore() {
  auto len = imglist->currentRow();
  for (auto i = 0; i < len; i++) {
    auto del = imglist->item(0);
    imglist->removeItemWidget(del);
    delete del;
    gif.removeFrame(0);
  }
  len = imglist->count();
  for (auto i = 0; i < len; i++) {
    imglist->item(i)->setText(
        QString("%1   %2 ms").arg(i).arg(gif.frameDelay(i)));
  }
}

void MainWindow::on_delafter() {
  auto len = imglist->count() - imglist->currentRow() - 1;
  auto pos = imglist->currentRow() + 1;
  for (auto i = 0; i < len; i++) {
    auto del = imglist->item(pos);
    imglist->removeItemWidget(del);
    delete del;
    gif.removeFrame(pos);
  }
}

void MainWindow::on_saveas() {
  auto filename = QFileDialog::getSaveFileName(this, tr("ChooseSaveFile"),
                                               lastusedpath, "gif (*.gif)");
  if (filename.isEmpty())
    return;
  lastusedpath = QFileInfo(filename).absoluteDir().absolutePath();
  if (gif.save(filename)) {
    DMessageManager::instance()->sendMessage(this, ICONRES("saveas"),
                                             tr("SaveAsSuccess"));
  } else {
    DMessageManager::instance()->sendMessage(this, ICONRES("saveas"),
                                             formatErrCode(gif.getLastError()));
  }
}

void MainWindow::on_export() {}

void MainWindow::on_exit() { close(); }

void MainWindow::on_undo() {}

void MainWindow::on_redo() {}

void MainWindow::on_copy() {}

void MainWindow::on_cut() {}

void MainWindow::on_paste() {}

void MainWindow::on_save() {
  if (curfilename.isEmpty()) {
    on_saveas();
    return;
  }

  if (gif.save(curfilename)) {
    DMessageManager::instance()->sendMessage(this, ICONRES("save"),
                                             tr("SaveAsSuccess"));
  } else {
    DMessageManager::instance()->sendMessage(this, ICONRES("save"),
                                             formatErrCode(gif.getLastError()));
  }
}

void MainWindow::on_reverse() {
  gif.reverse();
  refreshImglist();
}

void MainWindow::on_moveleft() {
  auto pos = imglist->currentRow();
  if (gif.moveleft(pos)) {
    auto p = imglist->item(pos);
    p->setIcon(QIcon(gif.frameimg(pos)));
    p->setText(QString("%1   %2 ms").arg(pos).arg(gif.frameDelay(pos)));
    pos--;
    p = imglist->item(pos);
    p->setIcon(QIcon(gif.frameimg(pos)));
    p->setText(QString("%1   %2 ms").arg(pos).arg(gif.frameDelay(pos)));
    imglist->setCurrentRow(pos);
  }
}

void MainWindow::on_moveright() {
  auto pos = imglist->currentRow();
  if (gif.moveright(pos)) {
    auto p = imglist->item(pos);
    p->setIcon(QIcon(gif.frameimg(pos)));
    p->setText(QString("%1   %2 ms").arg(pos).arg(gif.frameDelay(pos)));
    pos++;
    p = imglist->item(pos);
    p->setIcon(QIcon(gif.frameimg(pos)));
    p->setText(QString("%1   %2 ms").arg(pos).arg(gif.frameDelay(pos)));
    imglist->setCurrentRow(pos);
  }
}

void MainWindow::on_createreverse() {}

void MainWindow::on_setdelay() {
  auto indices = imglist->selectionModel()->selectedRows();
  bool ok;
  auto time10s =
      DInputDialog::getInt(this, tr("DelayTime"), tr("Input10ms"),
                           gif.defaultDelay() / 10, 1, INT_MAX, 1, &ok);
  if (ok) {
    auto time = time10s * 10;
    if (indices.count()) {
      for (auto i : indices) {
        auto index = i.row();
        gif.setFrameDelay(index, time);
        imglist->item(index)->setText(
            QString("%1   %2 ms").arg(index).arg(time));
      }
    } else {
      gif.setAllFrameDelay(time);
    }
  }
}

void MainWindow::on_scaledelay() {
  auto indices = imglist->selectionModel()->selectedRows();
  bool ok;
  auto scale = DInputDialog::getInt(this, tr("ScaleDelayTime"),
                                    tr("InputPercent"), 100, 1, 100, 1, &ok);
  if (ok) {
    if (indices.count()) {
      for (auto i : indices) {
        auto index = i.row();
        auto time = gif.frameDelay(index);
        time = time * scale / 1000;
        time *= 10;
        gif.setFrameDelay(index, time);
        imglist->item(index)->setText(
            QString("%1   %2 ms").arg(index).arg(time));
      }
    } else {
      gif.scaleAllFrameDelay(scale);
    }
  }
}

void MainWindow::on_insertpic() {}

void MainWindow::on_merge() {
  auto filenames = QFileDialog::getOpenFileNames(this, tr("ChooseFile"),
                                                 lastusedpath, "gif (*.gif)");

  if (filenames.isEmpty())
    return;
  lastusedpath = QFileInfo(filenames.first()).absoluteDir().absolutePath();
  for (auto item : filenames) {
    gif.merge(item);
  }
  refreshImglist();
}

void MainWindow::on_scalepic() {}

void MainWindow::on_cutpic() {}

void MainWindow::on_fliph() {
  auto pos = imglist->currentRow();
  gif.flip(FlipDirection::Horizontal);
  refreshImglist();
  imglist->setCurrentRow(pos);
}

void MainWindow::on_flipv() {
  auto pos = imglist->currentRow();
  gif.flip(FlipDirection::Vertical);
  refreshImglist();
  imglist->setCurrentRow(pos);
}

void MainWindow::on_clockwise() {
  auto pos = imglist->currentRow();
  gif.rotate();
  refreshImglist();
  imglist->setCurrentRow(pos);
}

void MainWindow::on_anticlockwise() {
  auto pos = imglist->currentRow();
  gif.rotate(false);
  refreshImglist();
  imglist->setCurrentRow(pos);
}

void MainWindow::on_exportapply() {
  auto filename = QFileDialog::getSaveFileName(this, tr("ChooseSaveFile"),
                                               lastusedpath, "png (*.png)");
  if (filename.isEmpty())
    return;
  lastusedpath = QFileInfo(filename).absoluteDir().absolutePath();
  QImage img(gif.size(), QImage::Format_RGBA8888);
  img.fill(Qt::transparent);
  img.save(filename);
  DMessageManager::instance()->sendMessage(this, ICONRES("blank"),
                                           tr("ExportSuccess"));
}

void MainWindow::on_applypic() {
  auto indices = imglist->selectionModel()->selectedRows();
  if (!indices.count()) {
    DMessageManager::instance()->sendMessage(this, ICONRES("model"),
                                             tr("NoSelection"));
    return;
  }

  auto filename = QFileDialog::getOpenFileName(this, tr("ChooseFile"),
                                               lastusedpath, "png (*.png)");
  if (filename.isEmpty())
    return;
  lastusedpath = QFileInfo(filename).absoluteDir().absolutePath();
  QImage img;
  if (img.load(filename) && img.size() == gif.size()) {
    for (auto i : indices) {
      auto index = i.row();
      gif.applymodel(img, index);
      imglist->item(index)->setIcon(QIcon(gif.frameimg(index)));
    }
  } else {
    DMessageManager::instance()->sendMessage(this, ICONRES("model"),
                                             tr("InvalidModel"));
  }
}

void MainWindow::on_about() {}

void MainWindow::on_sponsor() {
  SponsorDialog d;
  d.exec();
}

void MainWindow::on_wiki() {}