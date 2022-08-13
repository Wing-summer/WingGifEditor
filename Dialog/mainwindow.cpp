#include "Dialog/mainwindow.h"
#include "Dialog/aboutsoftwaredialog.h"
#include "Dialog/createreversedialog.h"
#include "Dialog/exportdialog.h"
#include "Dialog/newdialog.h"
#include "Dialog/reduceframedialog.h"
#include "Dialog/scalegifdialog.h"
#include "Dialog/sponsordialog.h"
//#include "UndoCommand/delayframecommand.h"
//#include "UndoCommand/delframedircommand.h"
//#include "UndoCommand/flipframecommand.h"
//#include "UndoCommand/insertframecommand.h"
//#include "UndoCommand/moveframecommand.h"
//#include "UndoCommand/reduceframecommand.h"
//#include "UndoCommand/removeframecommand.h"
//#include "UndoCommand/replaceframecommand.h"
//#include "UndoCommand/reverseframecommand.h"
//#include "UndoCommand/rotateframecommand.h"
//#include "UndoCommand/scaleframecommand.h"
#include <DInputDialog>
#include <DMenu>
#include <DMessageManager>
#include <DTitlebar>
#include <DToolBar>
#include <DToolButton>
#include <Magick++.h>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QMimeData>
#include <QRubberBand>
#include <QSettings>
#include <QShortcut>
#include <QVBoxLayout>
#include <QWidget>

#define ICONRES(name) QIcon(":/images/" name ".png")

#define CheckEnabled                                                           \
  if (curfilename.isEmpty())                                                   \
    return;

