#include "newdialog.h"
#include <DDialogButtonBox>
#include <DFileDialog>
#include <DMessageManager>
#include <QShortcut>
#include <QWidget>

NewDialog::NewDialog(NewType type, DMainWindow *parent) : DDialog(parent) {
  setWindowTitle(tr("New"));
  btnbox = new DButtonBox(this);
  QList<DButtonBoxButton *> blist;
  auto b = new DButtonBoxButton(tr("Add"), this);
  connect(b, &DButtonBoxButton::clicked, [=] {
    QFileDialog d;
    if (type == NewType::FromPics) {
      d.setNameFilter(tr("Images (*.jpg *.tiff *.png)"));
    } else {
      d.setNameFilter("GIF (*.gif)");
    }
    d.setAcceptMode(QFileDialog::AcceptOpen);
    d.setFileMode(QFileDialog::ExistingFiles);
    if (d.exec()) {
      auto files = d.selectedFiles();
      for (auto item : files) {
        QImage img;
        if (img.load(item)) {
          filenames << item;
          auto p = new QListWidgetItem(QIcon(":/images/picture.png"), item,
                                       imgslist);
          p->setToolTip(item);
        }
      }
    }
  });
  blist.append(b);
  b = new DButtonBoxButton(tr("Remove"), this);
  connect(b, &DButtonBoxButton::clicked, [=] {
    auto indices = imgslist->selectionModel()->selectedRows();
    QList<int> ins;
    for (auto i : indices) {
      ins.append(i.row());
    }
    std::sort(ins.begin(), ins.end(), std::greater<int>());
    for (auto i : ins) {
      auto p = imgslist->item(i);
      imgslist->removeItemWidget(p);
      delete p;
      filenames.removeAt(i);
    }
  });
  blist.append(b);
  b = new DButtonBoxButton(tr("Clear"), this);
  connect(b, &DButtonBoxButton::clicked, [=] {
    imgslist->clear();
    filenames.clear();
  });
  blist.append(b);
  b = new DButtonBoxButton(tr("MoveUp"), this);
  connect(b, &DButtonBoxButton::clicked, [=] {
    auto si = imgslist->selectionModel()->selectedRows();
    auto len = si.count();
    QVector<int> indices(len);
    auto i = 0;
    for (auto &item : si) {
      indices[i] = item.row();
    }
    std::sort(indices.begin(), indices.end());
    if (indices[0] == 0) {
      for (i = 1; i < len; i++) {
        if (indices[i] != i)
          break;
      }
    }

    for (; i < len; i++) {
      auto p = imgslist->takeItem(i);
      imgslist->insertItem(i - 1, p);
      filenames.move(i, i - 1);
    }
  });
  blist.append(b);
  b = new DButtonBoxButton(tr("MoveDown"), this);
  connect(b, &DButtonBoxButton::clicked, [=] {
    auto si = imgslist->selectionModel()->selectedRows();
    auto len = si.count();
    QVector<int> indices(len);
    auto i = 0;
    for (auto &item : si) {
      indices[i] = item.row();
    }
    std::sort(indices.begin(), indices.end(), std::greater<int>());
    if (indices[0] == len - 1) {
      for (i = 1; i < len; i++) {
        if (indices[i] != len - 1 - i)
          break;
      }
    }
    for (; i < len; i++) {
      auto p = imgslist->takeItem(i);
      imgslist->insertItem(i + 1, p);
      filenames.move(i, i + 1);
    }
  });
  blist.append(b);
  btnbox->setButtonList(blist, false);
  addContent(btnbox);
  addSpacing(10);
  auto w = new QWidget(this);
  auto hbox = new QHBoxLayout(w);
  imgslist = new DListWidget(this);
  connect(imgslist, &DListWidget::itemSelectionChanged, this, [=] {

  });
  hbox->addWidget(imgslist);
  auto vbox = new QVBoxLayout(w);
  hbox->addLayout(vbox);
  imgview = new DLabel(this);
  imgview->setFixedSize(200, 200);
  imgview->setScaledContents(true);
  vbox->addWidget(imgview);
  vbox->addSpacing(5);
  vbox->addWidget(new DLabel(tr("Size"), this));
  imgsize = new DLabel(this);
  vbox->addWidget(imgsize);
  addContent(w);
  addSpacing(5);
  addContent(new DLabel(tr("FirstSizeJudge"), this));
  addSpacing(20);
  auto dbbox = new DDialogButtonBox(
      DDialogButtonBox::Ok | DDialogButtonBox::Cancel, this);
  connect(dbbox, &DDialogButtonBox::accepted, this, &NewDialog::on_accept);
  connect(dbbox, &DDialogButtonBox::rejected, this, &NewDialog::on_reject);
  auto key = QKeySequence(Qt::Key_Return);
  auto s = new QShortcut(key, this);
  connect(s, &QShortcut::activated, this, &NewDialog::on_accept);
  addContent(dbbox);
}

QStringList NewDialog::getResult() { return filenames; }

void NewDialog::on_accept() {
  if (filenames.count()) {
    done(1);
    return;
  }
  DMessageManager::instance()->sendMessage(this, QIcon(":/images/new.png"),
                                           tr("NoImageFile"));
}

void NewDialog::on_reject() { done(0); }

void NewDialog::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event)
  done(0);
}
