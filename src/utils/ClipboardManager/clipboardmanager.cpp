#include "ClipboardManager.h"
#include <QClipboard>
#include <QApplication>

ClipboardManager& ClipboardManager::instance() {
    static ClipboardManager instance;
    return instance;
}

ClipboardManager::ClipboardManager() = default;

ClipboardManager::~ClipboardManager() {
    clearBackup();
}

void ClipboardManager::backup() {
    clearBackup();

    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *original = clipboard->mimeData();
    if (!original) return;

    _backup = new QMimeData();
    for (const QString &format : original->formats()) {
        _backup->setData(format, original->data(format));
    }
}

void ClipboardManager::setTemporaryPixmap(const QPixmap &pixmap) {
    QApplication::clipboard()->setPixmap(pixmap);
}

void ClipboardManager::restore() {
    if (_backup) {
        QApplication::clipboard()->setMimeData(_backup);
        _backup = nullptr;
    }
}

void ClipboardManager::clearBackup() {
    if (_backup) {
        delete _backup;
        _backup = nullptr;
    }
}