MainWindow::MainWindow(DMainWindow *parent) : DMainWindow(parent) {
  setMinimumSize(800, 600);
  setWindowTitle(tr("WingGifEditor"));
  setWindowIcon(ICONRES("icon"));
  auto w = new QWidget(this);
  setCentralWidget(w);
  auto vlayout = new QVBoxLayout(w);
  editor = new GifEditor(QPixmap(":/images/icon.png"), w);
  editor->setInteractive(true);
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
  imglist->setDragDropMode(QListWidget::DragDropMode::NoDragDrop);
  imglist->setDefaultDropAction(Qt::DropAction::TargetMoveAction);
  vlayout->addWidget(imglist);

  connect(imglist, &QListWidget::itemSelectionChanged, this, [=] {
    editor->setBackgroudPix(gif.frameimg(imglist->currentRow()));
    this->showGifMessage();
  });

  auto title = titlebar();
  title->setIcon(ICONRES("icon"));
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

#define AddMenuDB()                                                            \
  a->setDisabled(true);                                                        \
  dismenu.append(a)

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
  AddMenuDB();
  AddMenuShortcutAction("saveas", tr("SaveAs"), MainWindow::on_saveas, tm,
                        QKeySequence::SaveAs);
  AddMenuDB();
  AddMenuIconAction("export", tr("Export"), MainWindow::on_export, tm);
  tm->addSeparator();
  AddMenuDB();
  AddMenuShortcutAction("close", tr("Close"), MainWindow::on_exit, tm,
                        QKeySequence::Close);

  menu->addMenu(tm);

  tm = new QMenu(this);
  tm->setTitle(tr("Edit"));
  tm->setIcon(ICONRES("edit"));
  AddMenuShortcutAction("undo", tr("Undo"), MainWindow::on_undo, tm,
                        QKeySequence::Undo);
  a->setDisabled(true);
  undomenu = a;
  AddMenuShortcutAction("redo", tr("Redo"), MainWindow::on_redo, tm,
                        QKeySequence::Redo);
  a->setDisabled(true);
  redomenu = a;
  tm->addSeparator();
  AddMenuShortcutAction("cut", tr("Cut"), MainWindow::on_cut, tm,
                        QKeySequence::Cut);
  AddMenuDB();
  AddMenuShortcutAction("copy", tr("Copy"), MainWindow::on_copy, tm,
                        QKeySequence::Copy);
  AddMenuDB();
  AddMenuShortcutAction("paste", tr("Paste"), MainWindow::on_paste, tm,
                        QKeySequence::Paste);
  AddMenuDB();
  AddMenuShortcutAction("del", tr("Delete"), MainWindow::on_del, tm,
                        QKeySequence::Delete);
  AddMenuDB();
  tm->addSeparator();
  AddMenuShortcutAction("selall", tr("SelectAll"), MainWindow::on_selall, tm,
                        QKeySequence::SelectAll);
  AddMenuDB();
  AddMenuShortcutAction("desel", tr("Deselect"), MainWindow::on_desel, tm,
                        QKeySequence::Deselect);
  AddMenuDB();
  AddMenuShortcutAction("selrev", tr("ReverseSelection"),
                        MainWindow::on_selreverse, tm, keyselrev);
  AddMenuDB();
  tm->addSeparator();
  AddMenuShortcutAction("jmp", tr("Goto"), MainWindow::on_goto, tm, keygoto);
  AddMenuDB();
  menu->addMenu(tm);

  tm = new QMenu(this);
  tm->setIcon(ICONRES("play"));
  tm->setTitle(tr("Play"));
  AddMenuShortcutAction("first", tr("FirstFrame"), MainWindow::on_beginframe,
                        tm, keybeginframe);
  AddMenuDB();
  AddMenuShortcutAction("back", tr("LastFrame"), MainWindow::on_last, tm,
                        keylastframe);
  AddMenuDB();
  AddMenuShortcutAction("gifplay", tr("Play"), MainWindow::on_play, tm,
                        keyplay);
  AddMenuDB();
  AddMenuShortcutAction("pause", tr("Stop"), MainWindow::on_stop, tm, keypause);
  AddMenuDB();
  AddMenuShortcutAction("foreword", tr("NextFrame"), MainWindow::on_next, tm,
                        keynextframe);
  AddMenuDB();
  AddMenuShortcutAction("last", tr("EndFrame"), MainWindow::on_endframe, tm,
                        keylastframe);
  AddMenuDB();
  title->setMenu(menu);

  tm = new QMenu(this);
  tm->setIcon(ICONRES("pic"));
  tm->setTitle(tr("Picture"));
  AddMenuShortcutAction("rmframe", tr("ReduceFrame"),
                        MainWindow::on_decreaseframe, tm, keyreduceframe);
  AddMenuDB();
  AddMenuShortcutAction("rml", tr("DeleteBefore"), MainWindow::on_delbefore, tm,
                        keyrmleft);
  AddMenuDB();
  AddMenuShortcutAction("rmr", tr("DeleteAfter"), MainWindow::on_delafter, tm,
                        keyrmright);
  AddMenuDB();
  tm->addSeparator();
  AddMenuShortcutAction("mvf", tr("MoveLeft"), MainWindow::on_moveleft, tm,
                        keymvl);
  AddMenuDB();
  AddMenuShortcutAction("mvb", tr("MoveRight"), MainWindow::on_moveright, tm,
                        keymvr);
  AddMenuDB();
  AddMenuShortcutAction("reverse", tr("Reverse"), MainWindow::on_reverse, tm,
                        keyrev);
  AddMenuDB();
  tm->addSeparator();
  AddMenuShortcutAction("setdelay", tr("SetDelay"), MainWindow::on_setdelay, tm,
                        keysetdelay);
  AddMenuDB();
  AddMenuIconAction("pics", tr("InsertPics"), MainWindow::on_insertpic, tm);
  AddMenuDB();
  AddMenuIconAction("gifs", tr("MergeGIfs"), MainWindow::on_merge, tm);
  AddMenuDB();
  AddMenuShortcutAction("scale", tr("ScaleGif"), MainWindow::on_scalepic, tm,
                        keyscalepic);
  AddMenuDB();
  AddMenuShortcutAction("cutpic", tr("CutGif"), MainWindow::on_cutpic, tm,
                        keycutpic);
  AddMenuDB();
  tm->addSeparator();
  AddMenuIconAction("fliph", tr("FilpH"), MainWindow::on_fliph, tm);
  AddMenuDB();
  AddMenuIconAction("flipv", tr("FlipV"), MainWindow::on_flipv, tm);
  AddMenuDB();
  AddMenuIconAction("rotatel", tr("RotateLeft"), MainWindow::on_clockwise, tm);
  AddMenuDB();
  AddMenuIconAction("rotater", tr("RotateR"), MainWindow::on_anticlockwise, tm);
  AddMenuDB();
  menu->addMenu(tm);

  tm = new DMenu(this);
  tm->setTitle(tr("Effect"));
  tm->setIcon(ICONRES("effect"));
  AddMenuIconAction("blank", tr("ExportBlank"), MainWindow::on_exportapply, tm);
  AddMenuDB();
  AddMenuIconAction("model", tr("ApplyModel"), MainWindow::on_applypic, tm);
  AddMenuDB();
  tm->addSeparator();
  AddMenuIconAction("reverseplus", tr("CreateReverse"),
                    MainWindow::on_createreverse, tm);
  AddMenuDB();
  AddMenuShortcutAction("scaledelay", tr("ScaleDelay"),
                        MainWindow::on_scaledelay, tm, keyscaledelay);
  AddMenuDB();
  AddMenuIconAction("onion", tr("OnionMask"), MainWindow::on_onion, tm);
  AddMenuDB();
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

#define AddToolDB()                                                            \
  a->setEnabled(false);                                                        \
  distool.append(a);

  AddToolBarTool("open", MainWindow::on_open, tr("Open"));
  AddToolBarTool("save", MainWindow::on_save, tr("Save"));
  AddToolDB();
  AddToolBarTool("saveas", MainWindow::on_saveas, tr("SaveAs"));
  AddToolDB();
  AddToolBarTool("export", MainWindow::on_export, tr("Export"));
  AddToolDB();
  toolbar->addSeparator();
  AddToolBarTool("undo", MainWindow::on_undo, tr("Undo"));
  a->setEnabled(false);
  undotool = a;
  AddToolBarTool("redo", MainWindow::on_redo, tr("Redo"));
  a->setEnabled(false);
  redotool = a;
  AddToolBarTool("cut", MainWindow::on_cut, tr("Cut"));
  AddToolDB();
  AddToolBarTool("copy", MainWindow::on_copy, tr("Copy"));
  AddToolDB();
  AddToolBarTool("paste", MainWindow::on_paste, tr("Paste"));
  AddToolDB();
  AddToolBarTool("del", MainWindow::on_del, tr("Delete"));
  AddToolDB();
  AddToolBarTool("selall", MainWindow::on_selall, tr("SelectAll"));
  AddToolDB();
  AddToolBarTool("desel", MainWindow::on_desel, tr("Deselect"));
  AddToolDB();
  AddToolBarTool("selrev", MainWindow::on_selreverse, tr("ReverseSelection"));
  AddToolDB();
  AddToolBarTool("jmp", MainWindow::on_goto, tr("Goto"));
  AddToolDB();
  toolbar->addSeparator();
  AddToolBarTool("soft", MainWindow::on_about, tr("About"));
  AddToolBarTool("sponsor", MainWindow::on_sponsor, tr("Sponsor"));
  AddToolBarTool("wiki", MainWindow::on_wiki, tr("Wiki"));
  toolbar->setObjectName("main");
  addToolBar(toolbar);

  addToolBarBreak();
  toolbar = new DToolBar(this);
  AddToolBarTool("rmframe", MainWindow::on_decreaseframe, tr("ReduceFrame"));
  AddToolDB();
  AddToolBarTool("rml", MainWindow::on_delbefore, tr("DeleteBefore"));
  AddToolDB();
  AddToolBarTool("rmr", MainWindow::on_delafter, tr("DeleteAfter"));
  AddToolDB();
  AddToolBarTool("mvf", MainWindow::on_moveleft, tr("MoveLeft"));
  AddToolDB();
  AddToolBarTool("mvb", MainWindow::on_moveright, tr("MoveRight"));
  AddToolDB();
  AddToolBarTool("reverse", MainWindow::on_reverse, tr("Reverse"));
  AddToolDB();
  toolbar->addSeparator();
  AddToolBarTool("setdelay", MainWindow::on_setdelay, tr("SetDelay"));
  AddToolDB();
  AddToolBarTool("pics", MainWindow::on_insertpic, tr("InsertPics"));
  AddToolDB();
  AddToolBarTool("gifs", MainWindow::on_merge, tr("MergeGIfs"));
  AddToolDB();
  AddToolBarTool("scale", MainWindow::on_scalepic, tr("ScaleGif"));
  AddToolDB();
  AddToolBarTool("cutpic", MainWindow::on_cutpic, tr("CutGif"));
  AddToolDB();
  toolbar->addSeparator();
  AddToolBarTool("fliph", MainWindow::on_fliph, tr("FilpH"));
  AddToolDB();
  AddToolBarTool("flipv", MainWindow::on_flipv, tr("FlipV"));
  AddToolDB();
  AddToolBarTool("rotatel", MainWindow::on_clockwise, tr("RotateLeft"));
  AddToolDB();
  AddToolBarTool("rotater", MainWindow::on_anticlockwise, tr("RotateR"));
  AddToolDB();
  toolbar->setObjectName("pic");
  addToolBar(toolbar);

  toolbar = new DToolBar(this);
  AddToolBarTool("first", MainWindow::on_beginframe, tr("FirstFrame"));
  AddToolDB();
  AddToolBarTool("back", MainWindow::on_last, tr("LastFrame"));
  AddToolDB();
  AddToolBarTool("gifplay", MainWindow::on_play, tr("Play"));
  AddToolDB();
  AddToolBarTool("pause", MainWindow::on_stop, tr("Stop"));
  AddToolDB();
  AddToolBarTool("foreword", MainWindow::on_next, tr("NextFrame"));
  AddToolDB();
  AddToolBarTool("last", MainWindow::on_endframe, tr("EndFrame"));
  AddToolDB();
  toolbar->setObjectName("play");
  addToolBar(toolbar);

  toolbar = new DToolBar(this);
  AddToolBarTool("blank", MainWindow::on_exportapply, tr("ExportBlank"));
  AddToolDB();
  AddToolBarTool("model", MainWindow::on_applypic, tr("ApplyModel"));
  AddToolDB();
  AddToolBarTool("reverseplus", MainWindow::on_createreverse,
                 tr("CreateReverse"));
  AddToolDB();
  AddToolBarTool("scaledelay", MainWindow::on_scaledelay, tr("ScaleDelay"));
  AddToolDB();
  toolbar->setObjectName("effect");
  addToolBar(toolbar);

  status = new DStatusBar(this);
  DLabel *l;

#define AddNamedStatusLabel(Var, Content)                                      \
  Var = new DLabel(this);                                                      \
  Var->setText(Content);                                                       \
  status->addPermanentWidget(Var);

#define AddStatusLabel(Content) AddNamedStatusLabel(l, Content)
#define LoadPixMap(Var, Icon) Var.load(":/images/" Icon ".png");

#define AddStausILable(PixMap, Icon, Label, OPixMap, OIcon, GPixMap, GIcon)    \
  LoadPixMap(PixMap, Icon);                                                    \
  LoadPixMap(OPixMap, OIcon);                                                  \
  LoadPixMap(GPixMap, GIcon) Label = new DLabel(this);                         \
  Label->setPixmap(GPixMap);                                                   \
  Label->setScaledContents(true);                                              \
  Label->setFixedSize(20, 20);                                                 \
  Label->setAlignment(Qt::AlignCenter);                                        \
  status->addPermanentWidget(Label);                                           \
  AddStatusLabel(QString(' '));

#define AddFunctionIconButton(Var, Icon)                                       \
  Var = new DIconButton(this);                                                 \
  Var->setIcon(ICONRES(Icon));                                                 \
  Var->setIconSize(QSize(20, 20));                                             \
  status->addPermanentWidget(Var);

  AddStausILable(infoSaved, "saved", iSaved, infoUnsaved, "unsaved", infoSaveg,
                 "saveg");
  iSaved->setToolTip(tr("InfoSave"));
  AddStausILable(infoWriteable, "writable", iReadWrite, infoReadonly,
                 "readonly", inforwg, "rwg");
  iReadWrite->setToolTip(tr("InfoReadWrite"));
  AddFunctionIconButton(iSetfitInView, "fitinview");
  iSetfitInView->setToolTip(tr("FitInView"));
  iSetfitInView->setEnabled(false);
  AddStatusLabel(QString(5, ' '));
  connect(iSetfitInView, &DIconButton::clicked, this,
          [=] { editor->fitPicEditor(); });
  setStatusBar(status);

  auto pgif = &gif;
  connect(pgif, &GifDecoder::frameRemoved, this, [=](int index) {
    auto item = imglist->item(index);
    imglist->removeItemWidget(item);
    delete item;
  });
  connect(pgif, &GifDecoder::frameMoved, this, [=](int from, int to) {
    auto max = qMax(from, to);
    auto min = qMin(from, to);
    for (auto i = min; i <= max; i++) {
      auto p = imglist->item(i);
      p->setIcon(gif.thumbnail(i));
      p->setText(QString("%1   %2 ms").arg(i).arg(gif.frameDelay(i)));
    }
  });
  connect(pgif, &GifDecoder::frameRefreshAll, this,
          &MainWindow::refreshImglist);
  connect(pgif, &GifDecoder::frameDelaySet, this, [=](int index, int delay) {
    imglist->item(index)->setText(QString("%1   %2 ms").arg(index).arg(delay));
  });
  connect(pgif, &GifDecoder::frameRefreshImg, this, [=](int index) {
    imglist->item(index)->setIcon(gif.thumbnail(index));
    imglist->itemSelectionChanged();
  });
  connect(pgif, &GifDecoder::frameRefreshLabel, this,
          &MainWindow::refreshListLabel);
  connect(pgif, &GifDecoder::frameMerge, this, [=](int start, int count) {
    auto end = start + count - 1;
    for (int i = end; i >= start; i--) {
      auto p = new QListWidgetItem(
          gif.thumbnail(i),
          QString("%1   %2 ms").arg(i).arg(gif.frameDelay(i)));
      p->setSizeHint(QSize(120, 120));
      imglist->insertItem(start, p);
    }
  });
  connect(pgif, &GifDecoder::frameInsert, this, [=](int index) {
    auto p = new QListWidgetItem(
        gif.thumbnail(index),
        QString("%1   %2 ms").arg(index).arg(gif.frameDelay(index)));
    p->setSizeHint(QSize(120, 120));
    imglist->insertItem(index, p);
  });
  connect(pgif, &GifDecoder::frameImageChanged, this, [=] {
    auto len = imglist->count();
    for (auto i = 0; i < len; i++) {
      imglist->item(i)->setIcon(gif.thumbnail(i));
    }
    editor->setBackgroudPix(gif.frameimg(imglist->currentRow()));
  });

  // clip = new ClipBoardHelper(pgif, this);

  player = new PlayGifManager(this);
  connect(player, &PlayGifManager::tick, this,
          [=](int index) { imglist->setCurrentRow(index); });

  connect(&undo, &QUndoStack::canUndoChanged, this, [=](bool b) {
    CheckEnabled;
    undomenu->setEnabled(b);
    undotool->setEnabled(b);
  });

  connect(&undo, &QUndoStack::canRedoChanged, this, [=](bool b) {
    CheckEnabled;
    redomenu->setEnabled(b);
    redotool->setEnabled(b);
  });
  connect(&undo, &QUndoStack::cleanChanged, this, [=](bool clean) {
    CheckEnabled;
    this->setSaved(clean && curfilename != ":");
  });

  cuttingdlg = new CropGifDialog(this);
  connect(cuttingdlg, &CropGifDialog::selRectChanged, editor,
          &GifEditor::setSelRect);
  connect(cuttingdlg, &CropGifDialog::crop, this,
          [=](int x, int y, int w, int h) {
            CheckEnabled;
            editor->endCrop();
            this->setEditMode(true);
            // gif.crop(x, y, w, h);
            editor->fitPicEditor();
          });
  connect(cuttingdlg, &CropGifDialog::pressCancel, this, [=] {
    CheckEnabled;
    editor->endCrop();
    this->setEditMode(true);
  });
  connect(editor, &GifEditor::selRectChanged, cuttingdlg,
          &CropGifDialog::setSelRect);

#define ConnectShortCut(ShortCut, Slot)                                        \
  s = new QShortcut(ShortCut, this);                                           \
  connect(s, &QShortcut::activated, this, &Slot);

  QShortcut *s;

  ConnectShortCut(QKeySequence::Open, MainWindow::on_open);
  ConnectShortCut(QKeySequence::Save, MainWindow::on_save);
  ConnectShortCut(QKeySequence::SaveAs, MainWindow::on_saveas);
  ConnectShortCut(QKeySequence::Close, MainWindow::on_exit);
  ConnectShortCut(QKeySequence::Undo, MainWindow::on_undo);
  ConnectShortCut(QKeySequence::Redo, MainWindow::on_redo);
  ConnectShortCut(QKeySequence::Cut, MainWindow::on_cut);
  ConnectShortCut(QKeySequence::Copy, MainWindow::on_copy);
  ConnectShortCut(QKeySequence::Paste, MainWindow::on_paste);
  ConnectShortCut(QKeySequence::Delete, MainWindow::on_del);
  ConnectShortCut(QKeySequence::SelectAll, MainWindow::on_selall);
  ConnectShortCut(QKeySequence::Deselect, MainWindow::on_desel);
  ConnectShortCut(keyselrev, MainWindow::on_selreverse);
  ConnectShortCut(keygoto, MainWindow::on_goto);
  ConnectShortCut(keybeginframe, MainWindow::on_beginframe);
  ConnectShortCut(keylastframe, MainWindow::on_last);
  ConnectShortCut(keyplay, MainWindow::on_play);
  ConnectShortCut(keypause, MainWindow::on_stop);
  ConnectShortCut(keynextframe, MainWindow::on_next);
  ConnectShortCut(keylastframe, MainWindow::on_endframe);
  ConnectShortCut(keyreduceframe, MainWindow::on_decreaseframe);
  ConnectShortCut(keyrmleft, MainWindow::on_delbefore);
  ConnectShortCut(keyrmright, MainWindow::on_delafter);
  ConnectShortCut(keymvl, MainWindow::on_moveleft);
  ConnectShortCut(keymvr, MainWindow::on_moveright);
  ConnectShortCut(keyrev, MainWindow::on_reverse);
  ConnectShortCut(keysetdelay, MainWindow::on_setdelay);
  ConnectShortCut(keyscalepic, MainWindow::on_scalepic);
  ConnectShortCut(keycutpic, MainWindow::on_cutpic);
  ConnectShortCut(keyscaledelay, MainWindow::on_scaledelay);

  loadWindowStatus();
}

