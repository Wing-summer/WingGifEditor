#include "exportdialog.h"
#include <DDialogButtonBox>
#include <DLabel>
#include <QDir>
#include <QShortcut>
#include <QStandardPaths>

ExportDialog::ExportDialog(DMainWindow *parent) : DDialog(parent) {
  setWindowTitle(tr("Export"));
  addContent(new DLabel(tr("ChooseFolder"), this));
  addSpacing(3);
  folder = new DFileChooserEdit(this);
  folder->setFileMode(QFileDialog::FileMode::Directory);
  QDir dir(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
  dir.mkdir("WingGifEditor");
  folder->setText(dir.absolutePath() + "/" + "WingGifEditor");
  auto l = folder->lineEdit();
  l->setReadOnly(true);
  connect(l, &QLineEdit::textChanged, this,
          [=] { folder->setToolTip(l->text()); });
  addContent(folder);
  addSpacing(10);
  btnbox = new DButtonBox(this);
  QList<DButtonBoxButton *> blist;
  auto b = new DButtonBoxButton("JPG", this);
  connect(b, &DButtonBoxButton::toggled, [=](bool b) {
    if (b)
      res.type = ExportImageType::JPG;
  });
  blist.append(b);
  b = new DButtonBoxButton("TIFF", this);
  connect(b, &DButtonBoxButton::toggled, [=](bool b) {
    if (b)
      res.type = ExportImageType::TIFF;
  });
  blist.append(b);
  b = new DButtonBoxButton("WEBP", this);
  connect(b, &DButtonBoxButton::toggled, [=](bool b) {
    if (b)
      res.type = ExportImageType::WEBP;
  });
  blist.append(b);
  b = new DButtonBoxButton("PNG", this);
  connect(b, &DButtonBoxButton::toggled, [=](bool b) {
    if (b)
      res.type = ExportImageType::PNG;
  });
  blist.prepend(b);
  btnbox->setButtonList(blist, true);
  b->setChecked(true);
  addContent(btnbox);
  addSpacing(20);
  auto dbbox = new DDialogButtonBox(
      DDialogButtonBox::Ok | DDialogButtonBox::Cancel, this);
  connect(dbbox, &DDialogButtonBox::accepted, this, &ExportDialog::on_accept);
  connect(dbbox, &DDialogButtonBox::rejected, this, &ExportDialog::on_reject);
  auto key = QKeySequence(Qt::Key_Return);
  auto s = new QShortcut(key, this);
  connect(s, &QShortcut::activated, this, &ExportDialog::on_accept);
  addContent(dbbox);
}

ExportResult ExportDialog::getResult() { return res; }

void ExportDialog::on_accept() {
  res.path = folder->text();
  done(1);
}

void ExportDialog::on_reject() { done(0); }

void ExportDialog::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event);
  done(0);
}