void MainWindow::openGif(QString filename) {
  if (!gif.load(filename)) {
    setEditMode(false);
    DMessageManager::instance()->sendMessage(this, ICONRES("icon"),
                                             "OpenError");
    QMessageBox::critical(this, "",
                          QString(GifErrorString(gif.getLastError())));
    return;
  }
  showWaitNotify();
  curfilename = filename;
  imglist->setCurrentRow(0);
  editor->fitPicEditor();
  setEditMode(true);
  setSaved(true);
  setWritable(QFileInfo(filename).permission(QFile::WriteUser));
  showGifMessage();
  showProcessSuccess();
}

bool MainWindow::checkIsGif(QString filename) {
  QFile f(filename);
  if (f.open(QFile::ReadOnly)) {
    char *bu = new char[GIF_STAMP_LEN];
    f.read(bu, GIF_STAMP_LEN);
    return !memcmp(GIF_STAMP, bu, GIF_STAMP_LEN) ||
           !memcmp(GIF87_STAMP, bu, GIF_STAMP_LEN) ||
           !memcmp(GIF89_STAMP, bu, GIF_STAMP_LEN);
  }
  return false;
}

void MainWindow::refreshImglist() {
  auto pos = imglist->currentRow();
  imglist->clear();
  auto len = gif.frameCount();
  for (int i = 0; i < len; i++) {
    auto p = new QListWidgetItem(
        gif.thumbnail(i), QString("%1   %2 ms").arg(i).arg(gif.frameDelay(i)),
        imglist);
    p->setSizeHint(QSize(120, 120));
  }
  imglist->setCurrentRow(pos);
}

void MainWindow::refreshListLabel(int start) {
  auto len = gif.frameCount();
  for (int i = start; i < len; i++) {
    imglist->item(i)->setText(
        QString("%1   %2 ms").arg(i).arg(gif.frameDelay(i)));
  }
}

void MainWindow::setEditMode(bool b) {
  for (auto item : distool)
    item->setEnabled(b);
  for (auto item : dismenu)
    item->setEnabled(item);
  iSetfitInView->setEnabled(b);
}

bool MainWindow::ensureSafeClose() {
  if (!undo.isClean()) {
    if (QMessageBox::question(
            this, tr("ConfirmClose"),
            tr("ConfirmSave") + "\n" +
                (curfilename == ":" ? tr("Untitle") : curfilename)) ==
        QMessageBox::No)
      return false;
  }
  return true;
}

void MainWindow::setSaved(bool b) {
  iSaved->setPixmap(b ? infoSaved : infoUnsaved);
}

void MainWindow::setWritable(bool b) {
  iReadWrite->setPixmap(b ? infoWriteable : infoReadonly);
}

void MainWindow::showGifMessage(QString message) {
  status->showMessage(QString(5, ' ') + message +
                      QString(tr("%1 frame | %2 total")
                                  .arg(imglist->currentRow())
                                  .arg(gif.frameCount())));
}

void MainWindow::saveWindowStatus() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::loadWindowStatus() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());
}

void MainWindow::showWaitNotify() {
  DMessageManager::instance()->sendMessage(this, ICONRES("icon"),
                                           tr("PleaseWait!"));
}

void MainWindow::showProcessSuccess() {
  DMessageManager::instance()->sendMessage(this, ICONRES("icon"),
                                           tr("ProcessSuccess"));
}

bool MainWindow::saveGif(QString filename) {
  GifEncoder gifsaver;
  auto size = gif.size();
  if (gifsaver.init(filename.toStdString().c_str(), uint16_t(size.width()),
                    uint16_t(size.height()), 0)) {
    std::vector<std::vector<uint32_t>> images;
    std::vector<uint32_t> delay;
    for (auto &frame : gif.frames()) {
      auto &img = frame.image;
      std::vector<uint32_t> buffer(ulong(img.sizeInBytes()));
      memcpy(buffer.data(), img.constBits(), buffer.size());
      images.push_back(buffer);
      delay.push_back(uint32_t(frame.delayTime));
    }
    gifsaver.addImages(images, delay);
    gifsaver.finishEncoding();
    return true;
  }
  return false;
}

void MainWindow::on_new_frompics() {
  if (ensureSafeClose()) {
    NewDialog d(NewType::FromPics, this);
    if (d.exec()) {
      showWaitNotify();
      gif.loadfromImages(d.getResult());
      showProcessSuccess();
      curfilename = ":"; //表示新建
      setSaved(false);
      setWritable(true);
      setEditMode(true);
      imglist->setCurrentRow(0);
      showGifMessage();
      showProcessSuccess();
    }
  }
}

void MainWindow::on_new_fromgifs() {
  if (ensureSafeClose()) {
    NewDialog d(NewType::FromGifs, this);
    if (d.exec()) {
      showWaitNotify();
      gif.loadfromGifs(d.getResult());
      curfilename = ":"; //表示新建
      setSaved(false);
      setWritable(true);
      setEditMode(true);
      imglist->setCurrentRow(0);
      showGifMessage();
      showProcessSuccess();
    }
  }
}

void MainWindow::on_open() {
  if (ensureSafeClose()) {
    auto filename = QFileDialog::getOpenFileName(this, tr("ChooseFile"),
                                                 lastusedpath, "gif (*.gif)");
    if (filename.isEmpty())
      return;

    lastusedpath = QFileInfo(filename).absoluteDir().absolutePath();

    if (checkIsGif(filename))
      openGif(filename);
  }
}

void MainWindow::on_del() {
  CheckEnabled;
  QVector<int> indices;
  for (auto item : imglist->selectionModel()->selectedIndexes()) {
    indices.append(item.row());
  }
  // undo.push(new RemoveFrameCommand(&gif, indices));
}

void MainWindow::on_selall() {
  CheckEnabled;
  imglist->selectAll();
}

void MainWindow::on_selreverse() {
  CheckEnabled;
  auto len = imglist->count();
  for (auto i = 0; i < len; i++) {
    auto item = imglist->item(i);
    item->setSelected(!item->isSelected());
  }
}

void MainWindow::on_desel() {
  CheckEnabled;
  auto cur = imglist->currentRow();
  imglist->setCurrentRow(cur);
}

void MainWindow::on_goto() {
  CheckEnabled;
  bool ok;
  auto index =
      DInputDialog::getInt(this, tr("Goto"), tr("PleaseInputIndex"),
                           imglist->currentRow(), 0, imglist->count(), 1, &ok);
  if (ok) {
    imglist->setCurrentRow(index);
  }
}

void MainWindow::on_beginframe() {
  CheckEnabled;
  imglist->setCurrentRow(0);
}

void MainWindow::on_last() {
  CheckEnabled;
  auto pos = imglist->currentRow();
  if (pos > 0) {
    imglist->setCurrentRow(pos - 1);
  }
}

void MainWindow::on_play() {
  CheckEnabled;
  QList<int> ints;
  auto len = gif.frameCount();
  for (auto i = 0; i < len; i++) {
    ints.append(gif.frameDelay(i));
  }
  player->setTickIntervals(ints);
  player->play(imglist->currentRow());
}

void MainWindow::on_stop() {
  CheckEnabled;
  player->stop();
}

void MainWindow::on_next() {
  CheckEnabled;
  auto pos = imglist->currentRow();
  auto max = imglist->count();
  if (pos < max - 1) {
    imglist->setCurrentRow(pos + 1);
  }
}

void MainWindow::on_endframe() {
  CheckEnabled;
  imglist->setCurrentRow(imglist->count() - 1);
}

void MainWindow::on_decreaseframe() {
  CheckEnabled;
  ReduceFrameDialog d(imglist->count(), this);
  if (d.exec()) {
    auto res = d.getResult();
    QVector<int> delindices, modinter;
    QVector<Magick::Image> delimgs;
    //    gif.getReduceFrame(res.start, res.end, res.stepcount, delindices,
    //    delimgs,
    //                       modinter);
    //    undo.push(new ReduceFrameCommand(&gif, delindices, delimgs,
    //    modinter));
  }
}

void MainWindow::on_delbefore() {
  CheckEnabled;
  auto pos = imglist->currentRow();
  // undo.push(new DelFrameDirCommand(&gif, pos, DelDirection::Before,
  // imglist));
}

void MainWindow::on_delafter() {
  CheckEnabled;
  auto pos = imglist->currentRow();
  // undo.push(new DelFrameDirCommand(&gif, pos, DelDirection::After, imglist));
}

void MainWindow::on_saveas() {
  CheckEnabled;
  auto filename = QFileDialog::getSaveFileName(this, tr("ChooseSaveFile"),
                                               lastusedpath, "gif (*.gif)");
  if (filename.isEmpty())
    return;
  lastusedpath = QFileInfo(filename).absoluteDir().absolutePath();

  if (saveGif(filename)) {
    DMessageManager::instance()->sendMessage(this, ICONRES("saveas"),
                                             tr("SaveAsSuccess"));
    curfilename = filename;
  } else {
    DMessageManager::instance()->sendMessage(this, ICONRES("saveas"),
                                             "SaveAsFail");
  }
}

void MainWindow::on_export() {
  CheckEnabled;
  ExportDialog d;
  if (d.exec()) {
    auto res = d.getResult();
    QString ext;
    switch (res.type) {
    case ExportImageType::PNG: {
      ext = "png";
      break;
    }
    case ExportImageType::JPG: {
      ext = "jpg";
      break;
    }
    case ExportImageType::TIFF: {
      ext = "tiff";
      break;
    }
    case ExportImageType::WEBP: {
      ext = "webp";
      break;
    }
    }
    if (gif.exportImages(res.path, ext)) {
      DMessageManager::instance()->sendMessage(this, ICONRES("saveas"),
                                               tr("ExportSuccess"));
    } else {
      DMessageManager::instance()->sendMessage(this, ICONRES("saveas"),
                                               tr("ExportFail"));
    }
  }
}

void MainWindow::on_exit() {
  if (ensureSafeClose()) {
    gif.close();
    editor->setBackgroudPix(QPixmap(":/images/icon.png"));
    editor->scale(1, 1);
    iSaved->setPixmap(infoSaveg);
    iReadWrite->setPixmap(inforwg);
    status->showMessage("");
    curfilename.clear();
  }
}

void MainWindow::on_undo() {
  CheckEnabled;
  undo.undo();
}

void MainWindow::on_redo() {
  CheckEnabled;
  undo.redo();
}

void MainWindow::on_copy() {
  CheckEnabled;
  auto sels = imglist->selectionModel()->selectedRows();
  QVector<int> indices;
  for (auto &i : sels) {
    indices.append(i.row());
  }
  // clip->setImageFrames(indices);
}

void MainWindow::on_cut() {
  CheckEnabled;
  auto sels = imglist->selectionModel()->selectedRows();
  QVector<int> indices;
  for (auto &i : sels) {
    indices.append(i.row());
  }
  // clip->setImageFrames(indices);
  // undo.push(new RemoveFrameCommand(&gif, indices));
}

void MainWindow::on_paste() {
  CheckEnabled;
  auto pos = imglist->currentRow() + 1;
  QVector<Magick::Image> imgs;
  // clip->getImageFrames(imgs);
  if (imgs.count()) {
    // undo.push(new InsertFrameCommand(&gif, imglist, pos, imgs));
  }
}

void MainWindow::on_save() {
  CheckEnabled;
  if (curfilename == ":") {
    on_saveas();
    return;
  }

  if (saveGif(curfilename)) {
    undo.setClean();
    DMessageManager::instance()->sendMessage(this, ICONRES("save"),
                                             tr("SaveSuccess"));
  } else {
    DMessageManager::instance()->sendMessage(this, ICONRES("save"), "SaveFail");
  }
}

void MainWindow::on_reverse() {
  CheckEnabled;
  // undo.push(new ReverseFrameCommand(&gif));
  refreshImglist();
}

void MainWindow::on_moveleft() {
  CheckEnabled;
  auto pos = imglist->currentRow();
  // undo.push(new MoveFrameCommand(&gif, imglist, MoveFrameDirection::Left,
  // pos));
}

void MainWindow::on_moveright() {
  CheckEnabled;
  auto pos = imglist->currentRow();
  //  undo.push(
  //      new MoveFrameCommand(&gif, imglist, MoveFrameDirection::Right, pos));
}

void MainWindow::on_createreverse() {
  CheckEnabled;
  CreateReverseDialog d(imglist->count(), this);
  if (d.exec()) {
    auto res = d.getResult();
    QVector<Magick::Image> imgs;
    //    gif.getReverse(res.start, res.end, imgs);
    //    undo.push(
    //        new InsertFrameCommand(&gif, imglist, imglist->currentRow(),
    //        imgs));
  }
}

void MainWindow::on_setdelay() {
  CheckEnabled;
  auto indices = imglist->selectionModel()->selectedRows();
  bool ok;
  auto time10s = DInputDialog::getInt(this, tr("DelayTime"), tr("Input10ms"), 2,
                                      1, INT_MAX, 1, &ok);
  if (ok) {
    auto time = time10s * 10;
    QVector<int> is;
    for (auto i : indices) {
      is.append(i.row());
    }
    // undo.push(new DelayFrameCommand(&gif, is, time));
  }
}

void MainWindow::on_scaledelay() {
  CheckEnabled;
  auto indices = imglist->selectionModel()->selectedRows();
  bool ok;
  auto scale = DInputDialog::getInt(this, tr("ScaleDelayTime"),
                                    tr("InputPercent"), 100, 1, 100, 1, &ok);
  if (ok) {
    QVector<int> is;
    for (auto i : indices) {
      is.append(i.row());
    }
    // undo.push(new DelayScaleCommand(&gif, is, scale));
  }
}

void MainWindow::on_insertpic() {
  CheckEnabled;
  auto filenames = QFileDialog::getOpenFileNames(
      this, tr("ChooseFile"), lastusedpath, tr("Images (*.jpg *.tiff *.png)"));

  if (filenames.isEmpty())
    return;
  lastusedpath = QFileInfo(filenames.first()).absoluteDir().absolutePath();
  auto pos = imglist->currentRow() + 1;
  QVector<Magick::Image> imgs;
  // gif.getNativeImages(filenames, imgs);
  // undo.push(new InsertFrameCommand(&gif, imglist, pos, imgs));
}

void MainWindow::on_merge() {
  CheckEnabled;
  auto filenames = QFileDialog::getOpenFileNames(this, tr("ChooseFile"),
                                                 lastusedpath, "gif (*.gif)");

  if (filenames.isEmpty())
    return;
  lastusedpath = QFileInfo(filenames.first()).absoluteDir().absolutePath();
  auto pos = imglist->currentRow() + 1;
  QVector<Magick::Image> imgs;
  // gif.getNativeMergeGifFrames(filenames, imgs);
  // undo.push(new InsertFrameCommand(&gif, imglist, pos, imgs));
}

void MainWindow::on_scalepic() {
  CheckEnabled;
  ScaleGIFDialog d(gif.size(), this);
  if (d.exec()) {
    auto res = d.getResult();
    // undo.push(new ScaleFrameCommand(&gif, res.width, res.height, imglist));
  }
}

void MainWindow::on_cutpic() {
  CheckEnabled;
  setEditMode(false);
  QRectF rect;
  editor->initCrop(rect);
  cuttingdlg->setMaxSize(rect.size().toSize());
  cuttingdlg->setSelRect(rect);
  cuttingdlg->show();
}

void MainWindow::on_fliph() {
  CheckEnabled;
  // undo.push(new FlipFrameCommand(&gif, FlipDirection::Horizontal));
}

void MainWindow::on_flipv() {
  CheckEnabled;
  // undo.push(new FlipFrameCommand(&gif, FlipDirection::Vertical));
}

void MainWindow::on_clockwise() {
  CheckEnabled;
  // undo.push(new RotateFrameCommand(&gif, true));
}

void MainWindow::on_anticlockwise() {
  CheckEnabled;
  // undo.push(new RotateFrameCommand(&gif, false));
}

void MainWindow::on_exportapply() {
  CheckEnabled;
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
  CheckEnabled;
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

  QVector<int> rows;
  for (auto i : indices)
    rows.append(i.row());

  QVector<Magick::Image> imgs;
  //  if (gif.getModeledFrames(filename, rows, imgs)) {
  //    undo.push(new ReplaceFrameCommand(&gif, rows, imgs));
  //  } else {
  //    DMessageManager::instance()->sendMessage(this, ICONRES("model"),
  //                                             tr("InvalidModel"));
  //  }
}

void MainWindow::on_onion() {
  CheckEnabled;
  bool ok;
  auto index =
      DInputDialog::getInt(this, tr("OnionMask"), tr("PleaseInputIndex"),
                           imglist->currentRow(), 0, imglist->count(), 1, &ok);
  if (ok) {
    gif.setOnionIndex(index);
  }
}

void MainWindow::on_about() {
  AboutSoftwareDialog d;
  d.exec();
}

void MainWindow::on_sponsor() {
  SponsorDialog d;
  d.exec();
}

void MainWindow::on_wiki() {
  QDesktopServices::openUrl(QUrl("https://code.gitlink.org.cn/wingsummer/"
                                 "WingGifEditor/wiki/%E4%BB%8B%E7%BB%8D"));
}

void MainWindow::closeEvent(QCloseEvent *event) {
  player->stop();
  if (ensureSafeClose()) {
    saveWindowStatus();
    event->accept();
  } else
    event->ignore();
}

void MainWindow::resizeEvent(QResizeEvent *event) { Q_UNUSED(event); }

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasUrls())
    event->acceptProposedAction();
  else
    event->ignore();
}

void MainWindow::dropEvent(QDropEvent *event) {
  const QMimeData *mimeData = event->mimeData();
  if (mimeData->hasUrls()) {
    auto filename = mimeData->urls().first().toLocalFile();
  }
}
